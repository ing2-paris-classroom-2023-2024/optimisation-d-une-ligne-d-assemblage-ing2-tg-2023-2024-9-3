#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct CheminMaximal {
    int sommet;
    float poids;
    int *chemin;
};


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
    int nombreSommets;
    int *degres;
    int *station;
};
typedef struct t_graphe Graphe;

typedef struct arete {
    int debut;
    int fin;
} t_arete;

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
        sscanf(ligne, "%s %f", (*sommets)[numSommets].nom, &(*sommets)[numSommets].temps_execution);
        numSommets++;
    }
    return numSommets;
}

int lireFichierExclusions(exclusion *exclusions, char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur d'ouverture exclusion.txt");
        exit(1);
    }
    int numExclusions = 0;
    while (fscanf(fichier, "%s %s", exclusions[numExclusions].sommet1, exclusions[numExclusions].sommet2) == 2) {
        numExclusions++;
    }
    return numExclusions;
}

int lireTempsCycle(char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur d'ouverture temps_cycle.txt");
        exit(1);
    }
    int tempsCycle;
    fscanf(fichier, "%d", &tempsCycle);
    fclose(fichier);
    return tempsCycle;
}

int trouvernom(t_sommet *sommets, int numero_sommet, char *nom_sommet) {
    for (int i = 0; i < numero_sommet; i++) {
        if (strcmp(sommets[i].nom, nom_sommet) == 0) {
            return i;
        }
    }
    return -1;
}

void initialiserGraphe(Graphe *graphe, int numSommets) {
    graphe->numSommets = numSommets;
    graphe->matriceAdjacence = (int **)malloc(numSommets * sizeof(int *));
    for (int i = 0; i < numSommets; i++) {
        graphe->matriceAdjacence[i] = (int *)malloc(numSommets * sizeof(int));
        for (int j = 0; j < numSommets; j++) {
            graphe->matriceAdjacence[i][j] = 0;
        }
    }
    graphe->sommets = (t_sommet *)malloc(numSommets * sizeof(t_sommet));
    graphe->station = (int *)malloc(numSommets * sizeof(int));
}

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

void affichagestation_exclusion(Graphe *graphe) {
    printf("\n\t\t\t\t\t\t\tAffichage des stations uniquement contrainte d'exclusion \n\n");
    int nombreStations = 0;
    for (int i = 0; i < graphe->numSommets; i++) {
        if (graphe->station[i] > nombreStations) {
            nombreStations = graphe->station[i];
        }
    }

    for (int s = 1; s <= nombreStations; s++) {
        printf("Station %d contient les operations : \n", s);
        float tempsTotalStation = 0.0;  // To store the sum of execution times for the current station
        for (int i = 0; i < graphe->numSommets; i++) {
            if (graphe->station[i] == s) {
                printf("%s (%.2f) ", graphe->sommets[i].nom, graphe->sommets[i].temps_execution);
                tempsTotalStation += graphe->sommets[i].temps_execution;
            }
        }
        printf("\nSomme des temps d'execution dans la station %d : %.2f\n", s, tempsTotalStation);
    }
}

void calculerDegres(Graphe *graphe, int *degres) {
    for (int i = 0; i < graphe->numSommets; i++) {
        degres[i] = 0;
        for (int j = 0; j < graphe->numSommets; j++) {
            if (graphe->matriceAdjacence[i][j] == 1) {
                degres[i]++;
            }
        }
    }
}

void echangerSommets(t_sommet *a, t_sommet *b, int *degreA, int *degreB) {
    t_sommet tempSommet = *a;
    *a = *b;
    *b = tempSommet;
    int tempDegre = *degreA;
    *degreA = *degreB;
    *degreB = tempDegre;
}

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
void welsh_powel_coloration(Graphe *graphe, t_sommet *sommetsTrieesParDegre, int tempsCycle) {
    int couleur = 1;

    for (int i = 0; i < graphe->numSommets; i++) {
        int sommetActuel = trouvernom(sommetsTrieesParDegre, graphe->numSommets, graphe->sommets[i].nom);
        graphe->station[sommetActuel] = -1;
        int couleurUtilisee[graphe->numSommets];
        float tempsTotalStation[graphe->numSommets];

        for (int j = 0; j < graphe->numSommets; j++) {
            couleurUtilisee[j] = 0;
            tempsTotalStation[j] = 0.0;
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
                tempsTotalStation[graphe->station[voisin]] += graphe->sommets[voisin].temps_execution;
            }
        }

        int couleurDisponible;

        for (couleurDisponible = 1; couleurDisponible <= graphe->numSommets; couleurDisponible++) {
            if (couleurUtilisee[couleurDisponible] == 0) {
                // Check if adding the current operation exceeds the cycle time

                break;

            }
        }
        if (tempsTotalStation[couleurDisponible] + graphe->sommets[sommetActuel].temps_execution <= tempsCycle){
            graphe->station[sommetActuel] = couleurDisponible;}

        if (couleurDisponible >= couleur) {
            couleur = couleurDisponible + 1;
        }
    }
}

void repartition_exclusion(char *fichier_operation, char *fichier_exclusion, int tempsCycle) {
    Graphe graphe;
    t_sommet *sommets = NULL;
    exclusion exclusions[100];
    int numSommets = lire_operation(&sommets, fichier_operation);
    initialiserGraphe(&graphe, numSommets);
    for (int i = 0; i < numSommets; i++) {
        strcpy(graphe.sommets[i].nom, sommets[i].nom);
        graphe.sommets[i].temps_execution = sommets[i].temps_execution;
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
    welsh_powel_coloration(&graphe, sommetsTries, tempsCycle);
    free(sommetsTries);
    affichagestation_exclusion(&graphe);
    //afficherGraphe(&graphe);
}
int main() {
    char *operation = "../operations.txt";
    char *exclusion = "../exclusions.txt";
    char *temps_cycle = "../temps_cycle.txt";
    char *precedences = "../precedences.txt";
    char *nouveaufichier="../nouveau_fichier.txt";
    int tempsCycle = lireTempsCycle(temps_cycle);
    repartition_exclusion(operation, exclusion, tempsCycle);
    return 0;
}
