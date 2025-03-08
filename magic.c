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

RedBlackTree *rb_init(void);
void rb_destroy(RedBlackTree *tree);
void rb_insert(RedBlackTree *tree, int input_pos, int output_pos, int length);
void rb_delete_range(RedBlackTree *tree, int pos, int length);
int rb_find_mapping(RedBlackTree *tree, int pos, MAGICDirection direction);







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
    
    rb_delete_range(m->rb_tree, pos, length);
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
