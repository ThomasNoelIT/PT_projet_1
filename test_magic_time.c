#include <stdio.h>
#include <time.h>
#include "magic.h"

void test_mapping(MAGIC m) {
    // Test de mappage avec des positions existantes dans le flux
    int pos = 18;
    clock_t start_time = clock();  // Temps de départ
    int newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    clock_t end_time = clock();    // Temps de fin
    printf("Position %d (IN->OUT) est mappée à : %d\n", pos, newPos);
    printf("Temps de mappage (IN->OUT) : %.6f secondes\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    pos = 22;
    start_time = clock();  // Temps de départ
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    end_time = clock();    // Temps de fin
    printf("Position %d (OUT->IN) est mappée à : %d\n", pos, newPos);
    printf("Temps de mappage (OUT->IN) : %.6f secondes\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    // Test de mappage pour un octet supprimé
    pos = 4;
    start_time = clock();  // Temps de départ
    newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    end_time = clock();    // Temps de fin
    printf("Position %d (IN->OUT) après suppression, mappée à : %d\n", pos, newPos);
    printf("Temps de mappage (IN->OUT après suppression) : %.6f secondes\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    // Test de mappage pour une position supprimée
    pos = 15;
    start_time = clock();  // Temps de départ
    newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    end_time = clock();    // Temps de fin
    printf("Position %d (IN->OUT) supprimée, mappée à : %d\n", pos, newPos);
    printf("Temps de mappage (IN->OUT position supprimée) : %.6f secondes\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    // Test de mappage pour une position ajoutée
    pos = 10;
    start_time = clock();  // Temps de départ
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    end_time = clock();    // Temps de fin
    printf("Position %d (OUT->IN) ajoutée, mappée à : %d\n", pos, newPos);
    printf("Temps de mappage (OUT->IN position ajoutée) : %.6f secondes\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
}


int main(void) {
    MAGIC m = MAGICinit();
    if (!m) {
        printf("Failed to initialize MAGIC.\n");
        return 1;
    }

    // Ajout de modifications
    clock_t start_time = clock();  // Temps de départ
    printf("\nAjout de 5 octets à la position 10\n");
    MAGICadd(m, 10, 5);    // Ajout de 5 octets à la position 10
    clock_t end_time = clock();    // Temps de fin
    printf("Temps d'ajout (10, 5) : %.6f secondes\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    start_time = clock();  // Temps de départ
    printf("Ajout de 3 octets à la position 20\n");
    MAGICadd(m, 20, 3);    // Ajout de 3 octets à la position 20
    end_time = clock();    // Temps de fin
    printf("Temps d'ajout (20, 3) : %.6f secondes\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    // Suppression de modifications
    start_time = clock();  // Temps de départ
    printf("\nSuppression de 2 octets à la position 15\n");
    MAGICremove(m, 15, 2); // Suppression de 2 octets à la position 15
    end_time = clock();    // Temps de fin
    printf("Temps de suppression (15, 2) : %.6f secondes\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    // Test de mappage
    printf("\nTest de mappage\n");
    test_mapping(m);

    // Libération de la mémoire
    MAGICdestroy(m);

    return 0;
}

/*
gcc -Wall -pedantic -std=c11 -O3 -o test_magic_time test_magic_time.c magic.c
./test_magic_time
time ./test_magic_time
*/
