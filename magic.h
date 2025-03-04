#ifndef MAGIC_H
#define MAGIC_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Opaque data structure for modification.
 */
struct modification {
    int pos;
    int length;
    int type;
    struct modification *next;
};

/**
 * Opaque data structure for MAGIC ADT.
 */
typedef struct magic {
    int input_size;
    int output_size;

    // Arbre
    
    struct modification *modifications;

    // table de hachage 

} *MAGIC;

/**
 * Enumeration to define the mapping direction.
 */
typedef enum {
    STREAM_IN_OUT = 0,
    STREAM_OUT_IN = 1
} MAGICDirection;

/**
 * Initializes the MAGIC ADT.
 * @return A pointer to the initialized MAGIC instance.
 */
MAGIC MAGICinit();

/**
 * Removes a segment of bytes from the stream.
 * @param m The MAGIC instance.
 * @param pos The starting position of the removal.
 * @param length The number of bytes to remove.
 */
void MAGICremove(MAGIC m, int pos, int length);

/**
 * Adds a segment of bytes to the stream.
 * @param m The MAGIC instance.
 * @param pos The position where bytes should be added.
 * @param length The number of bytes to add.
 */
void MAGICadd(MAGIC m, int pos, int length);

/**
 * Maps a byte position from input to output or vice versa.
 * @param m The MAGIC instance.
 * @param direction The mapping direction.
 * @param pos The byte position to query.
 * @return The corresponding position in the mapped stream, or -1 if not found.
 */
int MAGICmap(MAGIC m, MAGICDirection direction, int pos);

/**
 * Destroys the MAGIC instance and frees memory.
 * @param m The MAGIC instance to destroy.
 */
void MAGICdestroy(MAGIC m);

#endif // MAGIC_H
