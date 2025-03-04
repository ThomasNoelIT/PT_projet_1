#include "magic.h"

// partie rbt
typedef enum
{
    RED,
    BLACK
} Color;

// Structure for a Red-Black Tree Node
typedef struct RedBlackTreeNode
{
    int key;   // Node key
    int value; // Output value (-1 if deleted)
    Color color;
    struct RedBlackTreeNode *parent, *left, *right;
} RedBlackTreeNode;

// Structure for the Red-Black Tree
typedef struct RedBlackTree
{
    RedBlackTreeNode *NIL; // Black leaf
    RedBlackTreeNode *root;
    // RedBlackTreeNode **deleted_elements;
    // int deleted_count;
} RedBlackTree; // Correction du nom de la structure

// partie magic
struct modification
{
    int pos;
    int length;
    int type;
    struct modification *next;
    struct modification *prev;
};

struct magic
{
    int input_size;
    int output_size;
    struct modification *modifications;
    RedBlackTree *rb_tree; // Ajout correct de l'arbre
    // hachage
};

enum MAGICDirection
{
    STREAM_IN_OUT = 0,
    STREAM_OUT_IN = 1
};

MAGIC MAGICinit(void)
{
    MAGIC m = (MAGIC)malloc(sizeof(struct magic));
    if (!m)
        return NULL;
    m->input_size = 0;
    m->output_size = 0;
    m->modifications = NULL;
    return m;
}

void MAGICremove(MAGIC m, int pos, int length)
{
    if (!m)
        return;
    (void)pos;
    (void)length;
    return;
}

void MAGICadd(MAGIC m, int pos, int length)
{
    if (!m)
        return;
    (void)pos;
    (void)length;
    return;
}

int MAGICmap(MAGIC m, MAGICDirection direction, int pos)
{
    if (!m)
        return -1;
    (void)direction;
    (void)pos;
    return 0;
}

void MAGICdestroy(MAGIC m)
{
    if (!m)
        return;
    free(m);
}
