//
// Created by edoua on 14/11/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_EDOUARD_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_EDOUARD_H
struct t_sommet {
    char nom[50];
    float temp_execution;
};
typedef struct t_sommet sommet;

struct t_exclusion {
    char sommet1[50];
    char sommet2[50];
};
typedef struct t_exclusion exclusion;

struct t_graphe {
    int numSommets;
    int **matriceAdjacence;
    float **matricePonderation;
    sommet *sommets;
    float *temps;
    int *station;
};
typedef struct t_graphe Graphe;



#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_EDOUARD_H
