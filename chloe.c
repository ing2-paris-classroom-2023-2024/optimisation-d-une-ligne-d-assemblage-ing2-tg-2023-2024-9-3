#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Sommet {
    int numero;
    float temps_execution;
}Sommet;


typedef struct t_graphe {
    float **matricePonderation;
    Sommet *sommets;
    int nombreSommets;
    float *temps;
}Graphe_pondere;

Graphe_pondere lireGraphe(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    Graphe_pondere graphe;
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier : %s\n", nomFichier);
        exit(0);
    }
    int sommet1;
    int sommet2;
    graphe.nombreSommets = 0;
    int *sommetsRencontres = NULL;

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        if (sommet1 > graphe.nombreSommets) {
            graphe.nombreSommets = sommet1;
        }
        if (sommet2 > graphe.nombreSommets) {
            graphe.nombreSommets = sommet2;
        }

        sommetsRencontres = realloc(sommetsRencontres, (graphe.nombreSommets + 1) * sizeof(int));
        sommetsRencontres[sommet1] = 1;
        sommetsRencontres[sommet2] = 1;
    }

    rewind(fichier);

    graphe.matricePonderation = (float **)malloc((graphe.nombreSommets + 1) * sizeof(float *));
    graphe.sommets = (Sommet *)malloc((graphe.nombreSommets + 1) * sizeof(Sommet));

    for (int i = 0; i <= graphe.nombreSommets; i++) {
        graphe.matricePonderation[i] = (float *)calloc((graphe.nombreSommets + 1), sizeof(float));
        graphe.sommets[i].temps_execution = 0.0;  // Initialisation du temps à 0 par défaut
    }


    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        graphe.matricePonderation[sommet1][sommet2] = 1.0;  // Ou tout autre valeur appropriée
        graphe.sommets[sommet1].numero = sommet1;
        graphe.sommets[sommet2].numero = sommet2;
    }

    free(sommetsRencontres);

    fclose(fichier);
    return graphe;
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

        if (sommet > 0 && sommet <= gr->nombreSommets) {
            gr->sommets[sommet].temps_execution = temps;
        }
    }

    fclose(fichier);
}

void afficherGraphePonderation(Graphe_pondere *graphe) {
    printf("Graphe avec ponderations :\n");
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        for (int j = 1; j <= graphe->nombreSommets; j++) {
            if (graphe->matricePonderation[i][j] != 0.0) {
                printf("(%d -> %d) Temps : %.2f\n", i, j, graphe->sommets[i].temps_execution);
            }
        }
    }
}

void libererGraphe(Graphe_pondere *graphe) {
    for (int i = 0; i <= graphe->nombreSommets; i++) {
        free(graphe->matricePonderation[i]);
    }
    free(graphe->matricePonderation);
    free(graphe->sommets);
}


void compterPredecesseurs(Graphe_pondere *graphe, int *zeroPredecesseur, int *deuxPredecesseurs) {
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        int compteur = 0;
        for (int j = 1; j <= graphe->nombreSommets; j++) {
            if (graphe->matricePonderation[j][i] != 0.0) {
                compteur++;
            }
        }
        if (compteur == 0) {
            zeroPredecesseur[i] = 1;
        } else if (compteur == 2) {
            deuxPredecesseurs[i] = 1;
        }
    }
}

void afficherSommetsPredecesseurs(Graphe_pondere *graphe) {
    int *zeroPredecesseur = (int *)calloc((graphe->nombreSommets + 1), sizeof(int));
    int *deuxPredecesseurs = (int *)calloc((graphe->nombreSommets + 1), sizeof(int));

    compterPredecesseurs(graphe, zeroPredecesseur, deuxPredecesseurs);

    printf("Sommets sans prédécesseur : ");
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        if (zeroPredecesseur[i] == 1) {
            printf("%d ", i);
        }
    }
    printf("\n");

    printf("Sommets avec uniquement deux prédécesseurs : ");
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        if (deuxPredecesseurs[i] == 1) {
            printf("%d ", i);
        }
    }
    printf("\n");

    free(zeroPredecesseur);
    free(deuxPredecesseurs);
}
int liretempscycle(char *nomFichier) {
    int temps;
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier :  %s\n", nomFichier);
        exit(0);
    }
    fscanf(fichier, "%d", &temps) ;
    printf("temps_cycle %d ", temps);
    fclose(fichier);
    return temps ;
}
void rangerDansStationsAvecDuree(Graphe_pondere *graphe) {
    bool *estPlace = (bool *)calloc((graphe->nombreSommets + 1), sizeof(bool));

    int station = 1;
    float dureeActuelle = 0.0;

    printf("Stations :\n");

    while (1) {
        bool sommetPlace = false;
        float dureeStationActuelle = 0.0;

        for (int sommet = 1; sommet <= graphe->nombreSommets; sommet++) {
            if (!estPlace[sommet]) {
                bool peutEtrePlace = true;
                for (int voisin = 1; voisin <= graphe->nombreSommets; voisin++) {
                    if (graphe->matricePonderation[voisin][sommet] != 0.0 && !estPlace[voisin]) {
                        peutEtrePlace = false;
                        break;
                    }
                }

                if (peutEtrePlace && dureeActuelle + graphe->temps[sommet] <= 10.0) {
                    printf("Station %d: Sommet %d (Temps %.2f)\n", station, sommet, graphe->temps[sommet]);

                    dureeActuelle += graphe->temps[sommet];
                    dureeStationActuelle += graphe->temps[sommet];
                    estPlace[sommet] = true;
                    sommetPlace = true;

                    if (dureeActuelle >= 10.0) {
                        station++;
                        dureeActuelle = 0.0;
                    }
                }
            }
        }

        if (!sommetPlace) {
            break;
        }

        if (dureeStationActuelle == 0.0) {
            break;
        }
    }

    free(estPlace);
}

int main() {
    Graphe_pondere graphe = lireGraphe("../precedences.txt");
    lirePonderations("../operations.txt", &graphe);
    afficherGraphePonderation(&graphe);
    afficherSommetsPredecesseurs(&graphe);
    libererGraphe(&graphe);

    return 0;
}

