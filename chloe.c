#include <stdio.h>
#include "exclusions.h"
#include "prectmpscycle.h"

int main(){
    char *operation = "../operations.txt";
    char *exclusion = "../exclusions.txt";
    printf("\t\t\t\t\t\t\tRepartition selon contrainte d'exclusion\n");
    repartition_exclusion(operation, exclusion);
    const char *fichierGraphe = "../precedences.txt";
    const char *fichierPonderation = "../operations.txt";
    const char *nouveauFichier="../nouveau_fichier.txt";
    const char *fichierTempsCycle="../temps_cyle.txt";
    printf("\n\n\t\t\t\t\t\t\tRepartition selon contrainte precedence et temps de cycle\n");
    precedence_tmpscycle(fichierGraphe,fichierPonderation,nouveauFichier,fichierTempsCycle);
    return 0;
}