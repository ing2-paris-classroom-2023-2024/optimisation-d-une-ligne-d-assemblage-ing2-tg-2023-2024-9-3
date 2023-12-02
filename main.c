#include "exclusions.h"
#include "precedence_temps_cycle.h"

int main() {
    char *operation = "../operations.txt";
    char *exclusion = "../exclusions.txt";
    char *temps_cycle = "../temps_cycle.txt";
    char *precedences = "../precedence.txt";
    char *nouveaufichier="../nouveau_fichier.txt";
    repartition_exclusion(operation, exclusion);
    repartition_precedence_tmpscycle(precedences,operation,nouveaufichier,temps_cycle);
    return 0;
}
