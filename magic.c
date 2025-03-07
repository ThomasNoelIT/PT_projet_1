#include "magic.h"

typedef enum{
    RED,
    BLACK
} Color;

// partie rbt

// Structure for a Red-Black Tree Node
typedef struct RedBlackTreeNode{
    int key;   // Node key
    int value; // Output value (-1 if deleted)
    Color color;
    struct RedBlackTreeNode *parent, *left, *right;
} RedBlackTreeNode;

// Structure for the Red-Black Tree
typedef struct RedBlackTree{
    RedBlackTreeNode *NIL; // Black leaf
    RedBlackTreeNode *root;
    // RedBlackTreeNode **deleted_elements;
    // int deleted_count;
} RedBlackTree;

RedBlackTree *rb_init(void);
void rb_destroy(RedBlackTree *tree);
void rb_insert(RedBlackTree *tree, int input_pos, int output_pos, int length);
void rb_delete_range(RedBlackTree *tree, int pos, int length);
int rb_find_mapping(RedBlackTree *tree, int pos, MAGICDirection direction);









// partie magic
struct modification{
    int pos;
    int length;
    int type;
    struct modification *next;
    struct modification *prev;
};

struct magic{
    int input_size;
    int output_size;
    struct modification *modifications;
    RedBlackTree *rb_tree;
    // hachage
};


MAGIC MAGICinit(void){
    MAGIC m = (MAGIC)malloc(sizeof(struct magic));
    if (!m)
        return NULL;

    m->input_size = 0;
    m->output_size = 0;
    m->modifications = NULL;

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

    struct modification *new_mod = (struct modification *)malloc(sizeof(struct modification));
    if (!new_mod)
        return;

    new_mod->pos = pos;
    new_mod->length = length;
    new_mod->type = -1; // -1 = suppression
    new_mod->next = NULL;
    new_mod->prev = NULL;

    if (!m->modifications){
        m->modifications = new_mod;
    }
    else{
        struct modification *current = m->modifications;
        struct modification *prev = NULL;

        while (current && current->pos < pos){
            prev = current;
            current = current->next;
        }

        if (!prev){
            new_mod->next = m->modifications;
            if (m->modifications)
                m->modifications->prev = new_mod;
            m->modifications = new_mod;
        }
        else{
            new_mod->next = current;
            new_mod->prev = prev;
            prev->next = new_mod;
            if (current)
                current->prev = new_mod;
        }
    }

    // maj arbre
    rb_delete_range(m->rb_tree, pos, length);
    m->output_size -= length;
}


void MAGICadd(MAGIC m, int pos, int length){
    if (!m || length <= 0)
        return;

    struct modification *new_mod = (struct modification *)malloc(sizeof(struct modification));
    if (!new_mod)
        return;

    new_mod->pos = pos;
    new_mod->length = length;
    new_mod->type = 1; // 1 = ajout
    new_mod->next = NULL;
    new_mod->prev = NULL;

    if (!m->modifications){
        m->modifications = new_mod;
    }
    else{
        struct modification *current = m->modifications;
        struct modification *prev = NULL;

        while (current && current->pos < pos){
            prev = current;
            current = current->next;
        }

        if (!prev){
            new_mod->next = m->modifications;
            if (m->modifications)
                m->modifications->prev = new_mod;
            m->modifications = new_mod;
        }
        else{
            new_mod->next = current;
            new_mod->prev = prev;
            prev->next = new_mod;
            if (current)
                current->prev = new_mod;
        }
    }

    // maj arbre
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

    struct modification *current = m->modifications;
    while (current){
        struct modification *next = current->next;
        free(current);
        current = next;
    }

    rb_destroy(m->rb_tree);

    free(m);
}
