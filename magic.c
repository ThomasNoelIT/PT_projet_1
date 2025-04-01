#include "magic.h"

#include <sys/stat.h>
#include <sys/types.h>


#define OUTPUT_DIR "tree_snapshots"


typedef enum{
    RED,
    BLACK
} Color;

typedef struct RedBlackTreeNode {
    int pos; 
    int delta; 
    int lazyShift;
    int timestamp;  // Ajout du timestamp
    struct RedBlackTreeNode *left, *right, *parent;
    Color color;    
} RBNode;

// Structure for the Red-Black Tree
typedef struct RedBlackTree{
    RBNode *NIL; 
    RBNode *root; 
} RBTree;


/*void writeNodeToFile(RBNode *node, FILE *file, RBTree *tree) {
    if (node == tree->NIL) return;
    printf("Écriture du noeud : %d %d %s\n", node->pos, node->delta, node->color ? "RED" : "BLACK");
    fprintf(file, "%d %d %s\n", node->pos, node->delta, node->color ? "RED" : "BLACK");
    writeNodeToFile(node->left, file, tree);
    writeNodeToFile(node->right, file, tree);
}

static int file_counter = 0; // Compteur global pour les fichiers
void sendTreeToFile(RBTree *tree, const char *filename) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%s_%d.txt", OUTPUT_DIR, filename, file_counter++);

    FILE *file = fopen(filepath, "w");  // Ouverture en mode écriture
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    writeNodeToFile(tree->root, file, tree);
    fclose(file);
}*/


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
    tree->NIL->pos = tree->NIL->delta = tree->NIL->lazyShift = tree->NIL->timestamp = 0;
    tree->root = tree->NIL;
    return tree;
}

RBNode *createNode(RBTree *tree, int pos, int delta, int timestamp) {
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    if (!node) return NULL;
    node->pos = pos;
    node->delta = delta;
    node->lazyShift = delta;
    node->timestamp = timestamp;  // Enregistrer le timestamp
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

    // ✅ Mise à jour correcte des lazyShift après rotation
    y->lazyShift += x->lazyShift; //parent
}

void rightRotate(RBTree *tree, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    //printf("1-Le noeud %d a désormais un totalShift de %d \n", y->pos, y->lazyShift);
    //printf("2-Le noeud %d a désormais un totalShift de %d \n", x->pos, x->lazyShift);
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

    // ✅ Mise à jour correcte des lazyShift après rotation
    y->lazyShift = y->delta + (y->left != tree->NIL ? y->left->lazyShift : 0); //parent
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
    //printf("Total Shift actuel %d\n",z->lazyShift);
}

void printTreeHelper(RBNode *node, RBTree *tree, int depth, char prefix) {
    if (node == tree->NIL) return;

    printTreeHelper(node->right, tree, depth + 1, 'R'); // Affiche le sous-arbre droit

    for (int i = 0; i < depth; i++) {
        printf("   "); // Indentation pour afficher la hiérarchie
    }

    printf("%d (%s, %d, %d)\n", node->pos, node->color == RED ? "R" : "B", node->delta, node->lazyShift);

    printTreeHelper(node->left, tree, depth + 1, 'L'); // Affiche le sous-arbre gauche
}

void printTree(RBTree *tree) {
    if (!tree || !tree->root) {
        printf("Arbre vide\n");
        return;
    }

    printf("\n===== ARBRE ROUGE-NOIR =====\n");
    printTreeHelper(tree->root, tree, 0, ' ');
    printf("============================\n");
}

void RBTreeInsert(RBTree *tree, int pos, int delta, int timestamp) {
    RBNode *z = createNode(tree, pos, delta, timestamp);
    if (!z) return;

    RBNode *y = tree->NIL;
    RBNode *x = tree->root;
    while (x != tree->NIL) {
        y = x;
        if (z->pos < x->pos) {
            x->lazyShift += z->delta;
            x = x->left;
        } else {
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
    printTree(tree);
    fixInsert(tree, z);
    printTree(tree);
}

int RBTreeFindMapping(RBTree *tree, int pos, MAGICDirection direction) {
    int shift = 0;
    RBNode *current = tree->root;
    RBNode *candidate = NULL;
    //printf("Noeud racine %d, noeud gauche %d, noeud droit %d, noeud gauche droit %d \n", tree->root->pos, tree->root->left->pos, tree->root->right->pos, tree->root->right->left->pos);
    //printf("Noeud racine totalShift %d, noeud gauche TotalShift %d, noeud droit totalShift %d, noeud gauche droit totalShift%d", tree->root->lazyShift, tree->root->left->lazyShift, tree->root->right->lazyShift, tree->root->right->left->lazyShift);
    //printf("Noeud cherché %d", pos);
    //printf("Noeud candidat %d  et décalage total de %d\n", current->pos, shift);
    if (!direction) {  
        // 🔹 Trouver la position actuelle de l'élément initialement en `pos`
        while (current != tree->NIL) {
            if (pos < current->pos) {
                current = current->left;
                //printf("Noeud candidat %d  et décalage total de %d\n", current->pos, shift);
            } else {
                candidate = current;
                shift += current->lazyShift;
                //printf("Noeud candidat %d  et décalage total de %d\n", candidate->pos, shift);
                current = current->right;
            }
        }

        if (candidate) {
            int newPos = pos + shift;
            return (newPos >= 0) ? newPos : -1;
        } else {
            return pos;
        }

    } else {  
        // 🔹 Trouver l'origine d'un élément actuellement à `pos`
        current = tree->root;
        shift = 0;
        candidate = NULL;
        //printf("Ah\n");
        while (current != tree->NIL) {
            int adjustedPos = current->pos + shift;

            if (pos < adjustedPos) {
                if(current->pos < pos){
                    candidate = current;
                    shift += current->lazyShift;
                }
                // candidate = current;
                // shift += current->lazyShift;
                current = current->left;
                //printf("Noeud candidat %d  et décalage total de %d\n", current->pos, shift);
            } else {
                candidate = current;
                shift += current->lazyShift;
                //printf("Noeud candidat %d  et décalage total de %d\n", candidate->pos, shift);
                current = current->right;
            }
        }

        if (candidate) {
            int originalPos = pos - shift;
            //printf("Position d'origine : %d \n", originalPos);
            return (originalPos >= 0) ? 
            originalPos : -1;
        } else {
            return pos;
        }
    }
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

struct magic{
    RBTree *shiftTree;  // Gestion des décalages
    RBTree *deleteTree; // Gestion des suppressions
    int timestamp;      // Compteur global de timestamp
};

MAGIC MAGICinit(void){
    MAGIC m = (MAGIC)malloc(sizeof(struct magic));
    if (!m)
        return NULL;
    m->shiftTree = RBTreeInit();
    m->deleteTree = RBTreeInit();
    m->timestamp = 0;
    if (!m->shiftTree || !m->deleteTree){
        free(m);
        return NULL;
    }
    return m;
}

void MAGICremove(MAGIC m, int pos, int length){
    if (!m || length <= 0)
        return;

    m->timestamp++;
    RBTreeInsert(m->deleteTree, pos, -length, m->timestamp);
    RBTreeInsert(m->shiftTree, pos + length, -length, m->timestamp);
}

void MAGICadd(MAGIC m, int pos, int length){
    if (!m || length <= 0)
        return;

    m->timestamp++;
    RBTreeInsert(m->shiftTree, pos, length, m->timestamp);
}

int MAGICmap(MAGIC m, MAGICDirection direction, int pos){
    if (!m || pos < 0)
        return -1;

    int shiftedPos = RBTreeFindMapping(m->shiftTree, pos, direction);
    int deleted = RBTreeFindMapping(m->deleteTree, shiftedPos, direction);
    
    return (deleted == -1) ? -1 : shiftedPos;
}

void MAGICdestroy(MAGIC m){
    if (!m)
        return;

    RBTreeDestroy(m->shiftTree);
    RBTreeDestroy(m->deleteTree);
    free(m);
}
