#include <stdio.h>
#include <time.h>
#include "magic.h"

void test_insertion_deletion(MAGIC m) {
    printf("\nTest d'insertion et suppression successives :\n");
    
    // Insertion de valeurs successives
    MAGICadd(m, 10, 5);
    MAGICadd(m, 20, 3);
    MAGICadd(m, 5, 2);
    MAGICadd(m, 15, 4);
    MAGICadd(m, 30, 6);
    MAGICadd(m, 25, 1);
    MAGICadd(m, 35, 7);

    // Suppression successives
    MAGICremove(m, 10, 5);
    MAGICremove(m, 20, 3);
    MAGICremove(m, 5, 2);
    MAGICremove(m, 15, 4);
}

void test_mapping(MAGIC m) {
    // Test mappage avec différentes directions
    printf("\nTest de mappage avec différentes directions :\n");
    int pos = 10;
    
    printf("Test mappage (IN->OUT) pour la position %d : ", pos);
    int newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    printf("Mappée à : %d\n", newPos);

    printf("Test mappage (OUT->IN) pour la position %d : ", pos);
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Mappée à : %d\n", newPos);
}

void test_edge_cases(MAGIC m) {
    // Test des cas de bord
    printf("\nTest des cas de bord :\n");

    // Test d'insertion à la position 0
    printf("Ajout de 5 octets à la position 0\n");
    MAGICadd(m, 0, 5);

    // Test d'insertion à une position très élevée

    // Test de suppression à une position non valide (position 50 qui n'existe pas)
    printf("Suppression de 5 octets à la position 50\n");
    MAGICremove(m, 50, 5);
}

int main(void) {
    MAGIC m = MAGICinit();
    if (!m) {
        printf("Failed to initialize MAGIC.\n");
        return 1;
    }

    // Test d'insertion et suppression successives
    test_insertion_deletion(m);

    // Test de mappage dans les deux directions
    test_mapping(m);

    // Test des cas de bord
    test_edge_cases(m);

    // Libération de la mémoire
    MAGICdestroy(m);

    return 0;
}

/*
gcc -Wall -pedantic -std=c11 -O3 -o test_magic_time test_magic_time.c magic.c
./test_magic_time
time ./test_magic_time
*/
