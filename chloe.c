#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
struct CheminMaximal {
    int sommet;
    float poids;
    int *chemin;
};


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
typedef struct {
    int sommet;
    int couleur;
    int ordreDecouverte;
} SommetCouleur;
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
    //afficherGraphe(&graphe);
}

Graphe lireGraphe(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    Graphe graphe;
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
    graphe.sommets = (t_sommet *)malloc((graphe.nombreSommets + 1) * sizeof(t_sommet));

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


void lirePonderations(const char *nomFichier, Graphe *gr) {
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

void afficherGraphePonderation(Graphe *graphe) {
    // printf("g oriente pondere :\n");
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        for (int j = 1; j <= graphe->nombreSommets; j++) {
            if (graphe->matricePonderation[i][j] != 0.0) {
                // printf("(%d -> %d) Temps : %.2f\n", i, j, graphe->sommets[i].temps_execution);
            }
        }
    }
}
void afficherSommetsAvecPonderation(Graphe *graphe) {
    // printf("Sommets avec leur ponderation :\n");
    for (int i = 1; i <= graphe->nombreSommets; i++) {
        // printf("t_sommet %d : Temps d'execution %.2f\n", graphe->sommets[i].numero, graphe->sommets[i].temps_execution);
    }
}

void libererGraphe(Graphe *graphe) {
    for (int i = 0; i <= graphe->nombreSommets; i++) {
        free(graphe->matricePonderation[i]);
    }
    free(graphe->matricePonderation);
    free(graphe->sommets);
}


void compterPredecesseurs(Graphe *graphe, int *zeroPredecesseur, int *deuxPredecesseurs) {
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

void afficherSommetsPredecesseurs(Graphe *graphe) {
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

float liretempscycle(const char *nomFichier) {
    float temps;
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier :  %s\n", nomFichier);
        exit(0);
    }
    fscanf(fichier, "%f", &temps) ;
    printf("temps_cycle %f \n", temps);
    fclose(fichier);
    return temps ;
}

struct CheminMaximal cheminpoidsmax(Graphe *graphe, int sommetInitial, int sommetFinal) {
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
            cheminMax.poids += graphe->sommets[predecesseur].temps_execution;
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

void calculerCheminsMaximaux(Graphe *graphe) {
    int *zeroPredecesseur = (int *)calloc((graphe->nombreSommets + 1), sizeof(int));
    int *deuxPredecesseurs = (int *)calloc((graphe->nombreSommets + 1), sizeof(int));

    compterPredecesseurs(graphe, zeroPredecesseur, deuxPredecesseurs);

    for (int sommet = 1; sommet <= graphe->nombreSommets; sommet++) {
        if (deuxPredecesseurs[sommet] == 1) {
            //printf("t_sommet %d  :\n", sommet);

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

            //printf("Chemin de poids maximal pour le t_sommet %d :\n", sommet);
            // printf("Poids total : %.2f\n", cheminMaxi.poids);
            // printf("Chemin : ");
            for (int i = 0; cheminMaxi.chemin[i] != -1; i++) {
                // printf("%d ", cheminMaxi.chemin[i]);
            }
            //printf("\n\n");
        }
    }

    free(zeroPredecesseur);
    free(deuxPredecesseurs);
}


void sauvegarderDernieresAretes(const char *nomFichier, Graphe *graphe) {
    FILE *fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        printf("Erreur lors de la création du fichier : %s\n", nomFichier);
        exit(0);
    }

    t_arete *dernieresAretes = (t_arete *)malloc((graphe->nombreSommets + 1) * sizeof(t_arete));

    for (int i = 1; i <= graphe->nombreSommets; i++) {
        dernieresAretes[i].debut = -1;
        dernieresAretes[i].fin = -1;
    }

    for (int sommet = 1; sommet <= graphe->nombreSommets; sommet++) {
        if (graphe->sommets[sommet].numero != 0) {
            struct CheminMaximal cheminMax = cheminpoidsmax(graphe, 1, sommet);
            int dernierSommet = -1;
            for (int i = 0; cheminMax.chemin[i] != -1; i++) {
                if (dernierSommet != -1) {
                    dernieresAretes[cheminMax.chemin[i]].debut = dernierSommet;
                    dernieresAretes[cheminMax.chemin[i]].fin = cheminMax.chemin[i];
                }
                dernierSommet = cheminMax.chemin[i];
            }
        }
    }

    for (int i = 1; i <= graphe->nombreSommets; i++) {
        if (dernieresAretes[i].debut != -1 && dernieresAretes[i].fin != -1) {
            fprintf(fichier, "%d %d\n", dernieresAretes[i].debut, dernieresAretes[i].fin);
        }
    }

    free(dernieresAretes);
    fclose(fichier);
}

void triDansStations(Graphe *graphe, float tempsCycle) {
    int *couleurs = (int *)calloc((graphe->nombreSommets + 1), sizeof(int));
    float tempsStation = 0.0;
    int stationCourante = 1;
    int *file = (int *)malloc((graphe->nombreSommets + 1) * sizeof(int));
    int debutFile = 0;
    int finFile = 0;

    file[finFile++] = 1;
    couleurs[1] = 1;

    // Tableau pour stocker les sommets dans chaque station
    int stations[graphe->nombreSommets][graphe->nombreSommets];
    int nbSommetsDansStation[graphe->nombreSommets];
    memset(nbSommetsDansStation, 0, sizeof(nbSommetsDansStation));

    while (debutFile != finFile) {
        int sommetActuel = file[debutFile++];
        couleurs[sommetActuel] = 2;

        if (graphe->sommets[sommetActuel].temps_execution + tempsStation > tempsCycle) {
            tempsStation = 0.0;
            stationCourante++;
        }

        // Ajouter le sommet à la station courante
        stations[stationCourante - 1][nbSommetsDansStation[stationCourante - 1]] = sommetActuel;
        nbSommetsDansStation[stationCourante - 1]++;

        tempsStation += graphe->sommets[sommetActuel].temps_execution;

        for (int successeur = 1; successeur <= graphe->nombreSommets; successeur++) {
            if (graphe->matricePonderation[sommetActuel][successeur] != 0.0 && couleurs[successeur] == 0) {
                file[finFile++] = successeur;
                couleurs[successeur] = 1;
            }
        }
    }

    // Affichage des sommets dans chaque station
    for (int i = 0; i < stationCourante; i++) {
        printf("Station %d : ", i + 1);
        for (int j = 0; j < nbSommetsDansStation[i]; j++) {
            printf("%d ", stations[i][j]);
        }
        printf("\n");
    }
    FILE *fichier = fopen("resultatsPredTC.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de la création du fichier.");
        return;
    }

    // Écriture des numéros de station dans la première ligne du fichier
    for (int i = 0; i < stationCourante; i++) {
        fprintf(fichier, "%d ", i + 1);
    }
    fprintf(fichier, "\n");

    // Écriture des sommets dans chaque station dans le fichier
    int maxSommetsDansStation = 0;
    for (int i = 0; i < stationCourante; i++) {
        if (nbSommetsDansStation[i] > maxSommetsDansStation) {
            maxSommetsDansStation = nbSommetsDansStation[i];
        }
    }

    for (int j = 0; j < maxSommetsDansStation; j++) {
        for (int i = 0; i < stationCourante; i++) {
            if (j < nbSommetsDansStation[i]) {
                fprintf(fichier, "%d ", stations[i][j]);
            } else {
                fprintf(fichier, "  "); // Si la station n'a pas de sommet à cette position, écrire un espace
            }
        }
        fprintf(fichier, "\n");
    }
    FILE *fichierDecouverte = fopen("decouverteSommets.txt", "w");
    if (fichierDecouverte == NULL) {
        printf("Erreur lors de la création du fichier decouverteSommets.txt.");
        return;
    }

// Écriture de l'ordre de découverte des sommets dans le fichier
    for (int i = 0; i < graphe->nombreSommets; i++) {
        fprintf(fichierDecouverte, "%d\n", file[i]);
    }

    fclose(fichierDecouverte);
    fclose(fichier);
    free(couleurs);
    free(file);
}

void afficherStationsDepuisFichier() {
    FILE *fichier = fopen("resultatsPredTC.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.");
        return;
    }

    int stations[100][100];
    int maxStation = 0;
    int maxSommet = 0;

    // Lecture des numéros de station depuis la première ligne
    int station;
    while (fscanf(fichier, "%d", &station) == 1) {
        if (station > maxStation) {
            maxStation = station;
        }
        fscanf(fichier, " ");
    }

    // Lecture des données du fichier dans un tableau à deux dimensions
    rewind(fichier); // Retour au début du fichier
    for (int i = 0; i < maxStation; i++) {
        for (int j = 0; j < 100; j++) {
            if (fscanf(fichier, "%d", &stations[i][j]) != 1) {
                break;
            }
            if (stations[i][j] > maxSommet) {
                maxSommet = stations[i][j];
            }
            fscanf(fichier, " ");
        }
    }

    fclose(fichier);

    // Affichage des stations et de leurs sommets correspondants
    for (int i = 0; i < maxStation; i++) {
        int nbSommetsDansStation = 0; // Compteur pour les sommets de la station i
        printf("Station %d: ", i + 1);
        for (int j = 0; j < maxSommet; j++) {
            if (stations[i][j] == 1) {
                printf("%d ", j);
                nbSommetsDansStation++; // Incrémentation du compteur de sommets
            }
        }
        printf("\n");
        if (nbSommetsDansStation == 0) {
            break; // Si aucun sommet dans la station, arrêter l'affichage
        }
    }
}
void lireStationsDepuisFichier(int stations[][100], int *maxStation, unsigned char *maxSommet, int nbSommetsDansStation[]) {
    FILE *fichier = fopen("resultatsPredTC.txt", "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.");
        return;
    }

    unsigned char sommet;
    int currentStation = 0;

    // Lecture des données du fichier
    while (fscanf(fichier, "%s", &sommet) == 1) {
        if (sommet > *maxSommet) {
            *maxSommet = sommet;
        }
        stations[currentStation][nbSommetsDansStation[currentStation]] = sommet;
        nbSommetsDansStation[currentStation]++;
        fscanf(fichier, " ");

        if (feof(fichier)) {
            (*maxStation)++; // Nouvelle station détectée
            currentStation = *maxStation; // Passage à la nouvelle station
        }
    }

    fclose(fichier);
}

void lireEtAfficherColoration() {
    FILE *fichierColoration = fopen("resultatColoration.txt", "r");
    FILE *fichierDecouverte = fopen("decouverteSommets.txt", "r");

    if (fichierColoration == NULL || fichierDecouverte == NULL) {
        printf("Erreur lors de l'ouverture du fichier.");
        return;
    }

    int nombreSommets;
    fscanf(fichierColoration, "%d", &nombreSommets);

    int numSommet;
    int couleur;
    int nombreStations = 0;

    // Tableau pour stocker l'ordre de découverte des sommets
    int ordreDecouverte[nombreSommets];
    for (int i = 0; i < nombreSommets; i++) {
        fscanf(fichierDecouverte, "%d\n", &ordreDecouverte[i]);
    }

    // Tableau pour stocker les stations
    int stations[nombreSommets][30];
    int nbSommetsDansStation[nombreSommets];
    for (int i = 0; i < nombreSommets; i++) {
        nbSommetsDansStation[i] = 0;
    }

    // Mettre tous les sommets dans la station 1
    for (int k = 0; k < nombreSommets; k++) {
        stations[0][nbSommetsDansStation[0]] = ordreDecouverte[k];
        nbSommetsDansStation[0]++;
    }

    // Parcourir la station 1 et créer de nouvelles stations si les couleurs diffèrent
    for (int i = 0; i < nbSommetsDansStation[0] - 1; i++) {
        int sommetActuel = stations[0][i];
        int sommetSuivant = stations[0][i + 1];

        while (fscanf(fichierColoration, "%d %d", &numSommet, &couleur) != EOF) {
            if ((sommetActuel == numSommet) && (sommetSuivant == ordreDecouverte[i + 1])) {
                // Même couleur, continuez la lecture
                break;
            } else {
                // Couleur différente, créez une nouvelle station
                nombreStations++;
                nbSommetsDansStation[nombreStations] = 0;
                stations[nombreStations][nbSommetsDansStation[nombreStations]] = ordreDecouverte[i + 1];
                nbSommetsDansStation[nombreStations]++;
                break;
            }
        }
    }

    // Affichage des stations
    for (int i = 0; i <= nombreStations; i++) {
        printf("Station %d : ", i + 1);
        for (int j = 0; j < nbSommetsDansStation[i]; j++) {
            printf("%d ", stations[i][j]);
        }
        printf("\n");
    }

    fclose(fichierColoration);
    fclose(fichierDecouverte);
}

void completerDecouverteAvecCouleur() {
    FILE *fichierColoration = fopen("resultatColoration.txt", "r");
    FILE *fichierDecouverte = fopen("decouverteSommets.txt", "r+");
    FILE *fichierTemp = fopen("temp.txt", "w");

    if (fichierColoration == NULL || fichierDecouverte == NULL || fichierTemp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.");
        return;
    }

    int numSommet;
    int couleur;

    // Lire et traiter toutes les lignes du fichier de découverte
    while (fscanf(fichierDecouverte, "%d", &numSommet) != EOF) {
        int couleurDecouverte;

        // Copier le numéro de sommet dans le fichier temporaire
        fprintf(fichierTemp, "%d", numSommet);

        // Si ce n'est pas la première ligne, lisez la couleur correspondant au numéro de sommet depuis le fichier de coloration
        if (numSommet != 0) {
            fscanf(fichierColoration, "%d %d", &numSommet, &couleur);
        } else {
            // Si c'est la première ligne, réinitialisez la position du fichier de coloration
            fseek(fichierColoration, 0, SEEK_SET);
            fscanf(fichierColoration, "%d %d", &numSommet, &couleur);
        }

        // Écrire la couleur lue (ou celle de la première ligne) dans le fichier temporaire
        fprintf(fichierTemp, " %d\n", couleur);
    }

    // Fermeture des fichiers
    fclose(fichierColoration);
    fclose(fichierDecouverte);
    fclose(fichierTemp);

    // Remplacer le fichier de découverte par le fichier temporaire mis à jour
    remove("decouverteSommets.txt");
    rename("temp.txt", "decouverteSommets.txt");
}


typedef struct {
    char nom[50];
    int couleur;
    int visite; // Pour vérifier si un sommet a été traité
} Sommet;


int sommet_deja_ecrit(char sommet[50], char sommetsEcrits[100][50], int nombreSommetsEcrits) {
    for (int i = 0; i < nombreSommetsEcrits; i++) {
        if (strcmp(sommet, sommetsEcrits[i]) == 0) {
            return 1;  // Sommet déjà écrit
        }
    }
    return 0;  // Sommet non encore écrit
}

void ajouterPrecedences(char *nomFichierDecouverte, char *nomFichierPrecedences, char *nomFichierSortie) {
    FILE *fichierDecouverte = fopen(nomFichierDecouverte, "r");
    if (fichierDecouverte == NULL) {
        printf("Erreur lors de l'ouverture du fichier de découverte.");
        return;
    }

    Sommet sommets[100];
    int nbSommets = 0;

    while (fscanf(fichierDecouverte, "%s %d", sommets[nbSommets].nom, &sommets[nbSommets].couleur) != EOF) {
        nbSommets++;
    }
    fclose(fichierDecouverte);

    FILE *fichierPrecedences = fopen(nomFichierPrecedences, "r");
    if (fichierPrecedences == NULL) {
        printf("Erreur lors de l'ouverture du fichier de précédences.");
        return;
    }

    int precedences[100][100] = {0};

    char sommet1[50], sommet2[50];
    while (fscanf(fichierPrecedences, "%s %s", sommet1, sommet2) != EOF) {
        int indexSommet1 = -1, indexSommet2 = -1;
        for (int i = 0; i < nbSommets; i++) {
            if (strcmp(sommet1, sommets[i].nom) == 0) {
                indexSommet1 = i;
            } else if (strcmp(sommet2, sommets[i].nom) == 0) {
                indexSommet2 = i;
            }
        }
        if (indexSommet1 != -1 && indexSommet2 != -1) {
            precedences[indexSommet1][indexSommet2] = 1;
        }
    }
    fclose(fichierPrecedences);

    FILE *fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sortie.");
        return;
    }

    // Tableau pour stocker les sommets déjà écrits dans le fichier de sortie
    char sommetsEcrits[100][50] = {""};

    for (int i = 0; i < nbSommets; i++) {
        int couleur = sommets[i].couleur;

        if (!sommet_deja_ecrit(sommets[i].nom, sommetsEcrits, nbSommets)) {
            fprintf(fichierSortie, "%d %s", sommets[i].couleur, sommets[i].nom);

            for (int j = 0; j < nbSommets; j++) {
                if (sommets[j].couleur == couleur && precedences[i][j] == 1) {
                    if (!sommet_deja_ecrit(sommets[j].nom, sommetsEcrits, nbSommets)) {
                        fprintf(fichierSortie, " %s", sommets[j].nom);
                        strcpy(sommetsEcrits[nbSommets], sommets[j].nom);
                    }
                }
            }

            fprintf(fichierSortie, "\n");

            // Ajouter le sommet à la liste des sommets écrits
            strcpy(sommetsEcrits[nbSommets], sommets[i].nom);
        }
    }

    fclose(fichierSortie);
}

void rangerEnStations(char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.");
        return;
    }

    Sommet sommets[100];
    int nbStations = 0;
    int nbSommetsDansStation[100] = {0};

    // Lecture du premier sommet
    fscanf(fichier, "%s %d", sommets[0].nom, &sommets[0].couleur);
    nbSommetsDansStation[0]++;

    for (int i = 1; fscanf(fichier, "%s %d", sommets[i].nom, &sommets[i].couleur) != EOF; i++) {
        if (sommets[i].couleur != sommets[i - 1].couleur) {
            // Si la couleur du sommet actuel est différente de celle du précédent, créer une nouvelle station
            nbStations++;
        }
        nbSommetsDansStation[nbStations]++;
    }

    // Affichage des stations
    int sommetIndex = 0;
    for (int i = 0; i <= nbStations; i++) {
        printf("Station %d (couleur %d) : ", i + 1, sommets[sommetIndex].couleur);
        for (int j = 0; j < nbSommetsDansStation[i]; j++) {
            printf("%s ", sommets[sommetIndex].nom);
            sommetIndex++;
        }
        printf("\n");
    }

    fclose(fichier);
}
void afficherSommetsDecouverts(char *nomFichier) {
    FILE *fichierDecouverte = fopen(nomFichier, "r");
    if (fichierDecouverte == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.", nomFichier);
        return;
    }

    int sommet;
    int nombreSommets = 0;

    // Compter le nombre de lignes dans le fichier pour déterminer le nombre de sommets
    while (fscanf(fichierDecouverte, "%d", &sommet) == 1) {
        nombreSommets++;
    }

    // Réinitialiser la position du curseur dans le fichier
    fseek(fichierDecouverte, 0, SEEK_SET);

    // Allouer un tableau pour stocker les sommets
    int *sommets = (int *)malloc(nombreSommets * sizeof(int));

    // Lecture des sommets depuis le fichier
    for (int i = 0; i < nombreSommets; i++) {
        if (fscanf(fichierDecouverte, "%d", &sommets[i]) != 1) {
            printf("Erreur lors de la lecture du fichier %s.", nomFichier);
            fclose(fichierDecouverte);
            free(sommets);
            return;
        }
    }

    // Affichage des sommets
    printf("Sommets découverts : ");
    for (int i = 0; i < nombreSommets; i++) {
        printf("%d ", sommets[i]);
    }
    printf("\n");

    fclose(fichierDecouverte);
    free(sommets);
}




void vider_fichier(const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    fclose(fichier);
}

void ranger_sommets(char *fichier_entree) {
    vider_fichier("sommets_places.txt");

    FILE *f_entree = fopen(fichier_entree, "r");
    if (f_entree == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[100];

    int couleur_precedente = -1;
    int station_actuelle = 1;

    int sommet_marque[100] = {0};

    FILE *fichier_sortie = fopen("sommets_places.txt", "a+");
    if (fichier_sortie == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    while (fgets(ligne, 100, f_entree) != NULL) {
        int couleur = -1;
        char sommet[100];

        if (sscanf(ligne, "%d %[^\n]", &couleur, sommet) != 2) {
            fprintf(stderr, "Format de ligne invalide : %s", ligne);
            continue;
        }

        int sommet_index = atoi(sommet);

        int sommet_present = 0;
        fseek(fichier_sortie, 0, SEEK_SET);
        while (fgets(ligne, 100, fichier_sortie) != NULL) {
            char *token = strtok(ligne, " \n");
            while (token != NULL) {
                if (strcmp(token, sommet) == 0) {
                    sommet_present = 1;
                    break;
                }
                token = strtok(NULL, " \n");
            }
            if (sommet_present) {
                break;
            }
        }

        if (!sommet_present) {
            if (couleur != couleur_precedente) {
                if (station_actuelle > 1) {
                    fprintf(fichier_sortie, "\n");
                    printf("\n");
                }
                printf("Station %d: Couleur %d, Sommets %s ", station_actuelle, couleur, sommet);
                fprintf(fichier_sortie, "%s ", sommet);
                couleur_precedente = couleur;
                station_actuelle++;
            } else {
                printf("%s ", sommet);
                fprintf(fichier_sortie, "%s ", sommet);
            }

            sommet_marque[sommet_index] = 1;
        }
    }

    printf("\n");
    fclose(f_entree);
    fclose(fichier_sortie);
}


int precedence_tmpscycle(char*fichierGraphe,char*fichierPonderation,char*nouveauFichier,char*fichierTempsCycle){
    Graphe graphe = lireGraphe(fichierGraphe);
    lirePonderations(fichierPonderation, &graphe);
    afficherSommetsAvecPonderation(&graphe);
    afficherGraphePonderation(&graphe);
    calculerCheminsMaximaux(&graphe);
    sauvegarderDernieresAretes(nouveauFichier, &graphe);
    Graphe nouveaugraphe = lireGraphe(nouveauFichier);
    lirePonderations(fichierPonderation, &nouveaugraphe);
    afficherGraphePonderation(&nouveaugraphe);
    float tempsCycle = liretempscycle(fichierTempsCycle);
    Graphe newGraphe;
    triDansStations(&nouveaugraphe, tempsCycle);
    libererGraphe(&graphe);
    libererGraphe(&nouveaugraphe);
}

void repartition_precedence_tmpscycle(char*fichier_precedences,char*fichier_operations,char*nouveauFichier,char*fichier_TempsCycle){
    printf("\t\t\t\t\t\t\tRepartition selon contrainte precedence et temps de cycle\n\n\n");
    precedence_tmpscycle(fichier_precedences,fichier_operations,nouveauFichier,fichier_TempsCycle);
}
void multicontrainte(){
    printf("\t\t\t\t\t\t\tRepartition selon contrainte precedence et exclusions\n\n\n");

    afficherSommetsDecouverts("decouverteSommets.txt");
    completerDecouverteAvecCouleur();
    ajouterPrecedences("decouverteSommets.txt","../precedences.txt","fichierResultats.txt");
    ranger_sommets("fichierResultats.txt");
    //rangerEnStations("decouverteSommets.txt");
}
int main() {
    char *operation = "../operations.txt";
    char *exclusion = "../exclusions.txt";
    char *temps_cycle = "../temps_cycle.txt";
    char *precedences = "../precedences.txt";
    char *nouveaufichier="../nouveau_fichier.txt";
    repartition_exclusion(operation, exclusion);
    repartition_precedence_tmpscycle(precedences,operation,nouveaufichier,temps_cycle);
    multicontrainte();
    return 0;
}
