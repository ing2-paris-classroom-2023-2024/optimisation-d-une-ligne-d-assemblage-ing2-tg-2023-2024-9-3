#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct t_nomSommet{
    char nom[50];
};
typedef struct t_nomSommet nom_sommet;

struct t_exclusion {
    char sommet1[50];
    char sommet2[50];
};
typedef struct t_exclusion Exclusion;

struct t_graphe {
    int numSommets;
    int **matriceAdjacence;
    nom_sommet *sommets;
};
typedef struct t_graphe Graphe;

void initialiserGraphe(Graphe *graphe, int numSommets) {
    graphe->numSommets = numSommets;
    graphe->matriceAdjacence = (int **)malloc(numSommets * sizeof(int *));
    for (int i = 0; i < numSommets; i++) {
        graphe->matriceAdjacence[i] = (int *)malloc(numSommets * sizeof(int));
        for (int j = 0; j < numSommets; j++) {
            graphe->matriceAdjacence[i][j] = 0;
        }
    }
    graphe->sommets = (nom_sommet *)malloc(numSommets * sizeof(nom_sommet));
}

void ajouterArc(Graphe *graphe, int sommet1, int sommet2) {
    graphe->matriceAdjacence[sommet1][sommet2] = 1;
    graphe->matriceAdjacence[sommet2][sommet1] = 1;
}
void afficherGraphe(Graphe *graphe) {
    printf("Graphe:\n");
    for (int i = 0; i < graphe->numSommets; i++) {
        printf("Sommet %s est relie a : ", graphe->sommets[i].nom);
        for (int j = 0; j < graphe->numSommets; j++) {
            if (graphe->matriceAdjacence[i][j] == 1) {
                printf("%s ", graphe->sommets[j].nom);
            }
        }
        printf("\n");
    }
}

int lire_operation(nom_sommet **sommets,char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Eurreur ouverture opération.txt");
        exit(0);
    }
    int numSommets = 0;
    char ligne[50];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        *sommets = (nom_sommet *)realloc(*sommets, (numSommets + 1) * sizeof(nom_sommet));
        sscanf(ligne, "%s", (*sommets)[numSommets].nom);
        numSommets++;
    }
    fclose(fichier);
    return numSommets;
}

int lireFichierExclusions(Exclusion *exclusions,char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL) {
        printf("Eurreur ouverture exclusions.txt");
        exit(0);
    }
    int numExclusions = 0;
    while (fscanf(fichier, "%s %s", exclusions[numExclusions].sommet1, exclusions[numExclusions].sommet2) == 2) {
        numExclusions++;
    }
    fclose(fichier);
    return numExclusions;
}

int trouverIndiceSommetParNom(nom_sommet *sommets, int numSommets, char *nomSommet) {
    for (int i = 0; i < numSommets; i++) {
        if (strcmp(sommets[i].nom, nomSommet) == 0) {
            return i;
        }
    }
    return -1; // Retourne -1 si le nom du sommet n'est pas trouvé
}
void graphe(){
    nom_sommet *sommets = NULL;
    Exclusion exclusions[100];
    Graphe graphe;
    int numSommets = lire_operation(&sommets, "../operations.txt");
    initialiserGraphe(&graphe, numSommets);
    for (int i = 0; i < numSommets; i++) {
        strcpy(graphe.sommets[i].nom, sommets[i].nom);
    }
    int numExclusions = lireFichierExclusions(exclusions, "../exclusions.txt");
    for (int i = 0; i < numExclusions; i++) {
        int indiceSommet1 = trouverIndiceSommetParNom(sommets, numSommets, exclusions[i].sommet1);
        int indiceSommet2 = trouverIndiceSommetParNom(sommets, numSommets, exclusions[i].sommet2);
        if (indiceSommet1 != -1 && indiceSommet2 != -1) {
            ajouterArc(&graphe, indiceSommet1, indiceSommet2);
        }
    }
    afficherGraphe(&graphe);
}

int main() {
    graphe();
    return 0;
}
