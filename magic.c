#include "magic.h"

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
    // arbre
    // hachage
};

enum MAGICDirection
{
    STREAM_IN_OUT = 0,
    STREAM_OUT_IN = 1
};

typedef enum
{
    RED,
    BLACK
} Color;

// Structure for a Red Black Tree Node
typedef struct RedBlackTreeNode
{
    int key;   // Node key
    int value; // output value (-1 if deleted)
    Color color;
    struct Node *parent, *left, *right;
} Node;

// Structure for the Red Black Tree
typedef struct RedBlackTree
{
    Node *NIL; // black leaf
    Node *root;
    Node **deleted_elements;
    int deleted_count;
} Magic;

MAGIC MAGICinit()
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
}

void MAGICadd(MAGIC m, int pos, int length)
{
    if (!m)
        return;
}

int MAGICmap(MAGIC m, MAGICDirection direction, int pos)
{
    if (!m)
        return -1;
    return 0;
}

void MAGICdestroy(MAGIC m)
{
    if (!m)
        return;
    free(m);
}
