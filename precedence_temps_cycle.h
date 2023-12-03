//
// Created by edoua on 02/12/2023.
//

#ifndef PROJET_TG_USINE__PRECEDENCES_TEMPS_CYCLE_H
#define PROJET_TG_USINE__PRECEDENCES_TEMPS_CYCLE_H
#include "structures.h"
//lecture du fichier precedences
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

    graphe.matricePonderation = (float **)malloc((graphe.nombreSommets + 1) * sizeof(float *));//creation d'une matrice avec les precedences des sommets
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

//lecture du fichier operations
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
            gr->sommets[sommet].temps_execution = temps; //ajout du temps de chaque sommet dans la strcuture graphe
        }
    }

    fclose(fichier);
}

void afficherGraphePonderation(Graphe *graphe) {
    // printf("graphe oriente pondere :\n");
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



//sous-programme qui compte les predecesseurs de chaque sommet et qui definit s'ils en ont 0/1/2
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
//ouverture et lecture du temps de cycle
float liretempscycle(const char *nomFichier) {
    float temps;
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier :  %s\n", nomFichier);
        exit(0);
    }
    fscanf(fichier, "%f", &temps) ;
    //printf("temps_cycle %f \n", temps);
    fclose(fichier);
    return temps ;
}
//algorithme de djikstra pour calculer le poids maximal
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
//calcul du poids de chaque chemin
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

//creation du nouveau graphe simplifie avec djikstra, enregistrement de ce graphe dans un fichier texte
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
//repartition en stations
void triDansStations(Graphe *graphe, float tempsCycle) {
    int *couleurs = (int *)calloc((graphe->nombreSommets + 1), sizeof(int));
    float tempsStation = 0.0;
    int stationCourante = 1;
    int *file = (int *)malloc((graphe->nombreSommets + 1) * sizeof(int));
    int debutFile = 0;
    int finFile = 0;

    file[finFile++] = 1;
    couleurs[1] = 1;

    // Tableau de stockage des sommets dans chaque station
    int stations[graphe->nombreSommets][graphe->nombreSommets];
    int nbSommetsDansStation[graphe->nombreSommets];
    memset(nbSommetsDansStation, 0, sizeof(nbSommetsDansStation));

    while (debutFile != finFile) {
        int sommetActuel = file[debutFile++];
        couleurs[sommetActuel] = 2;
//reinitialisation du temps de la station et passage a la station suivante
        if (graphe->sommets[sommetActuel].temps_execution + tempsStation > tempsCycle) {
            tempsStation = 0.0;
            stationCourante++;
        }

        // Ajouter le sommet à la station
        stations[stationCourante - 1][nbSommetsDansStation[stationCourante - 1]] = sommetActuel;
        nbSommetsDansStation[stationCourante - 1]++;

        tempsStation += graphe->sommets[sommetActuel].temps_execution; //ajout du temps du sommet a la station

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
    //ecriture de la repartition dans un fichier texte de resultats (pour faire la multicontrainte)
    FILE *fichier = fopen("resultatsPredTC.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de la création du fichier.");
        return;
    }

    for (int i = 0; i < stationCourante; i++) {
        fprintf(fichier, "%d ", i + 1);
    }
    fprintf(fichier, "\n");

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
//liberation memoire
    fclose(fichierDecouverte);
    fclose(fichier);
    free(couleurs);
    free(file);
}
///sous programme principal de cette partie du code
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
///joli affichage
void repartition_precedence_tmpscycle(char*fichier_precedences,char*fichier_operations,char*nouveauFichier,char*fichier_TempsCycle){
    printf("\t\t\t\t\t\t\tRepartition selon contrainte precedence et temps de cycle\n\n\n");
    precedence_tmpscycle(fichier_precedences,fichier_operations,nouveauFichier,fichier_TempsCycle);
}
#endif //PROJET_TG_USINE__PRECEDENCES_TEMPS_CYCLE_H
