#include <stdio.h>
#include "edouard.h"
#include <stdlib.h>

void lireFichier(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur ouverture de %s\n", nomFichier);
        exit(0);
    }
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        printf("%s", ligne);
    }
    fclose(fichier);
}

int main() {
    char *nomFichier = "../temps_cycle.txt";
    lireFichier(nomFichier);
    return 0;
}
