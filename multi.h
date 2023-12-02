//
// Created by chloe on 02/12/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_MULTI_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_MULTI_H
#include "structures.h"
#include "exclusions.h"
#include "prectmpscycle.h"

int *nouvelleRepartition(Graphe *graphe, float tempsCycle) {
    int *repartition1 = triDansStations(graphe, tempsCycle);  // Récupère la première répartition
    int *repartition2 = affichagestation_exclusion(graphe);    // Récupère la deuxième répartition

    int *nouvelleRepartition = (int *)malloc(graphe->nombreSommets * sizeof(int));

    // Copie la répartition initiale dans la nouvelle répartition
    for (int i = 0; i < graphe->nombreSommets; ++i) {
        nouvelleRepartition[i] = repartition1[i];
    }

    // Vérifie et ajuste la nouvelle répartition en fonction de la deuxième répartition
    for (int i = 0; i < graphe->nombreSommets; ++i) {
        for (int j = i + 1; j < graphe->nombreSommets; ++j) {
            // Si les sommets i et j sont dans la même station dans la repartition1
            // mais pas dans la repartition2, scinder en 2 stations
            if (repartition1[i] == repartition1[j] && repartition2[i] != repartition2[j]) {
                // Crée une nouvelle station pour le sommet j
                for (int k = 0; k < graphe->nombreSommets; ++k) {
                    if (nouvelleRepartition[k] == repartition1[j]) {
                        nouvelleRepartition[k] = repartition1[j] + 1;
                    }
                }
            }
        }
    }

    // Affiche la nouvelle répartition
    for (int i = 0; i < graphe->nombreSommets; ++i) {
        printf("Station %d : ", nouvelleRepartition[i] + 1);
        for (int j = 0; j < graphe->nombreSommets; ++j) {
            if (nouvelleRepartition[j] == i) {
                printf("%d ", j);
            }
        }
        printf("\n");
    }

    free(repartition1);
    free(repartition2);

    return nouvelleRepartition;
}
void affichagefinal(char* fichierTempsCycle){
    printf("affichage multicontraintes\n");
    Graphe graphe;
    float tempsCycle= liretempscycle(fichierTempsCycle);
    nouvelleRepartition(&graphe,tempsCycle);

}

#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_MULTI_H
