#include <stdio.h>
#include "magic.h"

void test_mapping(MAGIC m) {
    // Test de mappage avec des positions existantes dans le flux
    int pos = 18;
    int newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    printf("Position %d (IN->OUT) est mappee a : %d\n", pos, newPos);

    pos = 22;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (OUT->IN) est mappee a : %d\n", pos, newPos);

    // Test de mappage pour un octet supprimé
    pos = 4;
    newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // Test de mappage pour un octet ajouté
    pos = 9;
    newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);
}

int main(void) {
    /* MAGIC m = MAGICinit();
    if (!m) {
        printf("Failed to initialize MAGIC.\n");
        return 1;
    }

    // Ajout de modifications
    printf("\nAjout de 5 octets a la position 10\n");
    MAGICadd(m, 10, 5);    // Ajout de 5 octets à la position 10
    printf("Ajout de 3 octets a la position 20\n");
    MAGICadd(m, 20, 3);    // Ajout de 3 octets à la position 20

    // Suppression de modifications
    printf("\nSuppression de 2 octets a la position 15\n");
    MAGICremove(m, 15, 2); // Suppression de 2 octets à la position 15

    // Test de mappage
    printf("\nTest de mappage\n");
    test_mapping(m);

    // Cas avec des positions hors limites
    printf("\nTest de positions hors limites\n");
    int pos_hors_limite = 1000;
    int newPos = MAGICmap(m, STREAM_IN_OUT, pos_hors_limite);
    printf("Position %d (IN->OUT) est mappee a : %d\n", pos_hors_limite, newPos);

    // Test de suppression d'octets à la fin du flux
    printf("\nSuppression d'octets a la fin du flux\n");
    MAGICremove(m, 25, 2); // Suppression à la fin
    test_mapping(m);

    // Test de l'ajout à une position spécifique dans le flux
    printf("\nAjout d'octets a la position 5\n");
    MAGICadd(m, 5, 4);
    test_mapping(m);

    // Libération de la mémoire
    MAGICdestroy(m); */
    MAGIC m = MAGICinit();
    MAGICadd(m, 10, 5);
    MAGICremove(m, 15, 5);
    printf("MAGICmap(10) = %d\n", MAGICmap(m, STREAM_IN_OUT, 10)); // Devrait retourner -1
    printf("MAGICmap(10) = %d\n", MAGICmap(m, STREAM_OUT_IN, 10));
    printf("MAGICmap(13) = %d\n", MAGICmap(m, STREAM_IN_OUT, 13)); // Devrait être 18
    printf("MAGICmap(10) = %d\n", MAGICmap(m, STREAM_OUT_IN, 10));
    MAGICdestroy(m);
    return 0;

    return 0;
}

/*
gcc -Wall -pedantic -std=c11 -O3 -o test_magic test_magic.c magic.c
./test_magic
*/
