//
// Created by edoua on 02/12/2023.
//

#ifndef PROJET_TG_USINE__STRUCTURES_H
#define PROJET_TG_USINE__STRUCTURES_H
// Structure pour représenter un sommet d'opération
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//structure du chemin maximal pour l'algorithme de djikstra
struct CheminMaximal {
    int sommet;
    float poids;
    int *chemin;
};
//structure sommet pour graphe
struct Sommet {
    char nom[50];
    int numero;
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

// Structure pour  un graphe

struct t_graphe {
    int numSommets;
    int **matriceAdjacence;
    float **matricePonderation;
    t_sommet *sommets; //utilisation de la structure sommet
    int nombreSommets;
    int *degres;
    int *station;
};
typedef struct t_graphe Graphe;
//structure pour l'arete d'un graphe
typedef struct arete {
    int debut;
    int fin;
} t_arete;
//structure de coloration d'un graphe
typedef struct {
    int sommet;
    int couleur;
    int ordreDecouverte;
} SommetCouleur;
typedef struct {
    char nom[50];
    int couleur;
    int visite;
} Sommet;
//sous programme de liberation de memoire d'un graphe
void libererGraphe(Graphe *graphe) {
    for (int i = 0; i <= graphe->nombreSommets; i++) {
        free(graphe->matricePonderation[i]);
    }
    free(graphe->matricePonderation);
    free(graphe->sommets);
}

#endif //PROJET_TG_USINE__STRUCTURES_H
//