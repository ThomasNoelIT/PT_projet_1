#include <stdio.h>
#include <assert.h>
#include "magic.h"

/*
 * Test Plan for MAGIC Byte Stream Mapping System
 *
 * Each function tests a specific behavior of the MAGIC system:
 * - Insertion and deletion of byte ranges
 * - Mapping of logical to physical byte positions
 * - Boundary and error case handling
 */

// Tests basic mapping and how it changes after insertion
void test_mapping(void) {
    // Initialize a new MAGIC mapping object
    MAGIC m = MAGICinit();

    // Verify identity mapping for untouched positions
    assert(MAGICmap(m, STREAM_IN_OUT, 3) == 3);
    assert(MAGICmap(m, STREAM_IN_OUT, 4) == 4);
    assert(MAGICmap(m, STREAM_IN_OUT, 5) == 5);
    assert(MAGICmap(m, STREAM_IN_OUT, 7) == 7);

    // Insert 2 bytes at position 3
    MAGICadd(m, 3, 2);

    // Mapping after insertion: positions ≥ 3 should be shifted by 2
    assert(MAGICmap(m, STREAM_IN_OUT, 3) == 5);
    assert(MAGICmap(m, STREAM_IN_OUT, 4) == 6);
    assert(MAGICmap(m, STREAM_IN_OUT, 5) == 7);
    assert(MAGICmap(m, STREAM_IN_OUT, 7) == 9);

    // Clean up
    MAGICdestroy(m);
}

// Tests a basic insertion of bytes
void test_addition(void) {
    MAGIC m = MAGICinit();

    // Insert 3 bytes at position 5
    MAGICadd(m, 5, 3);

    // Expect positions 5, 6, 7 to be shifted to 8, 9, 10 respectively
    assert(MAGICmap(m, STREAM_IN_OUT, 5) == 8);
    assert(MAGICmap(m, STREAM_IN_OUT, 6) == 9);
    assert(MAGICmap(m, STREAM_IN_OUT, 7) == 10);

    MAGICdestroy(m);
}

// Tests deletion/removal of bytes
void test_suppression(void) {
    MAGIC m = MAGICinit();

    // Remove 3 bytes starting at position 6 (removes 6, 7, 8)
    MAGICremove(m, 6, 3);

    // Position 5 remains unaffected
    assert(MAGICmap(m, STREAM_IN_OUT, 5) == 5);

    // Position 6 was deleted, should map to -1 (invalid)
    assert(MAGICmap(m, STREAM_IN_OUT, 6) == -1);

    MAGICdestroy(m);
}

// Stress test: multiple additions and removals over a large range
void test_large_operations(void) {
    MAGIC m = MAGICinit();

    // Add 5 bytes every 10 positions (from 0 to 990)
    for (int i = 0; i < 1000; i += 10) {
        MAGICadd(m, i, 5);
    }

    // Remove 3 bytes every 20 positions starting at 5
    for (int i = 5; i < 1000; i += 20) {
        MAGICremove(m, i, 3);
    }

    // Verify that mapping is still working and correctly shifted
    assert(MAGICmap(m, STREAM_IN_OUT, 50) > 50);

    MAGICdestroy(m);
}

// Tests robustness against invalid operations
void test_invalid_operations(void) {
    MAGIC m = MAGICinit();

    // Mapping a negative index should return -1
    assert(MAGICmap(m, STREAM_IN_OUT, -1) == -1);

    // Adding at a negative position should be ignored or handled safely
    MAGICadd(m, -5, 3);

    // Removing from a very large (out of bounds) position should not crash
    MAGICremove(m, 1000000, 5);

    MAGICdestroy(m);
}

// Entry point: run all test cases
int main(void) {
    printf("Running tests...\n");
    test_mapping();
    test_addition();
    test_suppression();
    test_large_operations();
    test_invalid_operations();
    printf("Tous les tests ont réussi !\n"); // French: "All tests passed!"
    return 0;
}

/*
 * Compilation:
 *   gcc -Wall -pedantic -std=c11 -O3 -o magic_test_plan magic_test_plan.c magic.c
 *
 * Execution:
 *   ./magic_test_plan
 */
