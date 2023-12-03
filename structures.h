//
// Created by Alice on 03/12/2023.
//

#ifndef UNTITLED1_STRUCTURES_H
#define UNTITLED1_STRUCTURES_H

struct CheminMaximal {
    int sommet;
    float poids;
    int *chemin;
};

struct Sommet {
    char nom[50];
    int couleur;
    float temps_execution;
    int couleur;
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
    int nombreSommets;
    int *degres;
    int *station;
};
typedef struct t_graphe Graphe;

typedef struct arete {
    int debut;
    int fin;
} t_arete;

#endif //UNTITLED1_STRUCTURES_H
