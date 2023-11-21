#include <stdio.h>
#include <stdlib.h>

struct t_graphe {
    float **matricePonderation;
    float *temps;
    int nombreSommets;
};
typedef struct t_graphe Graphe_pondere;

void lireGraphe(const char *nomFichier, Graphe_pondere *graphe) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier : %s\n", nomFichier);
        exit(0);
    }
    int sommet1, sommet2;
    graphe->nombreSommets = 0;
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        if (sommet1 > graphe->nombreSommets) {
            graphe->nombreSommets = sommet1;
        }
        if (sommet2 > graphe->nombreSommets) {
            graphe->nombreSommets = sommet2;
        }
    }
    rewind(fichier);
    graphe->matricePonderation = (float **)malloc((graphe->nombreSommets + 1) * sizeof(float *));
    graphe->temps = (float *)malloc((graphe->nombreSommets + 1) * sizeof(float));

    for (int i = 0; i <= graphe->nombreSommets; i++) {
        graphe->matricePonderation[i] = (float *)calloc((graphe->nombreSommets + 1), sizeof(float));
        graphe->temps[i] = 0.0;
    }
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        graphe->matricePonderation[sommet1][sommet2] = 1.0;
    }

    fclose(fichier);
}
void lirePonderations(const char *nomFichier, Graphe_pondere *gr) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier : %s\n", nomFichier);
        exit(0);
    }
    int sommet;
    float temps;
    while (fscanf(fichier, "%d %f", &sommet, &temps) == 2) {
        if (sommet > 0 && sommet <= gr->nombreSommets) {
            gr->temps[sommet] = temps;
        }
    }

    fclose(fichier);
}

void afficherGraphePonderation(Graphe_pondere *gr) {
    printf("Graphe avec ponderations :\n");
    for (int i = 1; i <= gr->nombreSommets; i++) {
        for (int j = 1; j <= gr->nombreSommets; j++) {
            if (gr->matricePonderation[i][j] != 0.0) {
                printf("(%d -> %d) Temps : %.2f\n", i, j, gr->temps[i]);
            }
        }
    }
}

int main() {
    Graphe_pondere graphe;
    graphe.matricePonderation = NULL;
    graphe.temps = NULL;
    lireGraphe("../precedence.txt", &graphe);
    lirePonderations("../operations.txt", &graphe);
    afficherGraphePonderation(&graphe);
    for (int i = 0; i <= graphe.nombreSommets; i++) {
        free(graphe.matricePonderation[i]);
    }
    free(graphe.matricePonderation);
    free(graphe.temps);

    return 0;
}
