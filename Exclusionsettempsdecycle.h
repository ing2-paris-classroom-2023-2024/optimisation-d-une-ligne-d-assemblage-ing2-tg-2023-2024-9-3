//
// Created by Alice on 03/12/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_EXCLUSIONSETTEMPSDECYCLE_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_EXCLUSIONSETTEMPSDECYCLE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

// LECTURE DES FICHIERS
int Lire_Operations(t_sommet **sommets, char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur d'ouverture operation.txt");
        exit(1);
    }
    int numSommets = 0;
    char ligne[50];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        *sommets = (t_sommet *)realloc(*sommets, (numSommets + 1) * sizeof(t_sommet));
        sscanf(ligne, "%s %f", (*sommets)[numSommets].nom, &(*sommets)[numSommets].temps_execution);
        numSommets++;
    }
    return numSommets;
}

int Lire_Exclusions(exclusion *exclusions, char *nomFichier) {
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

int Lire_Temps_Cycle(char *nomFichier) {
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

int TrouverNom(t_sommet *sommets, int numero_sommet, char *nom_sommet) {
    for (int i = 0; i < numero_sommet; i++) {
        if (strcmp(sommets[i].nom, nom_sommet) == 0) {
            return i;
        }
    }
    return -1;
}

void InitialisationGraphe(Graphe *graphe, int numSommets) {
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

void AjouterUnArc(Graphe *graphe, int sommet1, int sommet2) {
    graphe->matriceAdjacence[sommet1][sommet2] = 1;
    graphe->matriceAdjacence[sommet2][sommet1] = 1;
}

void AfficherLeGraphe(Graphe *graphe) {
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

int ComparerLesCouleurs(const void *a, const void *b) {
    return ((t_sommet *)a)->couleur - ((t_sommet *)b)->couleur;
}

void CalculerLesDegres(Graphe *graphe, int *degres) {
    for (int i = 0; i < graphe->numSommets; i++) {
        degres[i] = 0;
        for (int j = 0; j < graphe->numSommets; j++) {
            if (graphe->matriceAdjacence[i][j] == 1) {
                degres[i]++;
            }
        }
    }
}

void EchangerLesSommets(t_sommet *a, t_sommet *b, int *degreA, int *degreB) {
    t_sommet tempSommet = *a;
    *a = *b;
    *b = tempSommet;
    int tempDegre = *degreA;
    *degreA = *degreB;
    *degreB = tempDegre;
}

t_sommet* TriABulleDegres(t_sommet *sommets, int *degres, int numSommets) {
    //Copie du tableau sommets
    t_sommet *sommetsCopie = (t_sommet *)malloc(numSommets * sizeof(t_sommet));
    for (int i = 0; i < numSommets; i++) {
        sommetsCopie[i] = sommets[i];
    }
    for (int i = 0; i < numSommets - 1; i++) {
        for (int j = 0; j < numSommets - i - 1; j++) {
            if (degres[j] < degres[j + 1]) {
                EchangerLesSommets(&sommetsCopie[j], &sommetsCopie[j + 1], &degres[j], &degres[j + 1]);
            }
        }
    }

    return sommetsCopie;
}
t_sommet * WelshPowelColoration(Graphe *graphe, t_sommet *sommetsTrieesParDegre, int tempsCycle) {
    int couleur = 1;

    t_sommet *infosColorees = (t_sommet *)malloc(graphe->numSommets * sizeof(t_sommet));

    for (int i = 0; i < graphe->numSommets; i++) {
        int sommetActuel = TrouverNom(sommetsTrieesParDegre, graphe->numSommets, graphe->sommets[i].nom);
        graphe->station[sommetActuel] = -1;
        int couleurUtilisee[graphe->numSommets];
        for (int j = 0; j < graphe->numSommets; j++) {
            couleurUtilisee[j] = 0;
        }
        if (graphe->degres[sommetActuel] == 0) {
            graphe->station[sommetActuel] = couleur;
            strcpy(infosColorees[i].nom, graphe->sommets[sommetActuel].nom);
            infosColorees[i].temps_execution = graphe->sommets[sommetActuel].temps_execution;
            infosColorees[i].couleur = couleur;
            couleur++;
            continue;  // Passer au sommet suivant
        }

        // Parcourir les voisins du sommet dans l'ordre du tableau trié
        for (int j = 0; j < graphe->numSommets; j++) {
            int voisin = TrouverNom(sommetsTrieesParDegre, graphe->numSommets, graphe->sommets[j].nom);
            if (graphe->matriceAdjacence[sommetActuel][voisin] == 1 && graphe->station[voisin] != -1) {
                couleurUtilisee[graphe->station[voisin]] = 1;
            }
        }
        int couleurDisponible;
        for (couleurDisponible = 1; couleurDisponible <= graphe->numSommets; couleurDisponible++) {
            if (couleurUtilisee[couleurDisponible] == 0) {
                break;
            }
        }
        graphe->station[sommetActuel] = couleurDisponible;

        strcpy(infosColorees[i].nom, graphe->sommets[sommetActuel].nom);
        infosColorees[i].temps_execution = graphe->sommets[sommetActuel].temps_execution;
        infosColorees[i].couleur = couleurDisponible;

        if (couleurDisponible >= couleur) {
            couleur = couleurDisponible + 1;
        }
    }
    return infosColorees;
}

void AfficherStationsEtOperations(Graphe *graphe, t_sommet *infosColorees) {
    int nombreStations = 0;
    for (int i = 0; i < graphe->numSommets; i++) {
        if (graphe->station[i] > nombreStations) {
            nombreStations = graphe->station[i];
        }
    }

    for (int s = 1; s <= nombreStations; s++) {
        printf("Station %d contient les operations : \n", s);
        float tempsTotalStation = 0.0;  //Stocke le temps total d'une station
        for (int i = 0; i < graphe->numSommets; i++) {
            if (graphe->station[i] == s) {
                printf("%s ", infosColorees[i].nom);
                tempsTotalStation += infosColorees[i].temps_execution;
            }
        }
        printf("\nSomme des temps d'execution dans la station %d : %.2f\n\n", s, tempsTotalStation);
    }
    printf("\n\n");
}

void RepartitionExclusionTempsCycle(char *fichier_operation, char *fichier_exclusion, int tempsCycle) {
    Graphe graphe;
    t_sommet *sommets = NULL;
    exclusion exclusions[100];
    int numSommets = Lire_Operations(&sommets, fichier_operation);
    InitialisationGraphe(&graphe, numSommets);
    for (int i = 0; i < numSommets; i++) {
        strcpy(graphe.sommets[i].nom, sommets[i].nom);
        graphe.sommets[i].temps_execution = sommets[i].temps_execution;
    }
    int numExclusions = Lire_Exclusions(exclusions, fichier_exclusion);
    for (int i = 0; i < numExclusions; i++) {
        int indiceSommet1 = TrouverNom(sommets, numSommets, exclusions[i].sommet1);
        int indiceSommet2 = TrouverNom(sommets, numSommets, exclusions[i].sommet2);
        if (indiceSommet1 != -1 && indiceSommet2 != -1) {
            AjouterUnArc(&graphe, indiceSommet1, indiceSommet2);
        }
    }
    int degres[numSommets];
    CalculerLesDegres(&graphe, degres);
    t_sommet *sommetsTries = TriABulleDegres(graphe.sommets, degres, numSommets);


    t_sommet *infosColorees = WelshPowelColoration(&graphe, sommetsTries, tempsCycle);

    qsort(infosColorees, numSommets, sizeof(t_sommet), ComparerLesCouleurs);


    int stationActuelle = 0;
    float tempsTotalStation = 0.0;


    for (int i = 0; i < numSommets; i++) {
        if (infosColorees[i].couleur != infosColorees[i - 1].couleur || tempsTotalStation + infosColorees[i].temps_execution > tempsCycle) {
            stationActuelle++;
            tempsTotalStation = 0.0;
        }

        graphe.station[i] = stationActuelle;
        tempsTotalStation += infosColorees[i].temps_execution;
    }

    /*printf("\nAssignation des stations :\n");
    for (int i = 0; i < numSommets; i++) {
        printf("%-3s (%.2f) : Couleur: %-2d, Station: %-2d\n", infosColorees[i].nom, infosColorees[i].temps_execution, infosColorees[i].couleur, graphe.station[i]);
    }*/

    printf("Temps de Cycle : %d\n\n", tempsCycle);
    AfficherStationsEtOperations(&graphe, infosColorees);

    free(sommetsTries);
    free(sommets);
    free(infosColorees);
}

#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_EXCLUSIONSETTEMPSDECYCLE_H
