#include "edouard.h"
#include <stdio.h>
#include <stdlib.h>

void lireContraintes(const char *nomFichier, int contraintes[][2], int *nombreContraintes) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier :  %s\n", nomFichier);
        exit(0);
    }
    *nombreContraintes = 0;
    while (fscanf(fichier, "%d %d", &contraintes[*nombreContraintes][0], &contraintes[*nombreContraintes][1]) == 2) {
        (*nombreContraintes)++;
    }
    fclose(fichier);
}

Graphe creerGraphe(int contraintes[][2], int nombreContraintes) {
    Graphe graphe;
    graphe.nombreSommets = 0;

    for (int i = 0; i < nombreContraintes; i++) {
        if (contraintes[i][0] > graphe.nombreSommets) {
            graphe.nombreSommets = contraintes[i][0];
        }
        if (contraintes[i][1] > graphe.nombreSommets) {
            graphe.nombreSommets = contraintes[i][1];
        }
    }

    graphe.matriceAdjacence = (int **)malloc((graphe.nombreSommets + 1) * sizeof(int *));
    if (graphe.matriceAdjacence == NULL) {
        printf("Erreur \n");
        exit(0);
    }

    for (int i = 0; i <= graphe.nombreSommets; i++) {
        graphe.matriceAdjacence[i] = (int *)calloc((graphe.nombreSommets + 1), sizeof(int));
        if (graphe.matriceAdjacence[i] == NULL) {
            printf("Erreur \n");
            exit(0);
        }
    }
    for (int i = 0; i < nombreContraintes; i++) {
        graphe.matriceAdjacence[contraintes[i][0]][contraintes[i][1]] = 1;
        graphe.matriceAdjacence[contraintes[i][1]][contraintes[i][0]] = 1;
    }
    return graphe;
}

void libererGraphe(Graphe graphe) {
    for (int i = 0; i <= graphe.nombreSommets; i++) {
        free(graphe.matriceAdjacence[i]);
    }
    free(graphe.matriceAdjacence);
}

// Ajout d'une fonction pour libérer la mémoire allouée pour les tâches de chaque station
void libererStations(Station stations[], int nombreStations) {
    for (int i = 1; i <= nombreStations; i++) {
        free(stations[i].taches);
    }
}
// coloration d'un graphe grâce a l'algorithme naif
void colorerGraphe(Graphe graphe, Station stations[], int *nombreStations) {
    int *couleurs = (int *)calloc((graphe.nombreSommets + 1), sizeof(int));
    if (couleurs == NULL) {
        printf( "Erreur \n");
        exit(0);
    }
    for (int i = 1; i <= graphe.nombreSommets; i++) {
        couleurs[i] = 0;
    }
    for (int sommet = 1; sommet <= graphe.nombreSommets; sommet++) {
        int couleurDisponible = 1;

        for (int voisin = 1; voisin <= graphe.nombreSommets; voisin++) {
            if (graphe.matriceAdjacence[sommet][voisin] && couleurs[voisin] != 0) {
                couleurDisponible = 0;
                break;
            }
        }
        if (couleurDisponible) {
            couleurs[sommet] = 1;
        } else {
            for (int couleur = 2; couleur <= *nombreStations + 1; couleur++) {
                int couleurLibre = 1;
                for (int voisin = 1; voisin <= graphe.nombreSommets; voisin++) {
                    if (graphe.matriceAdjacence[sommet][voisin] && couleurs[voisin] == couleur) {
                        couleurLibre = 0;
                        break;
                    }
                }
                if (couleurLibre) {
                    couleurs[sommet] = couleur;
                    break;
                }
            }
        }
        if (couleurs[sommet] > *nombreStations) {
            *nombreStations = couleurs[sommet];
        }
    }
    for (int i = 1; i <= *nombreStations; i++) {
        stations[i].taches = (int *)malloc(graphe.nombreSommets * sizeof(int));
        stations[i].nombreTaches = 0;
    }
    // Ajout de chaque opérations a la station associé
    for (int sommet = 1; sommet <= graphe.nombreSommets; sommet++) {
        stations[couleurs[sommet]].taches[stations[couleurs[sommet]].nombreTaches++] = sommet;
    }
    free(couleurs);
}

void afficherStations(Station stations[], int nombreStations) {
    printf("Repartition des stations :\n");
    for (int i = 1; i <= nombreStations; i++) {
        printf("Station %d : \n", i);
        printf("operations : ");
        for (int j = 0; j < stations[i].nombreTaches; j++) {
            printf(" %d ", stations[i].taches[j]);
        }
        printf("\n");
    }

    printf("Lorsqu'on considere que les contraintes d'exclusion nous obtenons %d stations \n", nombreStations);
}

void repartition_station_exclusion(char *fichier) {
    int nb_operation =0 ;
    printf("combien d'operation doivent etre effectuer au total : \n");
    scanf("%d",&nb_operation);
    int contraintes[nb_operation][2];
    int nombreContraintes;
    Graphe graphe;
    Station stations[nb_operation];
    int nombreStations = 0;
    lireContraintes(fichier, contraintes, &nombreContraintes);
    graphe = creerGraphe(contraintes, nombreContraintes);
    colorerGraphe(graphe, stations, &nombreStations);
    afficherStations(stations, nombreStations);
    libererGraphe(graphe);
    libererStations(stations, nombreStations);
}

int main() {
    char *exclusion = "../exclusion.txt";
    repartition_station_exclusion(exclusion);
    return 0;
}




