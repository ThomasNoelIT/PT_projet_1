#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
    int color;  // 0 = noir, 1 = rouge
} Node;

// Créer un nouveau nœud
Node* createNode(int value, int color) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->left = node->right = NULL;
    node->color = color;
    return node;
}

// Initialisation SDL
int initSDL(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return 0;
    }
    *window = SDL_CreateWindow("Arbre Rouge-Noir", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("Erreur fenêtre: %s\n", SDL_GetError());
        return 0;
    }
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("Erreur renderer: %s\n", SDL_GetError());
        return 0;
    }
    return 1;
}

// Dessiner un cercle
void drawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; 
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

// Dessiner une ligne
void drawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

// Afficher le texte
void drawText(SDL_Renderer* renderer, TTF_Font* font, int x, int y, char* text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {x - surface->w / 2, y - surface->h / 2, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Dessiner l'arbre récursivement
void drawTree(SDL_Renderer* renderer, TTF_Font* font, Node* root, int x, int y, int offset, int depth) {
    if (!root) return;

    int childY = y + 80;

    // Dessiner les liens
    if (root->left) {
        drawLine(renderer, x, y + 20, x - offset, childY);
        drawTree(renderer, font, root->left, x - offset, childY, offset / 2, depth + 1);
    }
    if (root->right) {
        drawLine(renderer, x, y + 20, x + offset, childY);
        drawTree(renderer, font, root->right, x + offset, childY, offset / 2, depth + 1);
    }

    // Définir la couleur du nœud (rouge ou noir)
    SDL_Color textColor = {255, 255, 255};
    if (root->color == 0)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Noir
    else
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Rouge

    drawCircle(renderer, x, y, 20);

    // Afficher la valeur du nœud
    char valueText[10];
    sprintf(valueText, "%d", root->value);
    drawText(renderer, font, x, y, valueText, textColor);
}

// Fonction principale
int main() {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (!initSDL(&window, &renderer)) return -1;
    if (TTF_Init() == -1) {
        printf("Erreur TTF: %s\n", TTF_GetError());
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 16);
    if (!font) {
        printf("Erreur chargement police: %s\n", TTF_GetError());
        return -1;
    }

    // Créer un arbre de test
    Node* root = createNode(10, 0);
    root->left = createNode(5, 1);
    root->right = createNode(15, 1);
    root->left->left = createNode(2, 0);
    root->left->right = createNode(7, 0);
    root->right->right = createNode(20, 0);

    int quit = 0;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = 1;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        drawTree(renderer, font, root, WIDTH / 2, 50, WIDTH / 4, 0);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Libération mémoire
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
