//
// Created by edoua on 02/12/2023.
//

#ifndef PROJET_TG_USINE__EXCLUSIONS_H
#define PROJET_TG_USINE__EXCLUSIONS_H
#include "structures.h"
//lecture du fichier d'operations afin de recuperer tous les numeros de sommets
int lire_operation(t_sommet **sommets, char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur d'ouverture operation.txt");
        exit(1);
    }
    int numSommets = 0;
    char ligne[50];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        *sommets = (t_sommet *)realloc(*sommets, (numSommets + 1) * sizeof(t_sommet));
        sscanf(ligne, "%s", (*sommets)[numSommets].nom);
        numSommets++;
    }
    return numSommets;
}
//lecture du fichier exclusions
int lireFichierExclusions(exclusion *exclusions, char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur d'ouverture exclusion.txt");
        exit(1);
    }
    int numExclusions = 0;
    while (fscanf(fichier, "%s %s", exclusions[numExclusions].sommet1, exclusions[numExclusions].sommet2) == 2) { //remplit la structure exclusions
        numExclusions++;
    }
    return numExclusions;
}
//chaque sommet est un char et non un int en cas de modification du fichier texte
int trouvernom(t_sommet *sommets, int numero_sommet, char *nom_sommet) {
    for (int i = 0; i < numero_sommet; i++) {
        if (strcmp(sommets[i].nom, nom_sommet) == 0) {
            return i;
        }
    }
    return -1;
}
//on initialise le graphe des exclusions
void initialiserGraphe(Graphe *graphe, int numSommets) {
    graphe->numSommets = numSommets;
    graphe->matriceAdjacence = (int **)malloc(numSommets * sizeof(int *));//creation de matrice d'adjacence pour visualiser le graphe
    for (int i = 0; i < numSommets; i++) {
        graphe->matriceAdjacence[i] = (int *)malloc(numSommets * sizeof(int));
        for (int j = 0; j < numSommets; j++) {
            graphe->matriceAdjacence[i][j] = 0;
        }
    }
    graphe->sommets = (t_sommet *)malloc(numSommets * sizeof(t_sommet));
    graphe->station = (int *)malloc(numSommets * sizeof(int));
}
//le graphe se parcours dans les 2 sens
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
//affihage en stations
void affichagestation_exclusion(Graphe *graphe) {
    FILE *fichier = fopen("resultatExclusions.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de la création du fichier.");
        return;
    }
    printf("\n\t\t\t\t\t\t\tAffichage des stations uniquement contrainte d'exclusion \n\n");
    int nombreStations = 0;
    for (int i = 0; i < graphe->numSommets; i++) {
        if (graphe->station[i] > nombreStations) {
            nombreStations = graphe->station[i];
        }
    }
    for (int s = 1; s <= nombreStations; s++) {
        printf("Station %d contient les operations : \n", s);
        for (int i = 0; i < graphe->numSommets; i++) {
            if (graphe->station[i] == s) {
                printf("%s ", graphe->sommets[i].nom);
            }
        }
        printf("\n");
    }
    for (int s = 1; s <= nombreStations; s++) {
        fprintf(fichier, "%d ", s);
        for (int i = 0; i < graphe->numSommets; i++) {
            if (graphe->station[i] == s) {
                fprintf(fichier, "%s ", graphe->sommets[i].nom);
            }
        }
        fprintf(fichier, "\n");
    }

    fclose(fichier);
}
//calcul du degre de chaque sommet
void calculerDegres(Graphe *graphe, int *degres) {
    for (int i = 0; i < graphe->numSommets; i++) {
        degres[i] = 0;
        for (int j = 0; j < graphe->numSommets; j++) {
            if (graphe->matriceAdjacence[i][j] == 1) {
                degres[i]++; //des que le sommet a une connxion a un autre on ajoute un degre
            }
        }
    }
}
//sous programeme qui intervertit 2 sommets
void echangerSommets(t_sommet *a, t_sommet *b, int *degreA, int *degreB) {
    t_sommet tempSommet = *a;
    *a = *b;
    *b = tempSommet;
    int tempDegre = *degreA;
    *degreA = *degreB;
    *degreB = tempDegre;
}
//code de tri a bulles repris du cours ing1 semestre 2
t_sommet* trie_a_bulle_degres(t_sommet *sommets, int *degres, int numSommets) {
    // Créer une copie du tableau sommets
    t_sommet *sommetsCopie = (t_sommet *)malloc(numSommets * sizeof(t_sommet));
    for (int i = 0; i < numSommets; i++) {
        sommetsCopie[i] = sommets[i];
    }
    for (int i = 0; i < numSommets - 1; i++) {
        for (int j = 0; j < numSommets - i - 1; j++) {
            if (degres[j] < degres[j + 1]) {
                echangerSommets(&sommetsCopie[j], &sommetsCopie[j + 1], &degres[j], &degres[j + 1]);
            }
        }
    }

    return sommetsCopie;
}
//coloration du graphe d'exclusions
void welsh_powel_coloration(Graphe *graphe, t_sommet *sommetsTrieesParDegre) {
    int couleur = 1;
    FILE *fichier = fopen("resultatColoration.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de la création du fichier.");
        return;
    }
    for (int i = 0; i < graphe->numSommets; i++) {
        int sommetActuel = trouvernom(sommetsTrieesParDegre, graphe->numSommets, graphe->sommets[i].nom);
        graphe->station[sommetActuel] = -1;
        int couleurUtilisee[graphe->numSommets];
        for (int j = 0; j < graphe->numSommets; j++) {
            couleurUtilisee[j] = 0;
        }
        if (graphe->degres[sommetActuel] == 0) {
            graphe->station[sommetActuel] = couleur++;
            continue;  // Passer au sommet suivant
        }

        // Parcourir les voisins du sommet dans l'ordre du tableau trié
        for (int j = 0; j < graphe->numSommets; j++) {
            int voisin = trouvernom(sommetsTrieesParDegre, graphe->numSommets, graphe->sommets[j].nom);
            if (graphe->matriceAdjacence[sommetActuel][voisin] == 1 && graphe->station[voisin] != -1) {
                couleurUtilisee[graphe->station[voisin]] = 1;
            }
        }
        int couleurDisponible;
        for (couleurDisponible = 1; couleurDisponible <= graphe->numSommets; couleurDisponible++) {
            if (couleurUtilisee[couleurDisponible] == 0) {
                break;
            }
        }
        graphe->station[sommetActuel] = couleurDisponible;
        if (couleurDisponible >= couleur) {
            couleur = couleurDisponible + 1;
        }
    }
    for (int i = 0; i < graphe->numSommets; i++) {
        fprintf(fichier, "%s %d\n", graphe->sommets[i].nom, graphe->station[i]);
    }

    fclose(fichier);
}

//repartition e station selon la couleur
void repartition_exclusion(char *fichier_operation, char *fichier_exclusion) {
    Graphe graphe;
    t_sommet *sommets = NULL;
    exclusion exclusions[100];
    int numSommets = lire_operation(&sommets, fichier_operation);
    initialiserGraphe(&graphe, numSommets);
    for (int i = 0; i < numSommets; i++) {
        strcpy(graphe.sommets[i].nom, sommets[i].nom);
    }
    int numExclusions = lireFichierExclusions(exclusions, fichier_exclusion);
    for (int i = 0; i < numExclusions; i++) {
        int indiceSommet1 = trouvernom(sommets, numSommets, exclusions[i].sommet1);
        int indiceSommet2 = trouvernom(sommets, numSommets, exclusions[i].sommet2);
        if (indiceSommet1 != -1 && indiceSommet2 != -1) {
            ajouterArc(&graphe, indiceSommet1, indiceSommet2);
        }
    }
    int degres[numSommets];
    calculerDegres(&graphe, degres);
    t_sommet *sommetsTries = trie_a_bulle_degres(graphe.sommets, degres, numSommets);
    welsh_powel_coloration(&graphe, sommetsTries);
    free(sommetsTries);
    affichagestation_exclusion(&graphe);

}


#endif //PROJET_TG_USINE__EXCLUSIONS_H
//