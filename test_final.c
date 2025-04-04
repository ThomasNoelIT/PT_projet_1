#include <stdio.h>
#include "magic.h"

// Displays IN -> OUT mappings for a predefined list of positions
void print_in_to_out(MAGIC m) {
    // Positions to check for mapping behavior
    int positions_to_check[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
        15, 20, 25, 30, 35, 40, 50, 53, 61, 70, 80, 90, 100, 110
    };

    int num_positions = sizeof(positions_to_check) / sizeof(positions_to_check[0]);
    for (int i = 0; i < num_positions; i++) {
        int pos = positions_to_check[i];
        printf("IN -> OUT [%3d] = %3d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));
    }
}

// === Basic test case: initial mapping with no transformations ===
void test_mapping(void) {
    MAGIC m = MAGICinit();

    printf("==== Test 1: Initial mapping (no transformations) ====\n");
    print_in_to_out(m);

    // Adding segments to the mapping
    MAGICadd(m, 0, 5);    // Add a segment at position 0 of length 5
    MAGICadd(m, 10, 3);   // Add a segment at position 10 of length 3
    MAGICadd(m, 20, 6);   // Add a segment at position 20 of length 6
    MAGICadd(m, 30, 2);   // Add a segment at position 30 of length 2
    MAGICadd(m, 40, 4);   // Add a segment at position 40 of length 4

    // Check edge positions at the boundaries of added segments
    printf("\n==== Test 2: Edge position mappings ====\n");
    int tests[] = {0, 4, 10, 12, 15, 20, 25, 29, 40, 43};
    for (int i = 0; i < 10; i++) {
        int pos = tests[i];
        printf("IN -> OUT [%3d] = %3d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));
    }

    // Check positions that are outside the added segments
    printf("\n==== Test 3: Mapping outside the added segments ====\n");
    printf("IN -> OUT [-1] = %3d\n", MAGICmap(m, STREAM_IN_OUT, -1)); // Before any mapping
    printf("IN -> OUT [50] = %3d\n", MAGICmap(m, STREAM_IN_OUT, 50)); // After the last segment

    // Check positions in between the added segments (gaps)
    printf("\n==== Test 4: Mapping in between segments ====\n");
    int mids[] = {7, 17, 22};
    for (int i = 0; i < 3; i++) {
        int pos = mids[i];
        printf("IN -> OUT [%3d] = %3d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));
    }

    // Check a larger range of positions from 0 to 59
    printf("\n==== Test 5: Range 0–59 ====\n");
    for (int i = 0; i < 60; i++) {
        printf("IN -> OUT [%3d] = %3d\n", i, MAGICmap(m, STREAM_IN_OUT, i));
    }

    // Check mappings for distant positions (sparse range)
    printf("\n==== Test 6: Spaced-out positions ====\n");
    for (int i = 0; i < 200; i++) {
        int pos = i * 100;
        printf("IN -> OUT [%5d] = %5d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));
    }
    MAGICdestroy(m); // Clean up
}


// Test multiple additions of bytes at various positions and lengths
void test_additions(void) {
    MAGIC m = MAGICinit();
    
    // Add bytes at different positions with varying lengths
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

    // Display the resulting IN -> OUT mapping
    print_in_to_out(m);

    MAGICdestroy(m);
}

// Test multiple removals of bytes at various positions and lengths
void test_removals(void) {
    MAGIC m = MAGICinit();
    
    // Remove bytes at different positions with varying lengths
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

    // Display the resulting IN -> OUT mapping
    print_in_to_out(m);
    
    MAGICdestroy(m);
}

// Test adding and then fully removing a segment
void test_full_segment_removal(void) {
    MAGIC m = MAGICinit();

    // Add a segment of 10 bytes at position 30
    MAGICadd(m, 30, 10);

    // Display the mapping after addition
    print_in_to_out(m);

    // Remove the exact same segment (full segment removal)
    MAGICremove(m, 30, 10);

    // Display the mapping after removal to confirm it's back to initial state
    print_in_to_out(m);

    MAGICdestroy(m);
}

// Test mixed additions and removals with non-overlapping ranges
void test_mixed_operations_no_overlap(void) {
    MAGIC m = MAGICinit();
    
    // Add bytes at various positions (no overlap with removals)
    MAGICadd(m, 0, 4);
    MAGICadd(m, 10, 3);
    MAGICadd(m, 20, 5);
    MAGICadd(m, 30, 2);
    MAGICadd(m, 40, 4);
    MAGICadd(m, 50, 3);

    // Remove bytes at distinct, non-overlapping positions
    MAGICremove(m, 5, 2);
    MAGICremove(m, 15, 3);
    MAGICremove(m, 25, 2);
    MAGICremove(m, 35, 2);
    MAGICremove(m, 45, 2);
    MAGICremove(m, 55, 1);

    // Print final IN -> OUT mapping after mixed operations
    print_in_to_out(m);

    MAGICdestroy(m);
}

// Test mixed additions and removals with overlapping regions
void test_mixed_operations_with_overlap(void) {
    MAGIC m = MAGICinit();
    
    // Add bytes at various positions
    MAGICadd(m, 0, 5);
    MAGICadd(m, 5, 3);
    MAGICadd(m, 10, 4);
    MAGICadd(m, 20, 6);
    MAGICadd(m, 30, 3);
    
    // Remove bytes that overlap with some of the added segments
    MAGICremove(m, 3, 4);   // Overlaps MAGICadd(0, 5) and MAGICadd(5, 3)
    MAGICremove(m, 7, 2);   // Overlaps MAGICadd(5, 3) and MAGICadd(10, 4)
    MAGICremove(m, 12, 3);  // Partially removes MAGICadd(10, 4)
    MAGICremove(m, 25, 5);  // Overlaps MAGICadd(20, 6) and MAGICadd(30, 3)

    // Print final IN -> OUT mapping to verify conflict resolution
    print_in_to_out(m);

    MAGICdestroy(m);
}

// Test reverse mapping functionality (OUT -> IN)
void test_reverse_mapping(void) {
    MAGIC m = MAGICinit();
    
    // Add segments at different positions
    MAGICadd(m, 0, 5);
    MAGICadd(m, 5, 5);
    MAGICadd(m, 15, 5);
    
    // Remove portions of the added segments
    MAGICremove(m, 2, 3);   // Removes [2, 3, 4] from first segment
    MAGICremove(m, 6, 2);   // Removes [6, 7] from second segment
    MAGICremove(m, 17, 2);  // Removes [17, 18] from third segment

    // Print OUT -> IN mapping for a full range
    printf("==== Reverse Mapping (OUT -> IN) ====\n");
    for (int i = 0; i < 20; i++) {
        printf("OUT -> IN [%d] = %d\n", i, MAGICmap(m, STREAM_OUT_IN, i));
    }

    // Special edge cases
    printf("\n==== Special Cases ====\n");
    printf("Reverse map of a removed element (e.g. OUT 3) = %d\n", MAGICmap(m, STREAM_OUT_IN, 3));  // Should return -1
    printf("Reverse map of a never-mapped output (e.g. OUT 12) = %d\n", MAGICmap(m, STREAM_OUT_IN, 12)); // Should return -1
    printf("Reverse map of a valid start (e.g. OUT 0) = %d\n", MAGICmap(m, STREAM_OUT_IN, 0));      // Should be valid
    printf("Reverse map of a valid end (e.g. OUT 19) = %d\n", MAGICmap(m, STREAM_OUT_IN, 19));      // Should be valid or -1 depending on mapping

    MAGICdestroy(m);
}

// Test edge cases including out-of-bounds and overlapping removals
void test_edge_cases(void) {
    MAGIC m = MAGICinit();
    
    printf("==== Test des Cas de Bord ====\n");

    // Suppression dépassant la plage existante
    MAGICremove(m, 0, 10);  // Suppression hors limites au départ

    // Ajout après suppression totale
    MAGICadd(m, 0, 3);
    MAGICadd(m, 2, 2);
    MAGICremove(m, 1, 4);  // Suppression recouvrant plusieurs ajouts

    print_in_to_out(m);

    // Suppressions successives sur la même zone
    MAGICadd(m, 5, 5);
    MAGICremove(m, 7, 2);
    MAGICremove(m, 5, 5);  // Suppression complète du segment ajouté

    // Ajouts après suppressions totales
    MAGICadd(m, 10, 2);
    MAGICadd(m, 15, 4);
    MAGICremove(m, 10, 10); // Suppression couvrant une plage non alignée

    print_in_to_out(m);

    MAGICdestroy(m);
}

// Test large operations with many additions and removals
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

// Test invalid positions (negative and out-of-bounds)
void test_invalid_positions(void) {
    printf("=== test_invalid_positions ===\n");
    MAGIC m = MAGICinit();

    MAGICadd(m, 0, 10);

    printf("map(-5) = %d\n", MAGICmap(m, STREAM_IN_OUT, -5));
    printf("map(1000000) = %d\n", MAGICmap(m, STREAM_IN_OUT, 1000000));
    printf("reverse map(-10) = %d\n", MAGICmap(m, STREAM_OUT_IN, -10));
    printf("reverse map(1000000) = %d\n", MAGICmap(m, STREAM_OUT_IN, 1000000));

    MAGICdestroy(m);
}

int main(void) {
    
    // Basic functional tests
    printf("\n=== Testing Mapping ===\n");
    test_mapping();

    printf("=== Testing Additions ===\n");
    test_additions();

    printf("\n=== Testing Removals ===\n");
    test_removals();

    // Specific scenario: full segment removal
    printf("\n=== Testing Full Segment Removal ===\n");
    test_full_segment_removal();

    // Mixed operation tests
    printf("\n=== Testing Mixed Operations (No Overlap) ===\n");
    test_mixed_operations_no_overlap();

    printf("\n=== Testing Mixed Operations (With Overlap) ===\n");
    test_mixed_operations_with_overlap();

    // Reverse mapping tests
    printf("\n=== Testing Reverse Mapping ===\n");
    test_reverse_mapping();

    // Edge case tests
    printf("\n=== Testing Edge Cases ===\n");
    test_edge_cases();

    // Performance and scalability
    printf("\n=== Testing Large Operations ===\n");
    test_large_operations();

    // Invalid position handling
    printf("\n=== Testing Invalid Positions ===\n");
    test_invalid_positions();

    printf("\nEnd of tests.\n");

    return 0;
}

/*
To compile and run:
gcc -Wall -pedantic -std=c11 -O3 -o test_final test_final.c magic.c
./test_final
*/
