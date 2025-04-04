#include <stdio.h>
#include "magic.h"

void test_mapping(MAGIC m) {
    // Test de mappage avec des positions existantes dans le flux
    int pos = 5;
    int newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) est mappee a : %d\n", pos, newPos);

    // pos = 2;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (OUT->IN) est mappee a : %d\n", pos, newPos);

    //Test de mappage pour un octet supprimé
    pos = 8;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // Test de mappage pour un octet ajouté
    pos = 10;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    pos = 15;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    pos = 20;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // Test de mappage pour un octet ajouté
    pos = 25;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    pos = 30;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    pos = 35;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // Test de mappage pour un octet ajouté
    pos = 40;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    pos = 50;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);
    pos = 53;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    pos = 61;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    pos = 70;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // Test de mappage pour un octet ajouté
    pos = 80;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    pos = 90;
    newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);
}

int main(void) {
    MAGIC m = MAGICinit();
    if (!m) {
        printf("Failed to initialize MAGIC.\n");
        return 1;
    }

    // // Ajout de modifications
    // printf("\nAjout de 5 octets a la position 10\n");
    // MAGICadd(m, 10, 5);    // Ajout de 5 octets à la position 10
    // printf("Ajout de 3 octets a la position 20\n");
    // MAGICadd(m, 20, 3);    // Ajout de 3 octets à la position 20

    // // Suppression de modifications
    // printf("\nSuppression de 2 octets a la position 15\n");
    // MAGICremove(m, 15, 2); // Suppression de 2 octets à la position 15

    // // Test de mappage
    // printf("\nTest de mappage\n");
    // test_mapping(m);

    // // Cas avec des positions hors limites
    // printf("\nTest de positions hors limites\n");
    // int pos_hors_limite = 1000;
    // int newPos = MAGICmap(m, STREAM_IN_OUT, pos_hors_limite);
    // printf("Position %d (IN->OUT) est mappee a : %d\n", pos_hors_limite, newPos);

    // // Test de suppression d'octets à la fin du flux
    // printf("\nSuppression d'octets a la fin du flux\n");
    // printf("\nSuppression de 2 octets a la position 25\n");
    // MAGICremove(m, 25, 2); // Suppression à la fin
    // test_mapping(m);

    // // Test de l'ajout à une position spécifique dans le flux
    // printf("\nAjout d'octets a la position 5\n");
    // MAGICadd(m, 5, 4);
    // test_mapping(m);

    // Mixte overlap
    // printf("Ajout de 2 octets à la position 4 \n");
    // MAGICadd(m, 4, 2);
    // int pos = 3;
    // int newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) est mappee a : %d\n", pos, newPos);
    // pos = 4;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 5;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    // pos = 6;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // printf("Suppression de 2 octet à la position 6\n");
    // MAGICremove(m, 6, 2);

    // pos = 4;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) est mappee a : %d\n", pos, newPos);

    // pos = 5;
    // newPos = MAGICmap(m,STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 6;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 7;
    // newPos = MAGICmap(m,STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // //remove test
    // MAGICremove(m, 3, 2);
    // printf("Suppression de 2 octet à la position 3\n");
    // int pos = 0;
    // int newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) est mappee a : %d\n", pos, newPos);
    // pos = 1;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // // Test de mappage pour un octet ajouté
    // pos = 2;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    // pos = 3;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 4;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    // pos = 5;
    // newPos = MAGICmap(m,STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 6;
    // newPos = MAGICmap(m,STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 7;
    // newPos = MAGICmap(m,STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // printf("Suppression de 1 octet à la position 6\n");
    // MAGICremove(m, 6, 1);
    // pos = 5;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) est mappee a : %d\n", pos, newPos);
    // pos = 6;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // // Test de mappage pour un octet ajouté
    // pos = 7;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    // pos = 8;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 9;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    // pos = 10;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 11;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);


    // MAGICremove(m, 8, 2);
    // printf("Suppression de 2 octet à la position 8\n");

    // pos = 5;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 6;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après ajout, mappee a : %d\n", pos, newPos);

    // pos = 7;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 8;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) est mappee a : %d\n", pos, newPos);
    // pos = 9;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 10;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 11;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 12;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 13;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // MAGICremove(m, 6, 2);
    // printf("Suppression de 2 octet à la position 6\n");
    // pos = 5;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 6;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 7;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 8;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 9;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 10;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // pos = 11;
    // newPos = MAGICmap(m, STREAM_OUT_IN, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // Mixte test

    // MAGICadd(m, 0, 4);
    // printf("Ajout de 4 octet à la position 0\n");
    // int pos = 0;
    // int newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) est mappee a : %d\n", pos, newPos);
    // pos = 1;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 2;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 3;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 4;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 5;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // MAGICadd(m, 10, 3);
    // printf("Ajout de 3 octet à la position 10\n");
    // pos = 10;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) est mappee a : %d\n", pos, newPos);
    // pos = 11;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 12;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 13;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 14;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 15;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // MAGICremove(m, 5, 2);
    // printf("Suppression de 2 octet à la position 5\n");
    // pos = 1;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) est mappee a : %d\n", pos, newPos);
    // pos = 2;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 3;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 4;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 5;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 6;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // MAGICremove(m, 15, 3);
    // printf("Suppression de 3 octet à la position 15\n");
    // pos = 13;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) est mappee a : %d\n", pos, newPos);
    // pos = 14;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 15;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 16;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 17;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);
    // pos = 18;
    // newPos = MAGICmap(m, STREAM_IN_OUT, pos);
    // printf("Position %d (IN->OUT) après suppression, mappee a : %d\n", pos, newPos);

    // addition test
    // test_mapping(m);
    // printf("Ajout de 1 octet à la position 5\n");
    // MAGICadd(m, 5, 1);
    // test_mapping(m);
    // printf("Ajout de 2 octets à la position 8\n");
    // MAGICadd(m, 8, 2);
    // test_mapping(m);
    // printf("Ajout de 3 octets à la position 10 \n");
    // MAGICadd(m, 10, 3);
    // test_mapping(m);
    // MAGICadd(m, 15, 4);
    // MAGICadd(m, 20, 6);
    // MAGICadd(m, 25, 5);
    // MAGICadd(m, 30, 3);
    // MAGICadd(m, 35, 2);
    // MAGICadd(m, 40, 4);
    // MAGICadd(m, 50, 1);
    // MAGICadd(m, 53, 3);
    // MAGICadd(m, 61, 2);
    // MAGICadd(m, 70, 5);
    // MAGICadd(m, 80, 4);
    // MAGICadd(m, 90, 3);
    // MAGICadd(m, 100, 2);
    // MAGICadd(m, 110, 1);
    // test_mapping(m);

    //test du prof
    // printf("Ajout de 5 octets à la position 5 \n");
    // MAGICadd(m, 0,5);
    // test_mapping(m);
    // printf("Suppression de 5 octets à la position 5 \n");
    // MAGICremove(m, 0, 5);
    // test_mapping(m);
    // printf("Ajout de 5 octets à la position 20\n");
    // MAGICadd(m,20,5);
    // test_mapping(m);
    // printf("Suppression de 5 octets à la position 20 \n");
    // MAGICremove(m, 20,5);
    // test_mapping(m);
    // printf("Ajout de 5 octets à la position 40 \n");
    // MAGICadd(m,40,5);
    // printf("Suppression de 5 octets à la position 40 \n");
    // MAGICremove(m, 40,5);
    // printf("Ajout de 5 octets à la position 60 \n");
    // MAGICadd(m, 60, 5);
    // printf("Suppression de 5 octets à la position 60 \n");
    // MAGICremove(m, 60, 5);
    // printf("Ajout de 5 octets à la position 80 \n");
    // MAGICadd(m, 80, 5);
    // printf("Suppression de 5 octets à la position 80 \n");
    // MAGICremove(m, 80, 5);
    // test_mapping(m);
    /* // Libération de la mémoire
    MAGICdestroy(m);
    MAGIC m = MAGICinit();
    MAGICadd(m, 10, 5);
    MAGICremove(m, 15, 5);
    printf("MAGICmap(10) = %d\n", MAGICmap(m, STREAM_IN_OUT, 10)); // Devrait retourner -1
    printf("MAGICmap(10) = %d\n", MAGICmap(m, STREAM_OUT_IN, 10));
    printf("MAGICmap(13) = %d\n", MAGICmap(m, STREAM_IN_OUT, 13)); // Devrait être 18
    printf("MAGICmap(10) = %d\n", MAGICmap(m, STREAM_OUT_IN, 10));
    MAGICdestroy(m);
    return 0; */

    return 0;
}

/*
gcc -Wall -pedantic -std=c11 -O3 -o test_magic test_magic.c magic.c
./test_magic
*/
