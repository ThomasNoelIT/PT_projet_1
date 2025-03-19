#include <stdio.h>
#include "magic.h"

int main(void) {
    MAGIC m = MAGICinit();
    if (!m) {
        printf("Failed to initialize MAGIC.\n");
        return 1;
    }

    // Ajout de modifications
    MAGICadd(m, 10, 5);    // Ajout de 5 octets à la position 10
    MAGICadd(m, 20, 3);    // Ajout de 3 octets à la position 20

    // Suppression de modifications
    MAGICremove(m, 15, 2); // Suppression de 2 octets à la position 15

    // Test de mappage
    int newPos = MAGICmap(m, STREAM_IN_OUT, 18);
    printf("Position 18 (IN->OUT) est mappée à : %d\n", newPos);

    newPos = MAGICmap(m, STREAM_OUT_IN, 22);
    printf("Position 22 (OUT->IN) est mappée à : %d\n", newPos);

    // Libération de la mémoire
    MAGICdestroy(m);

    return 0;
}


/*
gcc -Wall -pedantic -std=c11 -O3 -o test_magic test_magic.c magic.c
./test_magic
*/
