#include <stdio.h>
#include <assert.h>

#include "magic.h"

void test_addition(void) {
    MAGIC m = MAGICinit();
    MAGICadd(m, 5, 3); // Ajoute 3 bytes à la position 5
    assert(MAGICmap(m, STREAM_IN_OUT, 5) == 8);
    assert(MAGICmap(m, STREAM_IN_OUT, 6) == 9);
    assert(MAGICmap(m, STREAM_IN_OUT, 7) == 10);
    MAGICdestroy(m);
}

void test_suppression(void) {
    MAGIC m = MAGICinit();
    //MAGICadd(m, 5, 5);
    MAGICremove(m, 6, 3); // Supprime les bytes aux positions 6, 7, 8
    assert(MAGICmap(m, STREAM_IN_OUT, 5) == 5);
    // assert(MAGICmap(m, STREAM_IN_OUT, 6) == -1);
    MAGICdestroy(m);
}

void test_mapping(void) {
    MAGIC m = MAGICinit();
    MAGICadd(m, 3, 2);
    MAGICadd(m, 7, 4);
    assert(MAGICmap(m, STREAM_IN_OUT, 3) == 5);
    assert(MAGICmap(m, STREAM_IN_OUT, 4) == 6);
    assert(MAGICmap(m, STREAM_IN_OUT, 5) == 7);
    assert(MAGICmap(m, STREAM_IN_OUT, 7) == 13);
    MAGICdestroy(m);
}

void test_large_operations(void) {
    MAGIC m = MAGICinit();
    for (int i = 0; i < 1000; i += 10) {
        MAGICadd(m, i, 5);
    }
    for (int i = 5; i < 1000; i += 20) {
        MAGICremove(m, i, 3);
    }
    assert(MAGICmap(m, STREAM_IN_OUT, 50) > 50);
    MAGICdestroy(m);
}

int main(void) {
    test_addition();
    test_suppression();
    test_mapping();
    test_large_operations();
    printf("Tous les tests ont réussi !\n");
    return 0;
}
