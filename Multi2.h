//
// Created by alice on 14/11/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_EDOUARD_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_EDOUARD_H
struct t_nomSommet {
    char nom[50];
    int tempsExecution;
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
};
typedef struct t_graphe Graphe;

#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_EDOUARD_H