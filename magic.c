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
    tree->NIL->pos = tree->NIL->delta = tree->NIL->lazyShift = 0;
    tree->root = tree->NIL;
    return tree;
}

RBNode *createNode(RBTree *tree, int pos, int delta) {
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    if (!node) return NULL;
    node->pos = pos;
    node->delta = delta;
    node->lazyShift = delta;
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
    // x->lazyShift = x->delta + (x->left != tree->NIL ? x->left->lazyShift : 0);
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

void RBTreeInsert(RBTree *tree, int pos, int delta) {
    RBNode *z = createNode(tree, pos, delta);
    if (!z) return;

    RBNode *y = tree->NIL;
    RBNode *x = tree->root;
    while (x != tree->NIL) {
        y = x;
        if (z->pos < x->pos) {
            x->lazyShift += z->delta; // ✅ Mise à jour correcte
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
    //printTree(tree);
    fixInsert(tree, z);
    //printTree(tree);
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
                // printf("Ah\n");
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







/* #include "magic.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h> 

#define OUTPUT_DIR "tree_snapshots"

typedef enum {
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

typedef struct RedBlackTree {
    RBNode *NIL;
    RBNode *root;
} RBTree;

void writeNodeToFile(RBNode *node, FILE *file, RBTree *tree) {
    if (node == tree->NIL) return;
    fprintf(file, "%d %d %d %s\n", node->pos, node->delta, node->lazyShift, node->color == RED ? "RED" : "BLACK");
    writeNodeToFile(node->left, file, tree);
    writeNodeToFile(node->right, file, tree);
}

static int file_counter = 0;
void sendTreeToFile(RBTree *tree, const char *filename) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%s_%d.txt", OUTPUT_DIR, filename, file_counter++);
    
    FILE *file = fopen(filepath, "w");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return;
    }
    writeNodeToFile(tree->root, file, tree);
    fclose(file);
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
    node->lazyShift = delta;
    node->left = node->right = node->parent = tree->NIL;
    node->color = RED;
    return node;
}

void leftRotate(RBTree *tree, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != tree->NIL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->NIL) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;

    x->lazyShift = x->delta +
                   (x->left != tree->NIL ? x->left->lazyShift : 0);
    y->lazyShift = y->delta +
                   (y->left != tree->NIL ? y->left->lazyShift : 0);
}


void rightRotate(RBTree *tree, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != tree->NIL) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == tree->NIL) tree->root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;
    x->right = y;
    y->parent = x;

    y->lazyShift = y->delta +
                   (y->left != tree->NIL ? y->left->lazyShift : 0) +
                   (y->right != tree->NIL ? y->right->lazyShift : 0);
    x->lazyShift = x->delta +
                   (x->left != tree->NIL ? x->left->lazyShift : 0) +
                   (x->right != tree->NIL ? x->right->lazyShift : 0);
}










int getBlackHeight(RBNode *node, RBNode *NIL) {
    if (node == NIL) return 1; // NIL compte pour 1 dans la black-height
    int leftHeight = getBlackHeight(node->left, NIL);
    int rightHeight = getBlackHeight(node->right, NIL);
    
    if (leftHeight == 0 || rightHeight == 0 || leftHeight != rightHeight) return 0;
    
    return leftHeight + (node->color == BLACK ? 1 : 0);
}

bool isRedBlackTreeValid(RBNode *node, RBNode *NIL) {
    if (node == NIL) return true; // Un sous-arbre vide est valide
    
    // Vérifier que la racine est noire (doit être fait une seule fois)
    if (node->parent == NIL && node->color != BLACK) return false;

    // Vérifier la règle des nœuds rouges
    if (node->color == RED) {
        if (node->left->color == RED || node->right->color == RED) return false;
    }
    
    // Vérifier que tous les chemins ont la même black-height
    if (getBlackHeight(node, NIL) == 0) return false;

    // Vérifier récursivement les sous-arbres
    return isRedBlackTreeValid(node->left, NIL) && isRedBlackTreeValid(node->right, NIL);
}







void fixInsert(RBTree *tree, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;
            if (y->color == RED) {  // Cas 1: Recoloration
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {  
                if (z == z->parent->right) {  // Cas 2: Rotation gauche nécessaire
                    z = z->parent;
                    leftRotate(tree, z);
                }
                // Cas 3: Rotation droite + recoloration
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {  
            RBNode *y = z->parent->parent->left;
            if (y->color == RED) {  // Cas 1: Recoloration
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {  
                if (z == z->parent->left) {  // Cas 2: Rotation droite nécessaire
                    z = z->parent;
                    rightRotate(tree, z);
                }
                // Cas 3: Rotation gauche + recoloration
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    // Assurer que la racine est toujours noire
    tree->root->color = BLACK;

    printf("couleur de la racine %s\n", tree->root->color == RED ? "RED" : "BLACK");

    printTree(tree);
    
    // printf("vérification de l'arbre rouge-noir après insertion\n");
    // Vérification des propriétés de l'arbre rouge-noir
    if (!isRedBlackTreeValid(tree->root, tree->NIL)) {
        printf("L'arbre rouge-noir n'est pas valide après l'insertion.\n");
        fixInsert(tree, tree->root); // Correction récursive si nécessaire
    }
}






void RBTreeInsert(RBTree *tree, int pos, int delta) {
    RBNode *z = createNode(tree, pos, delta);
    if (!z) return;
    
    RBNode *y = tree->NIL;
    RBNode *x = tree->root;

    while (x != tree->NIL) {
        y = x;
        x = (z->pos < x->pos) ? x->left : x->right;
    }

    z->parent = y;
    if (y == tree->NIL) tree->root = z;
    else if (z->pos < y->pos) y->left = z;
    else y->right = z;
    
    fixInsert(tree, z);
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
        shift = 0;
        current = tree->root;
        candidate = NULL;

        while (current != tree->NIL) {
            int adjustedPos = current->pos + shift;

            if (pos < adjustedPos) {
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
}*/










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
    //RBTreeInsert(m->rb_tree_out_in, pos, -length);

    //printf("\nÉtat de l'arbre après retrait in_out:\n");
    //sendTreeToFile(m->rb_tree_in_out, "tree_in_out.txt");
}

void MAGICadd(MAGIC m, int pos, int length){
    if (!m || length <= 0)
        return;

    RBTreeInsert(m->rb_tree_in_out, pos, length);
    //RBTreeInsert(m->rb_tree_out_in, pos, length);


    //printf("\nÉtat de l'arbre après ajout in_out:\n");
    //sendTreeToFile(m->rb_tree_in_out, "tree_in_out.txt");

    //printTree(m->rb_tree_in_out);

}

int MAGICmap(MAGIC m, MAGICDirection direction, int pos){
    if (!m || pos < 0)
        return -1;

    if (direction == STREAM_IN_OUT)
        return RBTreeFindMapping(m->rb_tree_in_out, pos, direction);
    else
        return RBTreeFindMapping(m->rb_tree_in_out, pos, direction);
}

void MAGICdestroy(MAGIC m){
    if (!m)
        return;

    RBTreeDestroy(m->rb_tree_in_out);
    //RBTreeDestroy(m->rb_tree_out_in);
    free(m);
}
