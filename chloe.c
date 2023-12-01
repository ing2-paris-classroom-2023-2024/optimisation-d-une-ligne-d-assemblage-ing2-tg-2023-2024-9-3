#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Sommet {
    int numero;
    float temps_execution;
}Sommet;

typedef struct Graphe_pondere {
    float **matricePonderation;
    Sommet *sommets;
    int nombreSommets;
    float *temps;
}t_graphe;

struct CheminMaximal {
    int sommet;
    float poids;
    int *chemin;
};
typedef struct Edge {
    int start;
    int end;
} Edge;
t_graphe lireGraphe(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    t_graphe graphe;
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
        graphe.sommets[i].temps_execution = 0.0;
    }


    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) == 2) {
        graphe.matricePonderation[sommet1][sommet2] = 1.0;
        graphe.sommets[sommet1].numero = sommet1;
        graphe.sommets[sommet2].numero = sommet2;
    }
    free(sommetsRencontres);
    fclose(fichier);
    return graphe;
}


void lirePonderations(const char *nomFichier, t_graphe *gr) {
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

void afficherGraphePonderation(t_graphe *graphe) {
    printf("Graphe oriente pondere :\n");
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        for (int j = 1; j <= graphe->nombreSommets; j++) {
            if (graphe->matricePonderation[i][j] != 0.0) {
                printf("(%d -> %d) Temps : %.2f\n", i, j, graphe->sommets[i].temps_execution);
            }
        }
    }
}
void afficherSommetsAvecPonderation(t_graphe *graphe) {
    printf("Sommets avec leur ponderation :\n");
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        printf("Sommet %d : Temps d'execution %.2f\n", graphe->sommets[i].numero, graphe->sommets[i].temps_execution);
    }
}

void libererGraphe(t_graphe *graphe) {
    for (int i = 0; i <= graphe->nombreSommets; i++) {
        free(graphe->matricePonderation[i]);
    }
    free(graphe->matricePonderation);
    free(graphe->sommets);
}


void compterPredecesseurs(t_graphe *graphe, int *zeroPredecesseur, int *deuxPredecesseurs) {
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

void afficherSommetsPredecesseurs(t_graphe *graphe) {
    int *zeroPredecesseur = (int *)calloc((graphe->nombreSommets + 1), sizeof(int));
    int *deuxPredecesseurs = (int *)calloc((graphe->nombreSommets + 1), sizeof(int));

    compterPredecesseurs(graphe, zeroPredecesseur, deuxPredecesseurs);

    printf("Sommets sans predecesseur(s) : ");
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        if (zeroPredecesseur[i] == 1) {
            if (graphe->sommets[i].numero != 0) {
                printf("%d ", i);
            }
        }
    }
    printf("\n");

    printf("Sommets avec uniquement deux predecesseurs : ");
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        if (deuxPredecesseurs[i] == 1) {
            if (graphe->sommets[i].numero != 0) {
                printf("%d ", i);
            }
        }
    }
    printf("\n");

    free(zeroPredecesseur);
    free(deuxPredecesseurs);
}

int liretempscycle(const char *nomFichier) {
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
void rangerDansStationsAvecDuree(t_graphe *graphe) {
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


struct CheminMaximal cheminpoidsmax(t_graphe *graphe, int sommetInitial, int sommetFinal) {
    float *distances = (float *)malloc((graphe->nombreSommets + 1) * sizeof(float));
    int *predecesseurs = (int *)malloc((graphe->nombreSommets + 1) * sizeof(int));


    for (int i = 1; i <= graphe->nombreSommets; i++) {
        distances[i] = 0;
        predecesseurs[i] = -1;
    }
    distances[sommetInitial] = 0;


    while (1) {

        int sommetActuel = -1;
        float maxDistance = -1;
        for (int i = 1; i <= graphe->nombreSommets; i++) {
            if (distances[i] > maxDistance) {
                maxDistance = distances[i];
                sommetActuel = i;
            }
        }

        if (sommetActuel == -1 || sommetActuel == sommetFinal) {
            break;
        }


        for (int voisin = 1; voisin <= graphe->nombreSommets; voisin++) {
            if (graphe->matricePonderation[sommetActuel][voisin] != 0.0) {
                float nouvelleDistance = distances[sommetActuel] + graphe->matricePonderation[sommetActuel][voisin];
                if (nouvelleDistance > distances[voisin]) {
                    distances[voisin] = nouvelleDistance;
                    predecesseurs[voisin] = sommetActuel;
                }
            }
        }

        distances[sommetActuel] = -1;
    }


    struct CheminMaximal cheminMax;
    cheminMax.sommet = sommetFinal;
    cheminMax.poids = 0.0;


    int nombreSommetsCheminMax = 0;
    int sommet = sommetFinal;
    cheminMax.poids=graphe->sommets[sommet].temps_execution;

    while (sommet != -1) {
        int predecesseur = predecesseurs[sommet];
        if (predecesseur != -1) {
            cheminMax.poids += graphe->sommets[predecesseur].temps_execution;  // Add the weight of the edge
        }
        sommet = predecesseur;
        nombreSommetsCheminMax++;
    }

    cheminMax.chemin = (int *)malloc((nombreSommetsCheminMax + 1) * sizeof(int));
    sommet = sommetFinal;
    for (int i = nombreSommetsCheminMax - 1; i >= 0; i--) {
        cheminMax.chemin[i] = sommet;
        sommet = predecesseurs[sommet];
    }


    cheminMax.chemin[nombreSommetsCheminMax] = -1;

    free(distances);
    free(predecesseurs);

    return cheminMax;
}

void calculerCheminsMaximaux(t_graphe *graphe) {
    int *zeroPredecesseur = (int *)calloc((graphe->nombreSommets + 1), sizeof(int));
    int *deuxPredecesseurs = (int *)calloc((graphe->nombreSommets + 1), sizeof(int));

    compterPredecesseurs(graphe, zeroPredecesseur, deuxPredecesseurs);

    for (int sommet = 1; sommet <= graphe->nombreSommets; sommet++) {
        if (deuxPredecesseurs[sommet] == 1) {
            printf("Sommet %d  :\n", sommet);

            struct CheminMaximal cheminMaxi;
            cheminMaxi.poids = -1.0;

            for (int predecesseur = 1; predecesseur <= graphe->nombreSommets; predecesseur++) {
                if (zeroPredecesseur[predecesseur] == 1) {
                    struct CheminMaximal cheminMax = cheminpoidsmax(graphe, predecesseur, sommet);

                    if (cheminMax.poids > cheminMaxi.poids) {
                        cheminMaxi = cheminMax;
                    }
                }
            }

            // Afficher le chemin de poids maximal pour ce sommet
            printf("Chemin de poids maximal pour le sommet %d :\n", sommet);
            printf("Poids total : %.2f\n", cheminMaxi.poids);
            printf("Chemin : ");
            for (int i = 0; cheminMaxi.chemin[i] != -1; i++) {
                printf("%d ", cheminMaxi.chemin[i]);
            }
            printf("\n\n");
        }
    }

    free(zeroPredecesseur);
    free(deuxPredecesseurs);
}


void sauvegarderDernieresAretes(const char *nomFichier, t_graphe *graphe) {
    FILE *fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        printf("Erreur lors de la création du fichier : %s\n", nomFichier);
        exit(0);
    }

    Edge *dernieresAretes = (Edge *)malloc((graphe->nombreSommets + 1) * sizeof(Edge));

    // Initialiser toutes les arêtes à -1
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        dernieresAretes[i].start = -1;
        dernieresAretes[i].end = -1;
    }

    for (int sommet = 1; sommet <= graphe->nombreSommets; sommet++) {
        if (graphe->sommets[sommet].numero != 0) {
            struct CheminMaximal cheminMax = cheminpoidsmax(graphe, 1, sommet);
            int dernierSommet = -1;
            for (int i = 0; cheminMax.chemin[i] != -1; i++) {
                if (dernierSommet != -1) {
                    dernieresAretes[cheminMax.chemin[i]].start = dernierSommet;
                    dernieresAretes[cheminMax.chemin[i]].end = cheminMax.chemin[i];
                }
                dernierSommet = cheminMax.chemin[i];
            }
        }
    }

    // Écrire les dernières arêtes dans le fichier
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        if (dernieresAretes[i].start != -1 && dernieresAretes[i].end != -1) {
            fprintf(fichier, "%d %d\n", dernieresAretes[i].start, dernieresAretes[i].end);
        }
    }

    free(dernieresAretes);
    fclose(fichier);
}

int main() {
    const char *fichierGraphe = "../precedences.txt";
    const char *fichierPonderation = "../operations.txt";
    const char *nouveauFichier="../nouveau_fichier.txt";
    const char *fichierTempsCycle="../temps_cyle.txt";

    t_graphe graphe = lireGraphe(fichierGraphe);
    lirePonderations(fichierPonderation, &graphe);
    afficherSommetsAvecPonderation(&graphe);
    afficherGraphePonderation(&graphe);
    calculerCheminsMaximaux(&graphe);
    sauvegarderDernieresAretes(nouveauFichier, &graphe);
    t_graphe nouveaugraphe =lireGraphe(nouveauFichier);
    lirePonderations(fichierPonderation,&nouveaugraphe);
    afficherGraphePonderation(&nouveaugraphe);
    liretempscycle(fichierTempsCycle);
    libererGraphe(&graphe);

    return 0;
}