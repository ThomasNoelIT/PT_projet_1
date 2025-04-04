#include <stdio.h>
#include "magic.h"

int positions_to_check[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 25, 30, 35, 40, 50, 53, 61, 70, 80, 90, 100, 110};
int num_positions = sizeof(positions_to_check) / sizeof(positions_to_check[0]);


void test_additions(void) {
    MAGIC m = MAGICinit();
    
    // Ajout de bytes à différentes positions avec différentes longueurs
    MAGICadd(m, 0, 5);
    MAGICadd(m, 2, 2);
    MAGICadd(m, 5, 1);
    MAGICadd(m, 8, 2);
    MAGICadd(m, 10, 3);
    MAGICadd(m, 15, 4);
    MAGICadd(m, 20, 6);
    MAGICadd(m, 25, 5);
    MAGICadd(m, 30, 3);
    MAGICadd(m, 35, 2);
    MAGICadd(m, 40, 4);
    MAGICadd(m, 50, 1);
    MAGICadd(m, 53, 3);
    MAGICadd(m, 61, 2);
    MAGICadd(m, 70, 5);
    MAGICadd(m, 80, 4);
    MAGICadd(m, 90, 3);
    MAGICadd(m, 100, 2);
    MAGICadd(m, 110, 1);
    
    for (int i = 0; i < num_positions; i++) {
        int pos = positions_to_check[i];
        printf("IN -> OUT [%d] = %d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));
    }
    
    MAGICdestroy(m);
}


void test_removals(void) {
    MAGIC m = MAGICinit();
    
    // retrait de bytes à différentes positions avec différentes longueurs
    MAGICremove(m, 0, 2);
    MAGICremove(m, 3, 2);
    MAGICremove(m, 6, 1);
    MAGICremove(m, 8, 2);
    MAGICremove(m, 12, 3);
    MAGICremove(m, 18, 4);
    MAGICremove(m, 25, 2);
    MAGICremove(m, 30, 3);
    MAGICremove(m, 35, 1);
    MAGICremove(m, 40, 4);
    MAGICremove(m, 50, 1);
    MAGICremove(m, 55, 3);
    MAGICremove(m, 60, 2);
    MAGICremove(m, 70, 5);
    MAGICremove(m, 80, 4);
    MAGICremove(m, 90, 3);
    MAGICremove(m, 100, 2);
    MAGICremove(m, 110, 1);

    for (int i = 0; i < num_positions; i++) {
        int pos = positions_to_check[i];
        printf("IN -> OUT [%d] = %d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));
    }
    
    MAGICdestroy(m);
}

void test_mapping(void) {
    MAGIC m = MAGICinit();

    printf("==== Test 1: Test de Mapping ====\n");
    for (int i = 0; i < num_positions; i++) {
        int pos = positions_to_check[i];
        printf("IN -> OUT [%d] = %d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));
    }
    

    
    // Ajout de bytes à des positions et longueurs variées
    MAGICadd(m, 0, 5);
    MAGICadd(m, 10, 3);
    MAGICadd(m, 20, 6);
    MAGICadd(m, 30, 2);
    MAGICadd(m, 40, 4);
    
    // Cas 1: Positions au début, au milieu et à la fin des ajouts
    printf("==== Test 2: Position aux limites ====\n");
    printf("IN -> OUT [0] = %d\n", MAGICmap(m, STREAM_IN_OUT, 0));    // Première position ajoutée
    printf("IN -> OUT [4] = %d\n", MAGICmap(m, STREAM_IN_OUT, 4));    // Dernière position du premier segment ajouté
    printf("IN -> OUT [10] = %d\n", MAGICmap(m, STREAM_IN_OUT, 10));  // Première position du deuxième segment
    printf("IN -> OUT [12] = %d\n", MAGICmap(m, STREAM_IN_OUT, 12));  // Milieu du deuxième segment
    printf("IN -> OUT [15] = %d\n", MAGICmap(m, STREAM_IN_OUT, 15));  // Fin du deuxième segment
    printf("IN -> OUT [20] = %d\n", MAGICmap(m, STREAM_IN_OUT, 20));  // Première position du troisième segment
    printf("IN -> OUT [25] = %d\n", MAGICmap(m, STREAM_IN_OUT, 25));  // Milieu du troisième segment
    printf("IN -> OUT [29] = %d\n", MAGICmap(m, STREAM_IN_OUT, 29));  // Fin du troisième segment
    printf("IN -> OUT [40] = %d\n", MAGICmap(m, STREAM_IN_OUT, 40));  // Première position du dernier segment ajouté
    printf("IN -> OUT [43] = %d\n", MAGICmap(m, STREAM_IN_OUT, 43));  // Fin du dernier segment

    // Cas 2: Test de positions avant, au-delà, et entre les segments
    printf("\n==== Test 3: Positions avant et au-delà des ajouts ====\n");
    printf("IN -> OUT [-1] = %d\n", MAGICmap(m, STREAM_IN_OUT, -1));  // Avant le premier ajout
    printf("IN -> OUT [50] = %d\n", MAGICmap(m, STREAM_IN_OUT, 50));  // Au-delà du dernier ajout

    // Cas 3: Test de positions non ajoutées
    printf("\n==== Test 4: Test de positions non ajoutées ====\n");
    printf("IN -> OUT [7] = %d\n", MAGICmap(m, STREAM_IN_OUT, 7));    // Position entre les ajouts (aucun ajout)
    printf("IN -> OUT [17] = %d\n", MAGICmap(m, STREAM_IN_OUT, 17));   // Position entre deux ajouts
    printf("IN -> OUT [22] = %d\n", MAGICmap(m, STREAM_IN_OUT, 22));   // Position entre deux ajouts

    // Cas 4: Test d'une gamme de positions plus larges que celles des positions_to_check
    printf("\n==== Test 5: Test d'une large gamme de positions ====\n");
    for (int i = 0; i < 60; i++) {
        printf("IN -> OUT [%d] = %d\n", i, MAGICmap(m, STREAM_IN_OUT, i));
    }

    // Cas 5: Test de performance (mapping sur un grand nombre de positions)
    printf("\n==== Test 6: Test de performance ====\n");
    for (int i = 0; i < 200; i++) {
        int pos = i * 100; // Tester des positions espacées
        printf("IN -> OUT [%d] = %d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));
    }

    MAGICdestroy(m);
}


void test_mixed_operations_no_overlap(void) {
    MAGIC m = MAGICinit();
    
    // Ajouts sans chevauchement avec les suppressions
    MAGICadd(m, 0, 4);
    MAGICadd(m, 10, 3);
    MAGICadd(m, 20, 5);
    MAGICadd(m, 30, 2);
    MAGICadd(m, 40, 4);
    MAGICadd(m, 50, 3);

    // Suppressions à des positions distinctes
    MAGICremove(m, 5, 2);
    MAGICremove(m, 15, 3);
    MAGICremove(m, 25, 2);
    MAGICremove(m, 35, 2);
    MAGICremove(m, 45, 2);
    MAGICremove(m, 55, 1);


    for (int i = 0; i < num_positions; i++) {
        int pos = positions_to_check[i];
        printf("IN -> OUT [%d] = %d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));
    }

    MAGICdestroy(m);
}

void test_mixed_operations_with_overlap(void) {
    MAGIC m = MAGICinit();
    
    // Ajouts à certaines positions
    MAGICadd(m, 0, 5);
    MAGICadd(m, 5, 3);
    MAGICadd(m, 10, 4);
    MAGICadd(m, 20, 6);
    MAGICadd(m, 30, 3);
    
    // Suppressions qui chevauchent des ajouts
    MAGICremove(m, 3, 4);  // Supprime en plein milieu de MAGICadd(0, 5) et MAGICadd(5, 3)
    MAGICremove(m, 7, 2);  // Supprime en plein milieu de MAGICadd(5, 3) et MAGICadd(10, 4)
    MAGICremove(m, 12, 3); // Supprime une partie de MAGICadd(10, 4)
    MAGICremove(m, 25, 5); // Supprime une partie de MAGICadd(20, 6) et MAGICadd(30, 3)

    for (int i = 0; i < num_positions; i++) {
        int pos = positions_to_check[i];
        printf("IN -> OUT [%d] = %d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));
    }

    MAGICdestroy(m);
}


void test_reverse_mapping(void) {
    MAGIC m = MAGICinit();
    
    // Ajouts de bytes
    MAGICadd(m, 0, 5);
    MAGICadd(m, 5, 5);
    MAGICadd(m, 15, 5);
    
    // Suppressions de certaines plages
    MAGICremove(m, 2, 3);  // Supprime une partie du premier ajout
    MAGICremove(m, 6, 2);  // Supprime une partie du deuxième ajout
    MAGICremove(m, 17, 2); // Supprime une partie du troisième ajout

    // Vérification du reverse mapping sur une large plage
    printf("==== Reverse Mapping (OUT -> IN) ====\n");
    for (int i = 0; i < 20; i++) {
        printf("OUT -> IN [%d] = %d\n", i, MAGICmap(m, STREAM_OUT_IN, i));
    }

    // Cas spéciaux
    printf("\n==== Cas Spéciaux ====\n");
    printf("Reverse map d'un élément supprimé (ex: OUT 3) = %d\n", MAGICmap(m, STREAM_OUT_IN, 3));   // Suppression de [2, 3, 4]
    printf("Reverse map d'un élément jamais ajouté (ex: OUT 12) = %d\n", MAGICmap(m, STREAM_OUT_IN, 12)); // 12 n'a jamais été mappé
    printf("Reverse map d'un élément ajouté en début de mapping (ex: OUT 0) = %d\n", MAGICmap(m, STREAM_OUT_IN, 0)); // Doit exister
    printf("Reverse map d'un élément ajouté en fin de mapping (ex: OUT 19) = %d\n", MAGICmap(m, STREAM_OUT_IN, 19)); // Doit exister

    MAGICdestroy(m);
}


void test_edge_cases(void) {
    MAGIC m = MAGICinit();
    
    printf("==== Test des Cas de Bord ====\n");

    // Suppression dépassant la plage existante
    MAGICremove(m, 0, 10);  // Suppression hors limites au départ

    // Ajout après suppression totale
    MAGICadd(m, 0, 3);
    MAGICadd(m, 2, 2);
    MAGICremove(m, 1, 4);  // Suppression recouvrant plusieurs ajouts

    // Vérification après suppression extrême
    for (int i = 0; i < 5; i++) {
        printf("IN -> OUT [%d] = %d\n", i, MAGICmap(m, STREAM_IN_OUT, i));
    }

    // Suppressions successives sur la même zone
    MAGICadd(m, 5, 5);
    MAGICremove(m, 7, 2);
    MAGICremove(m, 5, 5);  // Suppression complète du segment ajouté

    // Ajouts après suppressions totales
    MAGICadd(m, 10, 2);
    MAGICadd(m, 15, 4);
    MAGICremove(m, 10, 10); // Suppression couvrant une plage non alignée

    // Vérification après ces manipulations extrêmes
    for (int i = 0; i < 20; i++) {
        printf("IN -> OUT [%d] = %d\n", i, MAGICmap(m, STREAM_IN_OUT, i));
    }

    MAGICdestroy(m);
}


void test_large_operations(void) {
    MAGIC m = MAGICinit();
    
    printf("\n==== Test des Opérations Larges ====\n");

    // Ajout de gros segments espacés régulièrement
    for (int i = 0; i < 2000000; i += 20000) {
        MAGICadd(m, i, 10000);
    }

    // Suppression partielle de plusieurs gros segments
    for (int i = 5000; i < 1000000; i += 40000) {
        MAGICremove(m, i, 5000);
    }

    // Suppression massive d’une section entière
    MAGICremove(m, 200000, 50000);

    // Vérifications échantillonnées à différents intervalles
    for (int i = 0; i < 200; i += 10) {
        int pos = i * 10000;
        printf("IN -> OUT [%d] = %d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));
    }

    MAGICdestroy(m);
}

void test_invalid_positions() {
    printf("=== test_invalid_positions ===\n");
    MAGIC m = MAGICinit();

    MAGICadd(m, 0, 10); // [0..10)

    printf("map(-5) = %d\n", MAGICmap(m, STREAM_IN_OUT, -5));
    printf("map(1000000) = %d\n", MAGICmap(m, STREAM_IN_OUT, 1000000));
    printf("reverse map(-10) = %d\n", MAGICmap(m, STREAM_OUT_IN, -10));
    printf("reverse map(1000000) = %d\n", MAGICmap(m, STREAM_OUT_IN, 1000000));

    MAGICdestroy(m);
}

void test_roundtrip_in_out_in() {
    printf("=== test_roundtrip_in_out_in ===\n");
    MAGIC m = MAGICinit();

    MAGICadd(m, 10, 10); // [10..20)
    MAGICadd(m, 50, 5);  // [50..55)

    for (int i = 0; i < 60; i++) {
        int out = MAGICmap(m, STREAM_IN_OUT, i);
        if (out != -1) {
            int back = MAGICmap(m, STREAM_OUT_IN, out);
            printf("in %d -> out %d -> in %d\n", i, out, back);
        } else {
            printf("in %d -> out %d (hors mapping)\n", i, out);
        }
    }

    MAGICdestroy(m);
}

void test_roundtrip_out_in_out() {
    printf("=== test_roundtrip_out_in_out ===\n");
    MAGIC m = MAGICinit();

    MAGICadd(m, 100, 5);   // [100..105)
    MAGICadd(m, 200, 10);  // [200..210)

    for (int out = 0; out < 20; out++) {
        int in = MAGICmap(m, STREAM_OUT_IN, out);
        if (in != -1) {
            int back = MAGICmap(m, STREAM_IN_OUT, in);
            printf("out %d -> in %d -> out %d\n", out, in, back);
        } else {
            printf("out %d -> in %d (hors mapping)\n", out, in);
        }
    }

    MAGICdestroy(m);
}

void test_full_segment_removal() {
    printf("=== test_full_segment_removal ===\n");
    MAGIC m = MAGICinit();

    MAGICadd(m, 30, 10);   // [30..40)

    printf("Mapping avant suppression :\n");
    for (int i = 25; i < 45; i++) {
        printf("map(%d) = %d\n", i, MAGICmap(m, STREAM_IN_OUT, i));
    }

    MAGICremove(m, 30, 10); // Suppression complète du segment

    printf("Mapping après suppression complète :\n");
    for (int i = 25; i < 45; i++) {
        printf("map(%d) = %d\n", i, MAGICmap(m, STREAM_IN_OUT, i));
    }

    MAGICdestroy(m);
}


int main(void) {
    printf("Testing Additions\n");
    test_additions();
    printf("\nTesting Removals\n");
    test_removals();
    printf("\nTesting Mapping\n");
    test_mapping();
    printf("\nTesting Mixed Operations no Overlap\n");
    test_mixed_operations_no_overlap();
    printf("\nTesting Mixed Operations with Overlap\n");
    test_mixed_operations_with_overlap();
    printf("\nTesting Reverse Mapping\n");
    test_reverse_mapping();
    printf("\nTesting Edge Cases\n");
    test_edge_cases();
    printf("\nTesting Large Operations\n");
    test_large_operations();

    printf("\nTesting Invalid Positions\n");
    test_invalid_positions();
    printf("\nTesting Roundtrip IN -> OUT -> IN\n");
    test_roundtrip_in_out_in();
    printf("\nTesting Roundtrip OUT -> IN -> OUT\n");
    test_roundtrip_out_in_out();
    printf("\nTesting Full Segment Removal\n");
    test_full_segment_removal();
    
    
    return 0;
}

/*gcc -Wall -pedantic -std=c11 -O3 -o test_final test_final.c magic.c
./test_final   */