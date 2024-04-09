#include <stdio.h>
#include <string.h>
#include "table_symboles.h"

Couple TS[NBMAXSYMB] ;  // la variable Table des Symboles
int NbSymb ; 		        // le nombre courant de couples dans la table

extern void initTS() {
    NbSymb = 0;
}

extern void afficheTS() {
    for (int i = 0; i < NbSymb; i++) 
    {
        printf("%s = %d\n", TS[i].nom, TS[i].valeur);
    }
}

extern int estPresentTS(char *idf, int *v) {
    for (int i = 0; i < NbSymb; i++) {
        if (strcmp(TS[i].nom, idf) == 0) 
        {
            *v = TS[i].valeur;
            printf("Inserting/Updating %s with value %d from estPresentTS\n", idf, TS[i].valeur);
            return 1;  // Vrai
        }
    }
    return 0;
}

extern void insererTS(char *idf, int v) 
{
    int index = -1;
    for (int i = 0; i < NbSymb; i++) 
    {
        if (strcmp(TS[i].nom, idf) == 0) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        TS[index].valeur = v;
    } else {
        if (NbSymb < NBMAXSYMB) {
            strcpy(TS[NbSymb].nom, idf);
            TS[NbSymb].valeur = v;
            printf("Inserting/Updating %s with value %d\n from insererTS", idf, v);
            NbSymb++;
        }else{
            printf("NBMAXSYMB\n");
        }
    }
}
