#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {
    RED,
    BLACK
} Color;

typedef struct RedBlackTreeNode {
    int key;
    Color color;
    struct RedBlackTreeNode *parent, *left, *right;
    int x, y;  // Coordonnées pour le dessin
} RedBlackTreeNode;

typedef struct RedBlackTree {
    RedBlackTreeNode *NIL;
    RedBlackTreeNode *root;
} RedBlackTree;

// Déclaration des fonctions de dessin
void draw_tree(SDL_Renderer *renderer, RedBlackTreeNode *node, int x, int y, int offset);
void draw_node(SDL_Renderer *renderer, RedBlackTreeNode *node);
void SDL_RenderFillEllipse(SDL_Renderer *renderer, int x, int y, int w, int h);
void draw_text(SDL_Renderer *renderer, const char *text, int x, int y);
RedBlackTreeNode* rb_insert_node(RedBlackTree *tree, int key);

RedBlackTreeNode *rb_init_node(int key, Color color) {
    RedBlackTreeNode *node = (RedBlackTreeNode *)malloc(sizeof(RedBlackTreeNode));
    node->key = key;
    node->color = color;
    node->left = node->right = node->parent = NULL;
    node->x = node->y = 0;
    return node;
}

RedBlackTree *rb_init() {
    RedBlackTree *tree = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    tree->NIL = rb_init_node(-1, BLACK);
    tree->root = tree->NIL;
    return tree;
}

void rb_insert(RedBlackTree *tree, int key) {
    RedBlackTreeNode *new_node = rb_insert_node(tree, key);
    // Ajoutez ici la logique pour ajuster l'arbre en fonction des règles des arbres rouges-noirs.
    // Par exemple, vous devez assurer que l'arbre est équilibré après l'insertion.
}

RedBlackTreeNode* rb_insert_node(RedBlackTree *tree, int key) {
    RedBlackTreeNode *new_node = rb_init_node(key, RED);
    RedBlackTreeNode *parent = tree->root;
    RedBlackTreeNode *x = tree->root;

    while (x != tree->NIL) {
        parent = x;
        if (key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    new_node->parent = parent;

    if (parent == tree->root || key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }

    new_node->left = new_node->right = tree->NIL;
    return new_node;
}

void draw_tree(SDL_Renderer *renderer, RedBlackTreeNode *node, int x, int y, int offset) {
    if (node == NULL || node->key == -1) return;

    node->x = x;
    node->y = y;

    // Dessiner le nœud actuel
    draw_node(renderer, node);

    // Dessiner les branches et les nœuds enfants
    if (node->left != NULL && node->left->key != -1) {
        SDL_RenderDrawLine(renderer, x, y, node->left->x, node->left->y);
        draw_tree(renderer, node->left, x - offset, y + 50, offset / 2);
    }
    if (node->right != NULL && node->right->key != -1) {
        SDL_RenderDrawLine(renderer, x, y, node->right->x, node->right->y);
        draw_tree(renderer, node->right, x + offset, y + 50, offset / 2);
    }
}

void draw_node(SDL_Renderer *renderer, RedBlackTreeNode *node) {
    // Définir la couleur en fonction du nœud (rouge ou noir)
    if (node->color == RED) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Rouge
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);    // Noir
    }

    // Dessiner un cercle
    SDL_RenderFillEllipse(renderer, node->x - 15, node->y - 15, 30, 30);

    // Afficher la clé
    char key_str[10];
    snprintf(key_str, 10, "%d", node->key);
    draw_text(renderer, key_str, node->x - 10, node->y - 10);  // Fonction de texte
}

void SDL_RenderFillEllipse(SDL_Renderer *renderer, int x, int y, int w, int h) {
    int rx = w / 2;
    int ry = h / 2;
    int cx = x + rx;
    int cy = y + ry;
    int dx = 0;
    int dy = ry;
    int d = 1 - ry;

    while (dx <= rx) {
        SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
        SDL_RenderDrawLine(renderer, cx - dx, cy - dy, cx + dx, cy - dy);
        SDL_RenderDrawLine(renderer, cx - dy, cy + dx, cx + dy, cy + dx);
        SDL_RenderDrawLine(renderer, cx - dy, cy - dx, cx + dy, cy - dx);
        if (d <= 0) {
            d += 2 * dx + 3;
        } else {
            d += 2 * (dx - dy) + 5;
            dy--;
        }
        dx++;
    }
}

void draw_text(SDL_Renderer *renderer, const char *text, int x, int y) {
    // Initialiser la couleur du texte (blanc)
    SDL_Color color = {255, 255, 255};  // Blanc
    // Créer la surface de texte
    SDL_Surface *textSurface = TTF_RenderText_Solid(TTF_OpenFont("arial.ttf", 24), text, color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Arbre Rouge-Noir", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    RedBlackTree *tree = rb_init();
    rb_insert(tree, 10);
    rb_insert(tree, 5);
    rb_insert(tree, 15);

    SDL_RenderClear(renderer);
    draw_tree(renderer, tree->root, 400, 50, 100);
    SDL_RenderPresent(renderer);

    // Attendre que l'utilisateur appuie sur une touche pour fermer
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN)) {
                quit = 1;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
