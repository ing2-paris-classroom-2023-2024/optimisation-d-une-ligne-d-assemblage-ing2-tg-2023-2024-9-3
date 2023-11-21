#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


struct t_graphe {
    float **matricePonderation;
    float *temps;  // Tableau de temps associé à chaque sommet
    int nombreSommets;
};
typedef struct t_graphe Graphe_pondere;
struct SommetDijkstra {
    int sommet;
    float distance;
};

void lireGraphe(const char *nomFichier, Graphe_pondere *graphe) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier : %s\n", nomFichier);
        exit(0);
    }

    int sommet1, sommet2;
    graphe->nombreSommets = 0;  // Initialisation du nombre de sommets

    // Compter le nombre de sommets
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        if (sommet1 > graphe->nombreSommets) {
            graphe->nombreSommets = sommet1;
        }
        if (sommet2 > graphe->nombreSommets) {
            graphe->nombreSommets = sommet2;
        }
    }

    rewind(fichier);

    // Allouer la mémoire pour la matrice de pondération et le tableau de temps
    graphe->matricePonderation = (float **)malloc((graphe->nombreSommets + 1) * sizeof(float *));
    graphe->temps = (float *)malloc((graphe->nombreSommets + 1) * sizeof(float));

    for (int i = 0; i <= graphe->nombreSommets; i++) {
        graphe->matricePonderation[i] = (float *)calloc((graphe->nombreSommets + 1), sizeof(float));
        graphe->temps[i] = 0.0;  // Initialisation du temps à 0 par défaut
    }

    // Remplir la matrice de pondération
    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        // Le graphe est orienté de sommet1 vers sommet2
        graphe->matricePonderation[sommet1][sommet2] = 1.0;  // Ou tout autre valeur appropriée
    }

    fclose(fichier);
}

void lirePonderations(const char *nomFichier, Graphe_pondere *gr) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier : %s\n", nomFichier);
        exit(0);
    }

    int sommet;
    float temps;
    while (fscanf(fichier, "%d %f", &sommet, &temps) == 2) {
        // Assurez-vous que le sommet est valide
        if (sommet > 0 && sommet <= gr->nombreSommets) {
            gr->temps[sommet] = temps;
        }
    }

    fclose(fichier);
}

void afficherGraphePonderation(Graphe_pondere *gr) {
    printf("Graphe avec pondérations :\n");
    for (int i = 1; i <= gr->nombreSommets; i++) {
        for (int j = 1; j <= gr->nombreSommets; j++) {
            if (gr->matricePonderation[i][j] != 0.0) {
                printf("(%d -> %d) Temps : %.2f\n", i, j, gr->temps[i]);
            }
        }
    }
}


// Fonction utilitaire pour initialiser un graphe pondéré
void initialiserGraphe(Graphe_pondere *graphe) {
    graphe->matricePonderation = NULL;
    graphe->temps = NULL;
    graphe->nombreSommets = 0;
}

// Fonction pour libérer la mémoire allouée pour le graphe pondéré
void libererGraphe(Graphe_pondere *graphe) {
    for (int i = 0; i <= graphe->nombreSommets; i++) {
        free(graphe->matricePonderation[i]);
    }
    free(graphe->matricePonderation);
    free(graphe->temps);
}

// Fonction pour trouver le sommet initial (celui qui n'a pas de prédécesseur)
int trouverSommetInitial(Graphe_pondere *graphe) {
    int *inDegrees = (int *)calloc((graphe->nombreSommets + 1), sizeof(int));

    // Calculer le degré entrant de chaque sommet
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        for (int j = 1; j <= graphe->nombreSommets; j++) {
            if (graphe->matricePonderation[i][j] != 0.0) {
                inDegrees[j]++;
            }
        }
    }

    // Trouver le sommet sans prédécesseur
    int sommetInitial = -1;
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        if (inDegrees[i] == 0) {
            sommetInitial = i;
            break;
        }
    }

    free(inDegrees);
    return sommetInitial;
}

struct CheminMinimal {
    int sommet;
    float poids;
    int *chemin;
};
struct CheminMinimal dijkstra(Graphe_pondere *graphe, int sommetInitial, int sommetFinal) {
    float *distances = (float *) malloc((graphe->nombreSommets + 1) * sizeof(float));
    int *predecesseurs = (int *) malloc((graphe->nombreSommets + 1) * sizeof(int));

    // Initialiser les distances et les prédécesseurs
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        distances[i] = INT_MAX;
        predecesseurs[i] = -1;
    }

    distances[sommetInitial] = 0;

    // Algorithme de Dijkstra
    while (1) {
        // Trouver le sommet avec la plus petite distance non traité
        int sommetActuel = -1;
        float minDistance = INT_MAX;
        for (int i = 1; i <= graphe->nombreSommets; i++) {
            if (distances[i] < minDistance && distances[i] != -1) {
                minDistance = distances[i];
                sommetActuel = i;
            }
        }

        if (sommetActuel == -1 || sommetActuel == sommetFinal) {
            break; // Tous les sommets ont été traités ou le sommet final a été atteint
        }

        // Mettre à jour les distances
        for (int voisin = 1; voisin <= graphe->nombreSommets; voisin++) {
            if (graphe->matricePonderation[sommetActuel][voisin] != 0.0) {
                float nouvelleDistance = distances[sommetActuel] + graphe->temps[voisin];
                if (nouvelleDistance < distances[voisin]) {
                    distances[voisin] = nouvelleDistance;
                    predecesseurs[voisin] = sommetActuel;
                }
            }
        }

        distances[sommetActuel] = -1; // Marquer le sommet comme traité
    }

    // Construire le chemin minimal
    struct CheminMinimal chemin;
    chemin.sommet = sommetFinal;
    chemin.poids = distances[sommetFinal];

    // Compter le nombre de sommets dans le chemin
    int nombreSommetsChemin = 0;
    int sommet = sommetFinal;
    while (sommet != -1) {
        sommet = predecesseurs[sommet];
        nombreSommetsChemin++;
    }

    chemin.chemin = (int *) malloc((nombreSommetsChemin + 1) * sizeof(int));
    sommet = sommetFinal;
    for (int i = nombreSommetsChemin - 1; i >= 0; i--) {
        chemin.chemin[i] = sommet;
        sommet = predecesseurs[sommet];
    }

// Ajouter la valeur spéciale à la fin du tableau
    chemin.chemin[nombreSommetsChemin] = -1;

    free(distances);
    free(predecesseurs);

    return chemin;
}
int main() {
    Graphe_pondere graphe;
    initialiserGraphe(&graphe);

    lireGraphe("../precedences.txt", &graphe);
    lirePonderations("../operations.txt", &graphe);
    afficherGraphePonderation(&graphe);
    // Trouver le sommet initial
    int sommetInitial = trouverSommetInitial(&graphe);
    printf("Sommet initial : %d\n", sommetInitial);

    // Trouver les sommets avec deux prédécesseurs
    printf("Sommets avec deux predecesseurs :\n");
    for (int i = 1; i <= graphe.nombreSommets; i++) {
        int predecesseurs = 0;
        for (int j = 1; j <= graphe.nombreSommets; j++) {
            if (graphe.matricePonderation[j][i] != 0.0) {
                predecesseurs++;
            }
        }
        if (predecesseurs == 2) {
            printf("%d\n", i);
        }
    }

    printf("\nChemins de poids minimal :\n");
    for (int i = 1; i <= graphe.nombreSommets; i++) {
        int predecesseurs = 0;
        for (int j = 1; j <= graphe.nombreSommets; j++) {
            if (graphe.matricePonderation[j][i] != 0.0) {
                predecesseurs++;
            }
        }
        if (predecesseurs == 2) {
            struct CheminMinimal chemin = dijkstra(&graphe, sommetInitial, i);
            printf("Chemin minimal pour le sommet %d : ", i);

            // Parcourir le tableau jusqu'à la valeur spéciale (-1)
            int j = 0;
            while (chemin.chemin[j] != -1) {
                printf("%d ", chemin.chemin[j]);
                j++;
            }

            printf(" (Poids : %.2f)\n", chemin.poids);
            free(chemin.chemin);
        }
    }

    // ... (Libération de la mémoire et autres opérations)

    return 0;
}