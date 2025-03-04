#include "magic.h"

typedef enum
{
    STREAM_IN_OUT = 0,
    STREAM_OUT_IN = 1
} MAGICDirection;

typedef enum
{
    RED,
    BLACK
} Color;

// partie rbt

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
} RedBlackTree;

RedBlackTree *initRedBlackTree(void);
void destroyRedBlackTree(RedBlackTree *tree);

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
    RedBlackTree *rb_tree;
    // hachage
};

MAGIC MAGICinit(void)
{
    MAGIC m = (MAGIC)malloc(sizeof(struct magic));
    if (!m)
        return NULL;

    m->input_size = 0;
    m->output_size = 0;
    m->modifications = NULL;

    // Initialisation de l'arbre rouge-noir via la fonction dédiée
    m->rb_tree = initRedBlackTree();
    if (!m->rb_tree)
    {
        free(m);
        return NULL;
    }

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

    struct modification *current = m->modifications;
    while (current)
    {
        struct modification *next = current->next;
        free(current);
        current = next;
    }

    destroyRedBlackTree(m->rb_tree);

    free(m);
}