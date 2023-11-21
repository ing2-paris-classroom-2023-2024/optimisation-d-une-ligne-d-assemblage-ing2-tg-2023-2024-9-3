#include <stdio.h>

int main() {
    double valeurTC;
    FILE *fichier = fopen("temps_cycle.txt", "r");

    if (fichier == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }
    if (fscanf(fichier, "%lf", &valeurTC) != 1) {
        fprintf(stderr, "Erreur lors de la lecture de la valeurTC depuis le fichier.\n");
        fclose(fichier);
        return 1;
    }

    fclose(fichier);

    const double temps_cycle = valeurTC;

    printf("Le temps de cycle est: %lf\n", temps_cycle);

    return 0;
}
