#include <stdio.h>
#define MAX_OPERATIONS 31

int ouvrir_fichiers() {
    FILE *precedences, *operations, *tempscycle;

    precedences = fopen("../precedence.txt", "r");
    operations = fopen("../operations.txt", "r");
    tempscycle = fopen("../temps_cycle.txt", "r");


    if (precedences == NULL || operations == NULL || tempscycle == NULL) {

        if (precedences != NULL) fclose(precedences);
        if (operations != NULL) fclose(operations);
        if (tempscycle != NULL) fclose(tempscycle);

        return 1;
    }

    return 0;
}

typedef struct {
    char nom_operation[50];
    int temps_execution;
} Operation;

void lire_operations(FILE *fichier, Operation operations[], int *nombre_operations) {
    while (fscanf(fichier, "%s %d", operations[*nombre_operations].nom_operation, &operations[*nombre_operations].temps_execution) == 2) {
        (*nombre_operations)++;
    }
}

void lire_contraintes(FILE *fichier, char contraintes[][50], int *nombre_contraintes) {
    while (fscanf(fichier, "%s %s", contraintes[*nombre_contraintes], contraintes[*nombre_contraintes + 1]) == 2) {
        (*nombre_contraintes) += 2;
    }
}

void lire_temps_cycle(FILE *fichier, int *t_cycle) {
    fscanf(fichier, "%*s %d", t_cycle);
}


int main() {

    int resultat = ouvrir_fichiers();
    if (resultat == 0) {
        printf("Pas de pb.\n");
    } else {
        printf("Erreur\n");
    }
    Operation operations[MAX_OPERATIONS];
    char contraintes[MAX_OPERATIONS][50];
    int nombre_operations = 0;
    int nombre_contraintes = 0;
    int t_cycle;

    lire_operations(operations, operations, &nombre_operations);
    //lire_contraintes(precedences, contraintes, &nombre_contraintes);
    //lire_temps_cycle(fichier_temps_cycle, &t_cycle);

   // fclose(precedences);
    fclose(operations);
   /// fclose(tempscycle);

    return 0;
}
