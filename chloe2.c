#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



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
    while (scanf( "%d %f", &sommet, &ponderation) == 2) {
        if (sommet > graphe->nombreSommets) {
            graphe->nombreSommets = sommet;
        }
    }

    graphe->matricePonderation = (float **)malloc((graphe->nombreSommets + 1) * sizeof(float *));
    for (int i = 0; i <= graphe->nombreSommets; i++) {
        graphe->matricePonderation[i] = (float *)calloc((graphe->nombreSommets + 1), sizeof(float));
    }

    rewind(fichier);

    while (scanf( "%d %f", &sommet, &ponderation) == 2) {
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

    while (scanf( "%d %d", &sommet1, &sommet2) == 2) {
        if (sommet1 > graphe->nombreSommets) {
            graphe->nombreSommets = sommet1;
        }
        if (sommet2 > graphe->nombreSommets) {
            graphe->nombreSommets = sommet2;
        }
    }


    while (scanf( "%d %d", &sommet1, &sommet2) == 2) {
        graphe->matricePonderation[sommet1][sommet2] = graphe->matricePonderation[sommet2][sommet2];
        graphe->matricePonderation[sommet2][sommet1] = graphe->matricePonderation[sommet1][sommet1];
    }

    fclose(fichier);
}
void DFS(Graphe_pondere graphe, int sommet, bool *visited) {
    visited[sommet] = true;
    for (int i = 1; i <= graphe.nombreSommets; i++) {
        if (graphe.matricePonderation[sommet][i] != 0.0 && !visited[i]) {
            DFS(graphe, i, visited);
        }
    }
}

bool estConnexe(Graphe_pondere graphe) {
    bool *visited = (bool *)malloc((graphe.nombreSommets + 1) * sizeof(bool));

    for (int i = 1; i <= graphe.nombreSommets; i++) {
        visited[i] = false;
    }

    int sommetDepart = 1;
    DFS(graphe, sommetDepart, visited);
    for (int i = 1; i <= graphe.nombreSommets; i++) {
        if (!visited[i]) {
            free(visited);
            return false; // Le graphe est disjoint
        }
    }

    free(visited);
    return true; // Le graphe est connexe
}
int main() {

    Graphe_pondere graphePonderation, grapheArretes;
    graphePonderation.nombreSommets = 0;
    graphePonderation.matricePonderation = NULL;
    grapheArretes.nombreSommets = 0;
    grapheArretes.matricePonderation = NULL;

    lirePonderations("../operations.txt", &graphePonderation);
    lireGraphe("../precedence.txt", &grapheArretes);
    if (estConnexe(graphePonderation)) {
        printf("Le graphe est connexe.\n");
    } else {
        printf("Le graphe est disjoint.\n");
    }
    for (int i = 0; i <= graphePonderation.nombreSommets; i++) {
        free(graphePonderation.matricePonderation[i]);
    }
    free(graphePonderation.matricePonderation);
    for (int i = 0; i <= grapheArretes.nombreSommets; i++) {
        free(grapheArretes.matricePonderation[i]);
    }
    free(grapheArretes.matricePonderation);
    return 0;
}
