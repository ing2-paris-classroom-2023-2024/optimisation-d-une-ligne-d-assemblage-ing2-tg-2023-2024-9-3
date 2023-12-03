//
// Created by chloe on 03/12/2023.
//

#ifndef PROJET_TG_USINE__PRECEDENCES_EXCLUSIONS_H
#define PROJET_TG_USINE__PRECEDENCES_EXCLUSIONS_H
#include "structures.h"
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
    char sommetsEcrits[100][50] = {""};
    FILE *fichierSortie = fopen(nomFichierSortie, "w");
    if (fichierSortie == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sortie.");
        return;
    }

    for (int i = 0; i < nbSommets; i++) {
        int couleur = sommets[i].couleur;

        if (!sommet_deja_ecrit(sommets[i].nom, sommetsEcrits, nbSommets)) {
            fprintf(fichierSortie, "%d %s\n", sommets[i].couleur, sommets[i].nom);

            for (int j = 0; j < nbSommets; j++) {
                if (sommets[j].couleur == couleur && precedences[i][j] == 1) {
                    if (!sommet_deja_ecrit(sommets[j].nom, sommetsEcrits, nbSommets)) {
                        fprintf(fichierSortie, "%d %s\n", sommets[j].couleur, sommets[j].nom);
                        strcpy(sommetsEcrits[nbSommets], sommets[j].nom);
                    }
                }
            }

            strcpy(sommetsEcrits[nbSommets], sommets[i].nom);
        }
    }

    fclose(fichierSortie);
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

void multicontraintePREDEX(){
    printf("\t\t\t\t\t\t\tRepartition selon contrainte precedence et exclusions\n\n\n");
    afficherSommetsDecouverts("decouverteSommets.txt");
    completerDecouverteAvecCouleur();
    ajouterPrecedences("decouverteSommets.txt","../precedences.txt","fichierResultats.txt");
    ranger_sommets("fichierResultats.txt");
}
#endif //PROJET_TG_USINE__PRECEDENCES_EXCLUSIONS_H
