#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



struct t_graphe {
    float **matricePonderation;
    int nombreSommets;
};
typedef struct t_graphe Graphe_pondere;

struct t_arete {
    int sommet1;
    int sommet2;
    float ponderation;
};
typedef struct t_arete Arete;

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
int compareArete(const void *a, const void *b) {
    return ((Arete *)a)->ponderation > ((Arete *)b)->ponderation;
}
int find(int parent[], int i) {
    if (parent[i] == -1) {
        return i;
    }
    return find(parent, parent[i]);
}
void unionSet(int parent[], int x, int y) {
    int xset = find(parent, x);
    int yset = find(parent, y);
    parent[xset] = yset;
}
void afficherArbreCouvrantMinimal(Graphe_pondere graphe, Arete *arbreCouvrant, int taille) {
    printf("\nArbre Couvrant Minimal (Algorithme de Kruskal) :\n");

    // Tableau pour suivre les sommets déjà inclus dans l'arbre
    bool sommetsInclus[graphe.nombreSommets + 1];
    memset(sommetsInclus, false, sizeof(sommetsInclus));

    for (int i = 0; i < taille; i++) {
        int x = arbreCouvrant[i].sommet1;
        int y = arbreCouvrant[i].sommet2;

        // Ajouter l'arête à l'arbre couvrant si les sommets ne sont pas déjà inclus
        if (!sommetsInclus[x] || !sommetsInclus[y]) {
            printf("(%d, %d): %f\n", x, y, arbreCouvrant[i].ponderation);

            // Marquer les sommets comme inclus
            sommetsInclus[x] = true;
            sommetsInclus[y] = true;
        }
    }
}

void kruskalMST(Graphe_pondere graphe) {
    int parent[graphe.nombreSommets + 1];
    memset(parent, -1, sizeof(parent));

    // Créer un tableau pour stocker les arêtes
    Arete *aretes = (Arete *)malloc(graphe.nombreSommets * graphe.nombreSommets * sizeof(Arete));
    int count = 0;

    // Remplir le tableau avec les arêtes du graphe
    for (int i = 1; i <= graphe.nombreSommets; i++) {
        for (int j = 1; j <= graphe.nombreSommets; j++) {
            if (graphe.matricePonderation[i][j] != 0.0) {
                aretes[count].sommet1 = i;
                aretes[count].sommet2 = j;
                aretes[count].ponderation = graphe.matricePonderation[i][j];
                count++;
            }
        }
    }

    // Trier les arêtes par poids
    qsort(aretes, count, sizeof(Arete), compareArete);

    // Créer un tableau pour stocker l'arbre couvrant minimal
    Arete *arbreCouvrant = (Arete *)malloc((graphe.nombreSommets - 1) * sizeof(Arete));
    int tailleArbre = 0;

    for (int i = 0; i < count && tailleArbre < graphe.nombreSommets - 1; i++) {
        int x = find(parent, aretes[i].sommet1);
        int y = find(parent, aretes[i].sommet2);

        // Ajouter l'arête à l'arbre couvrant si elle ne crée pas de cycle
        if (x != y) {
            arbreCouvrant[tailleArbre++] = aretes[i];
            unionSet(parent, x, y);
        }
    }

    // Afficher l'arbre couvrant minimal avec chaque sommet une seule fois
    afficherArbreCouvrantMinimal(graphe, arbreCouvrant, tailleArbre);

    free(aretes);
    free(arbreCouvrant);
}
int main() {
    Graphe_pondere graphe;
    graphe.nombreSommets = 0;
    graphe.matricePonderation = NULL;
    lirePonderations("../operations.txt", &graphe);
    lireGraphe("../precedence.txt", &graphe);
    afficherGraphePonderation(graphe);
    kruskalMST(graphe);
    for (int i = 0; i <= graphe.nombreSommets; i++) {
        free(graphe.matricePonderation[i]);
    }
    free(graphe.matricePonderation);
    return 0;
}