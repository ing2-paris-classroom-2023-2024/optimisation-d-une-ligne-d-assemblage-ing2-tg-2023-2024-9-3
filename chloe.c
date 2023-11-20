#include <stdio.h>
#include <stdlib.h>


struct t_graphe {
    float **matricePonderation;
    int nombreSommets;
};
typedef struct t_graphe Graphe_pondere;

void lirePonderations(char *nomFichier, Graphe_pondere *graphe) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier : %s\n", nomFichier);
        exit(0);
    }
    int sommet;
    float ponderation;
    while (fscanf(fichier, "%d %f", &sommet, &ponderation) == 2) {
        if (sommet > graphe->nombreSommets) {
            graphe->nombreSommets = sommet;
        }
    }
    graphe->matricePonderation = (float **)malloc((graphe->nombreSommets + 1) * sizeof(float *));
    for (int i = 0; i <= graphe->nombreSommets; i++) {
        graphe->matricePonderation[i] = (float *)calloc((graphe->nombreSommets + 1), sizeof(float));
    }
    rewind(fichier);
    while (fscanf(fichier, "%d %f", &sommet, &ponderation) == 2) {
        graphe->matricePonderation[sommet][sommet] = ponderation;
    }
    fclose(fichier);
}
void lireGraphe(const char *nomFichier, Graphe_pondere *graphe) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier : %s\n", nomFichier);
        exit(0);
    }
    int sommet1, sommet2;
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        if (sommet1 > graphe->nombreSommets) {
            graphe->nombreSommets = sommet1;
        }
        if (sommet2 > graphe->nombreSommets) {
            graphe->nombreSommets = sommet2;
        }
    }
    rewind(fichier);
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        graphe->matricePonderation[sommet1][sommet2] = graphe->matricePonderation[sommet2][sommet2];
        graphe->matricePonderation[sommet2][sommet1] = graphe->matricePonderation[sommet1][sommet1];
    }
    fclose(fichier);
}

void afficherGraphePonderation(Graphe_pondere graphe) {
    printf("Graphe :\n");
    for (int i = 1; i <= graphe.nombreSommets; i++) {
        for (int j = 1; j <= graphe.nombreSommets; j++) {
            if (i != j && graphe.matricePonderation[i][j] != 0.0) {
                printf("(%d, %d): %f\n", i, j, graphe.matricePonderation[i][j]);
            }
        }
    }
}
int main() {
    Graphe_pondere graphe;
    graphe.nombreSommets = 0;
    graphe.matricePonderation = NULL;
    lirePonderations("../operations.txt", &graphe);
    lireGraphe("../precedence.txt", &graphe);
    afficherGraphePonderation(graphe);
    for (int i = 0; i <= graphe.nombreSommets; i++) {
        free(graphe.matricePonderation[i]);
    }
    free(graphe.matricePonderation);
    return 0;
}