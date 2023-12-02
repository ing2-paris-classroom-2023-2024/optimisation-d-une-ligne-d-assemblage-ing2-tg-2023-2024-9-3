#include "multi.h"

int main() {
    char *operation = "../operations.txt";
    char *exclusion = "../exclusions.txt";
    char *temps_cycle = "../temps_cyle.txt";
    char *precedences = "../precedences.txt";
    char *nouveaufichier="../nouveau_fichier.txt";
    repartition_exclusion(operation, exclusion);
    repartition_precedence_tmpscycle(precedences,operation,nouveaufichier,temps_cycle);
    affichagefinal(temps_cycle);
    return 0;
}