#include "magic.h"

typedef enum{
    RED,
    BLACK
} Color;

// partie rbt

// Structure for a Red-Black Tree Node
typedef struct RedBlackTreeNode{
    int key;   // Node key
    int input_key; // Input key
    int value; // Output value (-1 if deleted)
    Color color;
    struct RedBlackTreeNode *parent, *left, *right;
} RedBlackTreeNode;

// Structure for the Red-Black Tree
typedef struct RedBlackTree{
    RedBlackTreeNode *NIL; // Black leaf
    RedBlackTreeNode *root;
} RedBlackTree;

// Fonctions de gestion de l'arbre rouge-noir
void rb_rotate_left(RedBlackTree *tree, RedBlackTreeNode *x);
void rb_rotate_right(RedBlackTree *tree, RedBlackTreeNode *y);
void rb_fix_insert(RedBlackTree *tree, RedBlackTreeNode *node);
void rb_fix_delete(RedBlackTree *tree, RedBlackTreeNode *node);
RedBlackTreeNode *rb_minimum(RedBlackTree *tree, RedBlackTreeNode *node);
RedBlackTreeNode *rb_transplant(RedBlackTree *tree, RedBlackTreeNode *u, RedBlackTreeNode *v);

RedBlackTree *rb_init(void){
    RedBlackTree *tree = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    if (!tree)
        return NULL;
    
    tree->NIL = (RedBlackTreeNode *)malloc(sizeof(RedBlackTreeNode));
    if (!tree->NIL){
        free(tree);
        return NULL;
    }
    
    tree->NIL->color = BLACK;
    tree->NIL->left = tree->NIL->right = tree->NIL->parent = NULL;
    tree->NIL->key = tree->NIL->input_key = tree->NIL->value = -1;
    
    tree->root = tree->NIL;
    return tree;
}

void rb_destroy_recursive(RedBlackTree *tree, RedBlackTreeNode *node){
    if (node == tree->NIL)
        return;
    
    rb_destroy_recursive(tree, node->left);
    rb_destroy_recursive(tree, node->right);
    free(node);
}

void rb_destroy(RedBlackTree *tree){
    if (!tree)
        return;
    
    rb_destroy_recursive(tree, tree->root);
    free(tree->NIL);
    free(tree);
}

void rb_insert(RedBlackTree *tree, int input_pos, int output_pos, int length){
    RedBlackTreeNode *new_node = (RedBlackTreeNode *)malloc(sizeof(RedBlackTreeNode));
    if (!new_node)
        return;
    
    new_node->key = input_pos;
    new_node->input_key = input_pos;
    new_node->value = output_pos;
    new_node->color = RED;
    new_node->left = tree->NIL;
    new_node->right = tree->NIL;
    
    RedBlackTreeNode *y = tree->NIL;
    RedBlackTreeNode *x = tree->root;
    
    while (x != tree->NIL) {
        y = x;
        if (new_node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    
    new_node->parent = y;
    if (y == tree->NIL)
        tree->root = new_node;
    else if (new_node->key < y->key)
        y->left = new_node;
    else
        y->right = new_node;
    
    rb_fix_insert(tree, new_node);
}

RedBlackTreeNode *rb_delete_node(RedBlackTree *tree, RedBlackTreeNode *node){
    RedBlackTreeNode *y = node;
    RedBlackTreeNode *x;
    Color original_color = y->color;
    
    if (node->left == tree->NIL) {
        x = node->right;
        rb_transplant(tree, node, node->right);
    } else if (node->right == tree->NIL) {
        x = node->left;
        rb_transplant(tree, node, node->left);
    } else {
        y = rb_minimum(tree, node->right);
        original_color = y->color;
        x = y->right;
        
        if (y->parent == node)
            x->parent = y;
        else {
            rb_transplant(tree, y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }
        rb_transplant(tree, node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }
    
    free(node);
    if (original_color == BLACK)
        rb_fix_delete(tree, x);
    
    return x;
}

void rb_remove(RedBlackTree *tree, int pos, int length){
    RedBlackTreeNode *node = tree->root;
    while (node != tree->NIL) {
        if (pos == node->key) {
            rb_delete_node(tree, node);
            return;
        } else if (pos < node->key)
            node = node->left;
        else
            node = node->right;
    }
} 

int rb_find_mapping(RedBlackTree *tree, int pos, MAGICDirection direction){
    RedBlackTreeNode *node = tree->root;
    while (node != tree->NIL) {
        if (pos == node->key)
            return node->value;
        else if (pos < node->key)
            node = node->left;
        else
            node = node->right;
    }
    return -1;
}









// partie magic
struct magic{
    int input_size;
    int output_size;
    RedBlackTree *rb_tree;
};

MAGIC MAGICinit(void){
    MAGIC m = (MAGIC)malloc(sizeof(struct magic));
    if (!m)
        return NULL;

    m->input_size = 0;
    m->output_size = 0;
    m->rb_tree = rb_init();
    if (!m->rb_tree){
        free(m);
        return NULL;
    }
    return m;
}

void MAGICremove(MAGIC m, int pos, int length){
    if (!m || length <= 0)
        return;
    
    rb_remove(m->rb_tree, pos, length);
    m->output_size -= length;
}

void MAGICadd(MAGIC m, int pos, int length){
    if (!m || length <= 0)
        return;
    
    rb_insert(m->rb_tree, pos, pos + length, length);
    m->output_size += length;
}

int MAGICmap(MAGIC m, MAGICDirection direction, int pos){
    if (!m || pos < 0)
        return -1;
    
    return rb_find_mapping(m->rb_tree, pos, direction);
}

void MAGICdestroy(MAGIC m){
    if (!m)
        return;
    
    rb_destroy(m->rb_tree);
    free(m);
}
