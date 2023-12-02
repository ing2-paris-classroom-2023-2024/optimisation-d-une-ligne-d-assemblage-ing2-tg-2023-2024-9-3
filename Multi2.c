#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "edouard.h"

struct t_nomSommet {
    char nom[50];
    float tempsExecution;
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
    int *station;
    float *temps
};
typedef struct t_graphe Graphe;

int lire_operation(Graphe* graphe, nom_sommet **sommets, char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur d'ouverture operation.txt");
        exit(1);
    }
    int numSommets = 0;
    char ligne[50];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        *sommets = (nom_sommet *)realloc(*sommets, (numSommets + 1) * sizeof(nom_sommet));
        sscanf(ligne, "%s %f", (*sommets)[numSommets].nom, &(*sommets)[numSommets].tempsExecution);
        numSommets++;
    }
    fclose(fichier);
    //fclose(fichier);
    return numSommets;
}

void lirePond(Graphe *graphe){
    printf("Sommets avec leur ponderation :\n");
    for (int i = 0; i < graphe->numSommets; i++) {
        printf("Sommet %s : Temps d'execution %.2f\n", graphe->sommets[i].nom, graphe->sommets[i].tempsExecution);
    }
}



int lireFichierExclusions(Exclusion *exclusions, char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur d'ouverture exclusion.txt");
        exit(1);
    }
    int numExclusions = 0;
    while (fscanf(fichier, "%s %s", exclusions[numExclusions].sommet1, exclusions[numExclusions].sommet2) == 2) {
        numExclusions++;
    }
    return numExclusions;
}

int lireTempsCycle(char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur d'ouverture temps_cycle.txt");
        exit(1);
    }
    int tempsCycle;
    fscanf(fichier, "%d", &tempsCycle);
    fclose(fichier);
    return tempsCycle;
}


int trouvernom(nom_sommet *sommets, int numero_sommet, char *nom_sommet) {
    for (int i = 0; i < numero_sommet; i++) {
        if (strcmp(sommets[i].nom, nom_sommet) == 0) {
            return i;
        }
    }
    return -1;
}

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
    graphe->station = (int *)malloc(numSommets * sizeof(int));
    graphe->temps = (float *)malloc(numSommets * sizeof(float));  // Allocation de mémoire pour les temps d'exécution
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



void colorerGraphe(Graphe *graphe) {
    for (int i = 0; i < graphe->numSommets; i++) {
        graphe->station[i] = -1;
    }
    for (int i = 0; i < graphe->numSommets; i++) {
        int couleurutilisee[graphe->numSommets];
        for (int j = 0; j < graphe->numSommets; j++) {
            couleurutilisee[j] = 0;
        }
        for (int j = 0; j < graphe->numSommets; j++) {
            if (graphe->matriceAdjacence[i][j] == 1 && graphe->station[j] != -1) {
                couleurutilisee[graphe->station[j]] = 1;
            }
        }
        int couleurDisponible;
        for (couleurDisponible = 0; couleurDisponible < graphe->numSommets; couleurDisponible++) {
            if (couleurutilisee[couleurDisponible] == 0) {
                break;
            }
        }
        graphe->station[i] = couleurDisponible;
    }
}
void affichagestation(Graphe *graphe) {
    printf("Affichage des stations:\n");
    int nombreStations = 0;
    for (int i = 0; i < graphe->numSommets; i++) {
        if (graphe->station[i] > nombreStations) {
            nombreStations = graphe->station[i];
        }
    }
    for (int s = 0; s <= nombreStations; s++) {
        printf("Station %d contient les operations : \n", s + 1);
        for (int i = 0; i < graphe->numSommets; i++) {
            if (graphe->station[i] == s) {
                printf("%s (%.2f) ", graphe->sommets[i].nom, graphe->sommets[i].tempsExecution);
            }
        }
        printf("\n");
    }
}



Graphe graphe(char *fichier_operation, char *fichier_exclusion) {
    Graphe graphe;
    nom_sommet *sommets = NULL;
    Exclusion exclusions[100];
    int numSommets = lire_operation(&graphe, &sommets, fichier_operation);
    initialiserGraphe(&graphe, numSommets);
    for (int i = 0; i < numSommets; i++) {
        strcpy(graphe.sommets[i].nom, sommets[i].nom);
        graphe.sommets[i].tempsExecution = sommets[i].tempsExecution;  // Mettez à jour les temps d'exécution
        graphe.temps[i] = sommets[i].tempsExecution;  // Stockage des temps d'exécution
    }
    int numExclusions = lireFichierExclusions(exclusions, fichier_exclusion);
    for (int i = 0; i < numExclusions; i++) {
        int indiceSommet1 = trouvernom(sommets, numSommets, exclusions[i].sommet1);
        int indiceSommet2 = trouvernom(sommets, numSommets, exclusions[i].sommet2);
        if (indiceSommet1 != -1 && indiceSommet2 != -1) {
            ajouterArc(&graphe, indiceSommet1, indiceSommet2);
        }
    }
    colorerGraphe(&graphe);
    affichagestation(&graphe);
    lirePond(&graphe);
    //afficherGraphe(&graphe);
    return graphe;
}

int main() {
    char *operation = "../operations.txt";
    char *exclusion = "../exclusions.txt";
    char *temps_cycle = "../temps_cycle.txt";
    int tempsCycle;
    tempsCycle = lireTempsCycle(temps_cycle);
    printf("temps cycle : %d\n", tempsCycle);
    graphe(operation, exclusion);
    return 0;
}
