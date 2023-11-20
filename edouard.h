//
// Created by edoua on 14/11/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_EDOUARD_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_EDOUARD_H
struct t_graphe {
    int **matriceAdjacence;
    int nombreSommets;
};
typedef struct t_graphe Graphe;

struct t_graheponderer {
    float **matrceponderation;
    int nbsommet
};
typedef struct t_grapheponderer Graphe_ponderer;

struct t_station {
    int *operation;
    int nb_operation;
};
typedef struct t_station Station;

#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_EDOUARD_H
