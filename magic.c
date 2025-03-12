#include "magic.h"

typedef enum{
    RED,
    BLACK
} Color;

typedef struct RedBlackTreeNode {
    int pos;         // Position where the modification start
    int delta;       // bytes added (+) or deleted (-)
    int totalShift;  // cumulated shift after modif
    struct RedBlackTreeNode *left, *right, *parent;
    Color color;       // Rouge ou Noir
} RBNode;

// Structure for the Red-Black Tree
typedef struct RedBlackTree{
    RBNode *NIL; // Black leaf
    RBNode *root;
} RBTree;

RBTree *RBTreeInit(void);
RBNode *createNode(RBTree *tree, int pos, int delta);
void RBTreeLeftRotate(RBTree *tree, RBNode *x);
void RBTreeRightRotate(RBTree *tree, RBNode *y);
void RBTreeFixInsert(RBTree *tree, RBNode *node);
void RBTreeInsert(RBTree *tree, int pos, int delta);
RBNode *RBTreeMinimum(RBTree *tree, RBNode *node);
RBTree *RBTreeTransplant(RBTree *tree, RBNode *u, RBNode *v);
void RBTreeDestroyRec(RBTree *tree, RBNode *node);
void RBTreeDestroy(RBTree *tree);
RBNode *RBTreeDelete(RBTree *tree, RBNode *node);
void RBTreeRemove(RBTree *tree, int pos, int length);
int RBTreeFindMapping(RBTree *tree, int pos, MAGICDirection direction);

void RBTreeDestroyRec(RBTree *tree, RBNode *node){
    if (node == tree->NIL)
        return;
    
    RBTreeDestroyRec(tree, node->left);
    RBTreeDestroyRec(tree, node->right);
    free(node);
}

void RBTreeDestroy(RBTree *tree){
    if (!tree)
        return;
    
    RBTreeDestroyRec(tree, tree->root);
    free(tree->NIL);
    free(tree);
}

RBTree *RBTreeInit(void) {
    RBTree *tree = (RBTree *)malloc(sizeof(RBTree));
    if (!tree) {
        fprintf(stderr, "Error: Memory allocation failed for RBT structure.\n");
        return NULL;
    }
    
    tree->NIL = (RBNode *)malloc(sizeof(RBNode));
    if (!tree->NIL) {
        fprintf(stderr, "Error: Memory allocation failed for NIL node.\n");
        free(tree);
        return NULL;
    }
    
    // Initialize NIL node
    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL->right = tree->NIL->parent = tree->NIL;  // Reference itself
    tree->NIL->pos = -1;
    tree->NIL->delta = 0;
    tree->NIL->totalShift = 0;

    // Root initially points to NIL
    tree->root = tree->NIL;
    
    return tree;
}

RBNode* createNode(RBTree *tree, int pos, int delta) {
    RBNode *newNode = (RBNode*)malloc(sizeof(RBNode));
    if (!newNode) return NULL;

    newNode->pos = pos;
    newNode->delta = delta;
    newNode->totalShift = delta;// by default
    newNode->left = newNode->right = newNode->parent = tree->NIL;
    newNode->color = RED;

    return newNode;
}

void RBTreeInsert(RBTree *tree, int pos, int delta) {
    RBNode *y = tree->NIL;  // Pointer to the parent of the new node
    RBNode *x = tree->root; // Start from the root

    int shift = 0; // Cumulative shift to update totalShift

    // Search for the insertion position and merge if `pos` already exists
    while (x != tree->NIL) {
        y = x;
        if (pos == x->pos) {
            // If a node at this position already exists, merge `delta`
            x->delta += delta;
            return;
        }
        if (pos < x->pos) {
            x = x->left;
        } else {
            shift += x->delta; // Accumulate shifts before insertion
            x = x->right;
        }
    }

    RBNode *z = createNode(tree, pos, delta);
    if (!z) return;

    z->parent = y;
    if (y == tree->NIL) {
        tree->root = z;
    } else if (pos < y->pos) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->totalShift += shift;

    RBTreeFixInsert(tree, z);
}

void RBTreeFixInsert(RBTree *tree, RBNode *z) {
    while (z->parent->color == RED) {  // While the parent of z is red (violation of property 4)
        if (z->parent == z->parent->parent->left) {  // If the parent of z is the left child of the grandparent
            RBNode *uncle = z->parent->parent->right; 
            if (uncle->color == RED) {  // Case 1: Uncle is red
                // Change the color of the parent and the uncle to black
                z->parent->color = BLACK;
                uncle->color = BLACK;
                // Change the color of the grandparent to red
                z->parent->parent->color = RED;
                z = z->parent->parent;  // Move up to the grandparent
            } else {  // Case 2: Uncle is black
                if (z == z->parent->right) {
                    z = z->parent;
                    LeftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RightRotate(tree, z->parent->parent);
            }
        } else {  // If the parent of z is the right child of the grandparent
            RBNode *uncle = z->parent->parent->left;
            if (uncle->color == RED) {  // Case 1: Uncle is red
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;  // Move up to the grandparent
            } else {  // Case 2: Uncle is black
                if (z == z->parent->left) {
                    z = z->parent;
                    RightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(tree, z->parent->parent);
            }
        }
    }
    // Ensure that the root is always black
    tree->root->color = BLACK;
}


RBNode *RBTreeDelete(RBTree *tree, RBNode *node){
    RBNode *y = node;
    RBNode *x;
    Color original_color = y->color;
    
    if (node->left == tree->NIL) {
        x = node->right;
        RBTreeTransplant(tree, node, node->right);
    } else if (node->right == tree->NIL) {
        x = node->left;
        RBTreeTransplant(tree, node, node->left);
    } else {
        y = rb_minimum(tree, node->right);
        original_color = y->color;
        x = y->right;
        
        if (y->parent == node)
            x->parent = y;
        else {
            RBTreeTransplant(tree, y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }
        RBTreeTransplant(tree, node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }
    
    free(node);
    if (original_color == BLACK)
        rb_fix_delete(tree, x);
    
    return x;
}

void RBTreeRemove(RBTree *tree, int pos, int length){
    RBNode *node = tree->root;
    while (node != tree->NIL) {
        if (pos == node->pos) {
            RBTreeDelete(tree, node);
            return;
        } else if (pos < node->pos)
            node = node->left;
        else
            node = node->right;
    }
} 

void RBTReeLeftRotate(RBTree *tree, RBNode *x) {
    RBNode *y = x->right;      // The node to the right of x becomes the pivot node
    x->right = y->left;        // The left subtree of y becomes the right subtree of x

    if (y->left != tree->NIL) {
        y->left->parent = x;   // The parent of y's left subtree becomes x
    }

    y->parent = x->parent;     // The parent of y becomes the parent of x

    if (x->parent == tree->NIL) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;   // If x is the left child of its parent, y becomes its left child
    } else {
        x->parent->right = y;  // If x is the right child of its parent, y becomes its right child
    }

    y->left = x;               // x becomes the left child of y
    x->parent = y;             // The parent of x becomes y
}


void RBTreeRightRotate(RBTree *tree, RBNode *x) {
    RBNode *y = x->left;       // The node to the left of x becomes the pivot node
    x->left = y->right;        // The right subtree of y becomes the left subtree of x

    if (y->right != tree->NIL) {
        y->right->parent = x;  // The parent of y's right subtree becomes x
    }

    y->parent = x->parent;     // The parent of y becomes the parent of x

    if (x->parent == tree->NIL) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;  // If x is the right child of its parent, y becomes its right child
    } else {
        x->parent->left = y;   // If x is the left child of its parent, y becomes its left child
    }

    y->right = x;              // x becomes the right child of y
    x->parent = y;             // The parent of x becomes y
}

// partie magic
struct magic{
    RBTree *rb_tree;
};

MAGIC MAGICinit(void){
    MAGIC m = (MAGIC)malloc(sizeof(struct magic));
    if (!m)
        return NULL;
    m->rb_tree = RBInit();
    if (!m->rb_tree){
        free(m);
        return NULL;
    }
    return m;
}

void MAGICremove(MAGIC m, int pos, int length){
    if (!m || length <= 0)
        return;
    
    RBTreeInsert(m->rb_tree, pos, length);
}

void MAGICadd(MAGIC m, int pos, int length){
    if (!m || length <= 0)
        return;
    
    RBTreeInsert(m->rb_tree, pos, pos + length, length);
}

int MAGICmap(MAGIC m, MAGICDirection direction, int pos){
    if (!m || pos < 0)
        return -1;
    
    return RBTFindMapping(m->rb_tree, pos, direction);
}

void MAGICdestroy(MAGIC m){
    if (!m)
        return;
    
    rb_destroy(m->rb_tree);
    free(m);
}
