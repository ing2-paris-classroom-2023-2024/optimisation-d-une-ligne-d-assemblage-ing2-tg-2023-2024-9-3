//
// Created by edoua on 02/12/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_STRUCTURES_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_STRUCTURES_H
// Structure pour représenter un sommet d'opération
struct t_operation {
    int nom;
    float duree; // Ajouter d'autres informations si nécessaire
};
typedef struct t_operation Operation;

struct CheminMaximal {
    int sommet;
    float poids;
    int *chemin;
};

// Structure pour représenter une station
struct t_station {
    int *operation;
    int nb_operation;
};
typedef struct t_station Station;
struct Sommet {
    char nom[50];
    int numero;
    float temps_execution;
};
typedef struct Sommet t_sommet;
// Structure pour représenter les exlusions
struct t_exclusion {
    char sommet1[50];
    char sommet2[50];
};
typedef struct t_exclusion exclusion;

// Structure pour représenter un graphe

struct t_graphe {
    int numSommets;
    int **matriceAdjacence;
    float **matricePonderation;
    t_sommet *sommets;
    float *temps;
    int nombreSommets;
    int *station;
};
typedef struct t_graphe Graphe;

typedef struct arete {
    int debut;
    int fin;
} t_arete;


#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_9_33_STRUCTURES_H
