//
// Created by chloe on 02/12/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_EXCLUSIONS_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_EXCLUSIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"



int lire_operation(t_sommet **sommets, char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur d'ouverture operation.txt");
        exit(1);
    }
    int numSommets = 0;
    char ligne[50];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        *sommets = (t_sommet *)realloc(*sommets, (numSommets + 1) * sizeof(t_sommet));
        sscanf(ligne, "%s", (*sommets)[numSommets].nom);
        numSommets++;
    }
    return numSommets;
}

int lireFichierExclusions(t_exclusion *exclusions, char *nomFichier) {
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

int trouvernom(t_sommet *sommets, int numero_sommet, char *nom_sommet) {
    for (int i = 0; i < numero_sommet; i++) {
        if (strcmp(sommets[i].nom, nom_sommet) == 0) {
            return i;
        }
    }
    return -1;
}

void initialiserGraphe(t_graphe *graphe, int numSommets) {
    graphe->numSommets = numSommets;
    graphe->matriceAdjacence = (int **)malloc(numSommets * sizeof(int *));
    for (int i = 0; i < numSommets; i++) {
        graphe->matriceAdjacence[i] = (int *)malloc(numSommets * sizeof(int));
        for (int j = 0; j < numSommets; j++) {
            graphe->matriceAdjacence[i][j] = 0;
        }
    }
    graphe->sommets = (t_sommet *)malloc(numSommets * sizeof(t_sommet));
    graphe->station = (int *)malloc(numSommets * sizeof(int));
}

void ajouterArc(t_graphe *graphe, int sommet1, int sommet2) {
    graphe->matriceAdjacence[sommet1][sommet2] = 1;
    graphe->matriceAdjacence[sommet2][sommet1] = 1;
}

void afficherGraphe(t_graphe *graphe) {
    printf("t_graphe:\n");
    for (int i = 0; i < graphe->numSommets; i++) {
        printf("t_sommet %s est relie a : ", graphe->sommets[i].nom);
        for (int j = 0; j < graphe->numSommets; j++) {
            if (graphe->matriceAdjacence[i][j] == 1) {
                printf("%s ", graphe->sommets[j].nom);
            }
        }
        printf("\n");
    }
}
void colorerGraphe(t_graphe *graphe) {
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
void affichagestation_exclusion(t_graphe *graphe) {
    //printf("Affichage des stations uniquement contrainte d'exclusion :\n");
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
                printf("%s ", graphe->sommets[i].nom);
            }
        }
        printf("\n");
    }
}


void repartition_exclusion(char *fichier_operation, char *fichier_exclusion) {
    t_graphe graphe;
    t_sommet *sommets = NULL;
    t_exclusion exclusions[100];
    int numSommets = lire_operation(&sommets, fichier_operation);
    initialiserGraphe(&graphe, numSommets);
    for (int i = 0; i < numSommets; i++) {
        strcpy(graphe.sommets[i].nom, sommets[i].nom);
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
    affichagestation_exclusion(&graphe);
    //afficherGraphe(&graphe);

#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_EXCLUSIONS_H
