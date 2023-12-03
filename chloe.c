#include "exclusions.h"
#include "Exclusionsettempsdecycle.h"
#include "structures.h"
#include "precedence_temps_cycle.h"
#include "multi.h"
int main() {
    ///ouverture des fichiers necesssaires
    char *operation = "../operations.txt";
    char *exclusion = "../exclusions.txt";
    char *temps_cycle = "../temps_cycle.txt";
    char *precedences = "../precedences.txt";
    char *nouveaufichier="../nouveau_fichier.txt";
    ///repartition selon exclusions
    // repartition_exclusion(operation, exclusion);
    ///repartition selon temps de cycle et precedences
    repartition_precedence_tmpscycle(precedences,operation,nouveaufichier,temps_cycle);
    ///repartition selon exclusions et precedences
    multicontraintePREDEX();
    ///repartition selon exclusions et temps de cycle
    repartition_exclusion_temps(operation,exclusion,temps_cycle);

    return 0;
}
//