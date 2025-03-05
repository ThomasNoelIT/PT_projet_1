#include <stdio.h>
#include "magic.h"

int main(void)
{
    MAGIC m = MAGICinit();
    if (!m)
    {
        printf("Échec de l'initialisation de MAGIC.\n");
        return 1;
    }

    // Ajout de modifications
    MAGICadd(m, 4, 2);
    MAGICadd(m, 9, 3);
    MAGICremove(m, 3, 2);
    MAGICremove(m, 4, 3);

    // Test de mapping
    int pos = 5;
    printf("Mapping de l'entrée %d vers sortie : %d\n", pos, MAGICmap(m, STREAM_IN_OUT, pos));

    // Nettoyage
    MAGICdestroy(m);
    return 0;
}
