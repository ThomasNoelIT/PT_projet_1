#include "magic.h"
#include "stdbool.h"
#include "stdlib.h"
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
    //printTree(tree);
    fixInsert(tree, z);
    //printTree(tree);
}



RBNode* findDeleteNode(RBTree *tree, int pos) {
    RBNode *current = tree->root;

    while (current != tree->NIL) {
        // Si pos est dans la plage de suppression (start <= pos <= start + length)
        if (pos >= current->pos && pos < current->pos - current->delta) {
            //printf("Intervalle : [%d; %d] et timestamp : %d\n", current->pos, current->pos - current->delta, current->timestamp);
            return current;
        }
        if (pos < current->pos) {
            current = current->left;
        } else if (pos > current->pos) {
            current = current->right;
        } else {
            return current;  // Nœud trouvé, il est supprimé
        }
    }
    return NULL;  // Aucun nœud trouvé
}



int RBTreeFindMapping(RBTree *sTree,RBTree *dTree,  int pos, MAGICDirection direction) {
    int shift = 0;
    RBNode *current = sTree->root;
    RBNode *candidate = NULL;

    if (!direction) {  // STREAM_IN_OUT (on cherche la position actuelle)
        while (current != sTree->NIL) {
            int adjustedPos = pos + shift;
            if (adjustedPos < current->pos) {
                current = current->left;
            } else {
                candidate = current;
                shift += current->lazyShift;
                current = current->right;
            }
        }
        if (candidate) {
            int newPos = pos + shift;
            
            if(shift > 0){
                // Vérifier si l'origine a été supprimée
                RBNode *deleteNode = findDeleteNode(dTree, newPos);
                if (deleteNode && deleteNode->timestamp >= candidate->timestamp) {
                    return -1;  // La position d'origine a été supprimée
                }
            }
            //printf("La position recherché est %d, La position courante est : %d, La position après décalage : %d, Le décalage est : %d\n", pos,candidate->pos, newPos, shift);
            return (newPos >= candidate->pos) ? newPos : -1;
        } else {
            return pos;
        }

    } else {  // STREAM_OUT_IN (on cherche l'origine avant décalage)
        current = sTree->root;
        shift = 0;
        candidate = NULL;

        while (current != sTree->NIL) {
            int adjustedPos = current->pos + shift;
            //printf("Position recherché : %d, Position courante : %d, décalage : %d, Position ajusté : %d \n", pos, current->pos, shift, adjustedPos);

            if (pos < adjustedPos) {
                if (current->pos <= pos) {
                    candidate = current;
                    shift += current->lazyShift;
                }
                current = current->left;
            } else {
                candidate = current;
                shift += current->lazyShift;
                //printf("1- Position recherché : %d, Position courante : %d, décalage : %d, Position ajusté : %d \n", pos, current->pos, shift, adjustedPos);
                //printf(" Décalage actuel de : %d\n", shift);
                current = current->right;
            }
        }

        if (candidate && pos >= candidate->pos && pos < candidate->pos + shift) {
            //printf("Le décalage est de %d \n", shift);
            return -1;  // pos a été ajouté par MAGICadd et ne possède pas de position d'origine
        }

        if (candidate) {
            int originalPos = pos - shift;
            //printf("%d\n", originalPos);

            // Vérifier si l'origine a été supprimée
            RBNode *deleteNode = findDeleteNode(dTree, originalPos);
            if (deleteNode && deleteNode->timestamp > candidate->timestamp) {
                return -1;  // La position d'origine a été supprimée
            }
            return (originalPos >= 0) ? originalPos : -1;
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

// Variable globale pour le timestamp
static long global_timestamp = 0;

void incrementTimestamp(void) {
    global_timestamp++;
}

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

// Mise à jour du code MAGICremove
void MAGICremove(MAGIC m, int pos, int length) {
    if (!m || length <= 0) return;

    incrementTimestamp();  // Incrémentation du timestamp à chaque modification

    // Insérer dans deleteTree pour marquer la suppression
    RBTreeInsert(m->deleteTree, pos, -length, global_timestamp);
    
    // Ajouter un signal dans le shiftTree pour décaler les éléments plus grands
    RBTreeInsert(m->shiftTree, pos, -length, global_timestamp);
    
    // printf("Suppression dans deleteTree avec timestamp %ld\n", global_timestamp);
}



// Mise à jour du code MAGICadd
void MAGICadd(MAGIC m, int pos, int length) {
    if (!m || length <= 0) return;
    if(pos < 0) return;

    incrementTimestamp();  // Incrémentation du timestamp à chaque modification

    // Insérer dans le shiftTree avec le timestamp actuel
    RBTreeInsert(m->shiftTree, pos, length, global_timestamp);
    // Pas de modification dans deleteTree pour l'ajout

    // Enregistrer l'ajout dans le shiftTree
    // printf("Ajout dans shiftTree avec timestamp %ld\n", global_timestamp);
}

int MAGICmap(MAGIC m, MAGICDirection direction, int pos){
    if (!m || pos < 0)
        return -1;

    int shiftedPos = RBTreeFindMapping(m->shiftTree, m->deleteTree, pos,direction);
    
    return shiftedPos;
}

void MAGICdestroy(MAGIC m){
    if (!m)
        return;

    RBTreeDestroy(m->shiftTree);
    RBTreeDestroy(m->deleteTree);
    free(m);
}
