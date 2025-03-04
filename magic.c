#include "magic.h"

MAGIC MAGICinit()
{
    MAGIC m = malloc(sizeof(struct magic));
    if (!m)
        return NULL;
    return m;
}

void MAGICremove(MAGIC m, int pos, int length)
{
    if (!m)
        return;
}

void MAGICadd(MAGIC m, int pos, int length)
{
    if (!m)
        return;
}

int MAGICmap(MAGIC m, MAGICDirection direction, int pos)
{
    if (!m)
        return -1;
    return 0;
}

void MAGICdestroy(MAGIC m)
{
    if (!m)
        return;
    free(m);
}
