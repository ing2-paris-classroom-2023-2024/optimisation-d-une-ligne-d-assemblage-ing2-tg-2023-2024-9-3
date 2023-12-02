//
// Created by chloe on 02/12/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_STRUCTURES_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_STRUCTURES_H
typedef struct Sommet {
    char nom[50];
    int numero;
    float temps_execution;
}t_sommet;
struct Graphe {
    int numSommets;
    int nombreSommets;
    int **matriceAdjacence;
    float **matricePonderation;
    t_sommet *sommets;
    int *station;
};
typedef struct Graphe t_graphe;

struct CheminMaximal {
    int sommet;
    float poids;
    int *chemin;
};
typedef struct arete {
    int debut;
    int fin;
} t_arete;
typedef struct exclusion {
    char sommet1[50];
    char sommet2[50];
}t_exclusion;
#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_3_STRUCTURES_H
