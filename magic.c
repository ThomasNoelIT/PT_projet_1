#include "magic.h"

typedef enum{
    RED,
    BLACK
} Color;

typedef struct RedBlackTreeNode {
    int pos; 
    int delta; 
    int lazyShift;  
    struct RedBlackTreeNode *left, *right, *parent;
    Color color;    
} RBNode;

// Structure for the Red-Black Tree
typedef struct RedBlackTree{
    RBNode *NIL; 
    RBNode *root; 
} RBTree;

RBTree *RBTreeInit(void) {
    RBTree *tree = (RBTree*)malloc(sizeof(RBTree));
    if (!tree) return NULL;
    tree->NIL = (RBNode*)malloc(sizeof(RBNode));
    if (!tree->NIL) {
        free(tree);
        return NULL;
    }
    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL->right = tree->NIL->parent = NULL;
    tree->NIL->pos = tree->NIL->delta = tree->NIL->lazyShift = 0;
    tree->root = tree->NIL;
    return tree;
}

RBNode *createNode(RBTree *tree, int pos, int delta) {
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    if (!node) return NULL;
    node->pos = pos;
    node->delta = delta;
    node->lazyShift = 0;
    node->left = node->right = node->parent = tree->NIL;
    node->color = RED; 
    return node;
}

void leftRotate(RBTree *tree, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != tree->NIL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == tree->NIL) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(RBTree *tree, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != tree->NIL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == tree->NIL) {
        tree->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

void fixInsert(RBTree *tree, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void updatelazyShift(RBNode *node, RBTree *tree) {
    if (node == tree->NIL) return;

    node->lazyShift = node->delta;
    if (node->left != tree->NIL) {
        node->lazyShift += node->left->lazyShift;
    }
    if (node->right != tree->NIL) {
        node->lazyShift += node->right->lazyShift;
    }
}

void RBTreeInsert(RBTree *tree, int pos, int delta) {
    RBNode *z = createNode(tree, pos, delta);
    if (!z) return;

    RBNode *y = tree->NIL;
    RBNode *x = tree->root;

    while (x != tree->NIL) {
        y = x;
        if (z->pos < x->pos) {
            x->lazyShift += z->delta; //If the new node is lower, we update the totalShift of the current node.
            x = x->left;
        } else {
            z->lazyShift += x->lazyShift;
            x = x->right;
        }
    }

    z->parent = y;
    if (y == tree->NIL) {
        tree->root = z;
    } else if (z->pos < y->pos) {
        y->left = z;
    } else {
        y->right = z;
    }

    fixInsert(tree, z);

    updatelazyShift(z, tree);
}


void RBTreeFreeNodes(RBTree *tree, RBNode *node) {
    if (node != tree->NIL) {
        RBTreeFreeNodes(tree, node->left);
        RBTreeFreeNodes(tree, node->right);
        free(node);
    }
}

void RBTreeDestroy(RBTree *tree) {
    if (!tree) return;

    RBTreeFreeNodes(tree, tree->root);
    free(tree->NIL);
    free(tree);
}

int RBTreeFindMapping(RBTree *tree, int pos, MAGICDirection direction) {
    int shift = 0;
    RBNode *current = tree->root;
    if(!direction){
        while (current != tree->NIL) {// trouver un moyen de trouver le plus grand noeud, plus petit que pos
            if (pos < current->pos) {
                current = current->left;
            } else {
                shift += current->lazyShift;
                current = current->right;
            }
        }
        if(current->pos != pos){
            if((pos - (current->pos + shift)) < 0){
                return -1;
            }
            else{
                return pos + shift;
            }
        }else{
            if((pos + shift) < 0){
                return -1;
            }else{
                return pos + shift;
            }
        }
    }else{
        while (current != tree->NIL) {// trouver un moyen de trouver le plus grand noeud shifter, plus petit que pos
            if (pos < (current->pos + shift)) {
                current = current->left;
            } else {
                shift += current->lazyShift;
                current = current->right;
            }
        }
        if(current->pos != pos){

        }
    }

    return pos + shift;
}








// Partie MAGIC

struct magic{
    RBTree *rb_tree_in_out;
    RBTree *rb_tree_out_in;
};

MAGIC MAGICinit(void){
    MAGIC m = (MAGIC)malloc(sizeof(struct magic));
    if (!m)
        return NULL;
    m->rb_tree_in_out = RBTreeInit();
    m->rb_tree_out_in = RBTreeInit();
    if (!m->rb_tree_in_out || !m->rb_tree_out_in){
        free(m);
        return NULL;
    }
    return m;
}

void MAGICremove(MAGIC m, int pos, int length){
    if (!m || length <= 0)
        return;

    RBTreeInsert(m->rb_tree_in_out, pos, -length);
    RBTreeInsert(m->rb_tree_out_in, pos, -length);
}

void MAGICadd(MAGIC m, int pos, int length){
    if (!m || length <= 0)
        return;

    RBTreeInsert(m->rb_tree_in_out, pos, length);
    RBTreeInsert(m->rb_tree_out_in, pos, length);
}

int MAGICmap(MAGIC m, MAGICDirection direction, int pos){
    if (!m || pos < 0)
        return -1;

    if (direction == STREAM_IN_OUT)
        return RBTreeFindMapping(m->rb_tree_in_out, pos, direction);
    else
        return RBTreeFindMapping(m->rb_tree_out_in, pos, direction);
}

void MAGICdestroy(MAGIC m){
    if (!m)
        return;

    RBTreeDestroy(m->rb_tree_in_out);
    RBTreeDestroy(m->rb_tree_out_in);
    free(m);
}
