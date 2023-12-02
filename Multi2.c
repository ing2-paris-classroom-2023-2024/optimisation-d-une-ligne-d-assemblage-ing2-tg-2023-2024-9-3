#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Multi2.h"


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


void colorerGraphe(Graphe *graphe, int tempsCycle) {
    for (int i = 0; i < graphe->numSommets; i++) {
        graphe->station[i] = -1;
    }

    int stationActuelle = 0;
    for (int i = 0; i < graphe->numSommets; i++) {
        int couleurutilisee[graphe->numSommets];
        for (int j = 0; j < graphe->numSommets; j++) {
            couleurutilisee[j] = 0;
        }

        // Check if the operation can fit into the current station
        float tempsTotalStation = 0.0;
        for (int j = 0; j < graphe->numSommets; j++) {
            if (graphe->matriceAdjacence[i][j] == 1 && graphe->station[j] != -1) {
                couleurutilisee[graphe->station[j]] = 1;
                tempsTotalStation += graphe->sommets[j].tempsExecution;
            }
        }

        int couleurDisponible;
        for (couleurDisponible = 0; couleurDisponible < graphe->numSommets; couleurDisponible++) {
            if (couleurutilisee[couleurDisponible] == 0) {
                // Check if the operation fits within the cycle time constraint
                if (tempsTotalStation + graphe->sommets[i].tempsExecution <= tempsCycle) {
                    graphe->station[i] = couleurDisponible;
                    break;
                }
            }
        }


        // Check if the current station exceeds the cycle time
        if (tempsTotalStation + graphe->sommets[i].tempsExecution > tempsCycle) {
            // Move to a new station
            stationActuelle++;
        }
    }
}


void affichagestation(Graphe *graphe, int tempsCycle) {
    printf("Affichage des stations:\n");
    int nombreStations = 1;

    for (int i = 0; i < graphe->numSommets; i++) {
        if (graphe->station[i] > nombreStations) {
            nombreStations = graphe->station[i];
        }
    }

    for (int s = 0; s <= nombreStations; s++) {
        printf("Station %d contient les operations : ", s + 1);
        float tempsTotalStation = 0.0;

        for (int i = 0; i < graphe->numSommets; i++) {
            if (graphe->station[i] == s) {
                // Vérifier si l'ajout de cette opération dépasse le temps de cycle
                if (tempsTotalStation + graphe->sommets[i].tempsExecution > tempsCycle) {
                    printf("\n");
                    tempsTotalStation = 0.0;
                }

                if (graphe->station[i] == s) {
                    printf("%s (%.2f) ", graphe->sommets[i].nom, graphe->sommets[i].tempsExecution);
                    tempsTotalStation += graphe->sommets[i].tempsExecution;
                }
            }
        }

        if (tempsTotalStation > 0.0) {
            printf("\nTemps total d'execution de la station %d : %.2f\n\n", s + 1, tempsTotalStation);
        }
    }
}




Graphe graphe(char *fichier_operation, char *fichier_exclusion, int tempsCycle) {
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
    colorerGraphe(&graphe, tempsCycle);
    affichagestation(&graphe, tempsCycle);
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
    graphe(operation, exclusion, tempsCycle);
    return 0;
}
