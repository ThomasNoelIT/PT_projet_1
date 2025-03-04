#ifndef MAGIC_H
#define MAGIC_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Opaque data structure for modification.
 */
typedef struct modification MODIFICATION;

/**
 * Opaque data structure for MAGIC ADT.
 */
typedef struct magic *MAGIC;

/**
 * Opaque data structure for MAGICDirection enumeration.
 */
typedef enum MAGICDirection MAGICDirection;

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

#endif
