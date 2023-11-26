#include <stdio.h>
#include <stdlib.h>

struct t_graphe {
    int **matriceAdjacenceExclusion;
    int **matriceAdjacencePrecedence;
    int nombreSommets;
};
typedef struct t_graphe Graphe;

struct t_station {
    int *operation;
    int nb_operation;
};
typedef struct t_station Station;

int lireNombreOperations(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier :  %s\n", nomFichier);
        exit(0);
    }
    int nombreOperations = 0;
    char ligne[100];

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        nombreOperations++;
    }
    fclose(fichier);
    return nombreOperations;
}

void lireContraintes(const char *nomFichier, int contraintes[][2], int *nombreContraintes) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur de l'ouverture du fichier :  %s\n", nomFichier);
        exit(0);
    }
    *nombreContraintes = 0;
    while (fscanf(fichier, "%d %d", &contraintes[*nombreContraintes][0], &contraintes[*nombreContraintes][1]) == 2) {
        (*nombreContraintes)++;
    }
    fclose(fichier);
}

Graphe creerGrapheExclusionPrecedence(int contraintesExclusion[][2], int nombreContraintesExclusion, int contraintesPrecedence[][2], int nombreContraintesPrecedence) {
    Graphe graphe;
    graphe.nombreSommets = 0;

    for (int i = 0; i < nombreContraintesExclusion; i++) {
        if (contraintesExclusion[i][0] > graphe.nombreSommets) {
            graphe.nombreSommets = contraintesExclusion[i][0];
        }
        if (contraintesExclusion[i][1] > graphe.nombreSommets) {
            graphe.nombreSommets = contraintesExclusion[i][1];
        }
    }

    for (int i = 0; i < nombreContraintesPrecedence; i++) {
        if (contraintesPrecedence[i][0] > graphe.nombreSommets) {
            graphe.nombreSommets = contraintesPrecedence[i][0];
        }
        if (contraintesPrecedence[i][1] > graphe.nombreSommets) {
            graphe.nombreSommets = contraintesPrecedence[i][1];
        }
    }

    graphe.matriceAdjacenceExclusion = (int **)malloc((graphe.nombreSommets + 1) * sizeof(int *));
    graphe.matriceAdjacencePrecedence = (int **)malloc((graphe.nombreSommets + 1) * sizeof(int *));

    for (int i = 0; i <= graphe.nombreSommets; i++) {
        graphe.matriceAdjacenceExclusion[i] = (int *)calloc((graphe.nombreSommets + 1), sizeof(int));
        graphe.matriceAdjacencePrecedence[i] = (int *)calloc((graphe.nombreSommets + 1), sizeof(int));
    }

    for (int i = 0; i < nombreContraintesExclusion; i++) {
        graphe.matriceAdjacenceExclusion[contraintesExclusion[i][0]][contraintesExclusion[i][1]] = 1;
        graphe.matriceAdjacenceExclusion[contraintesExclusion[i][1]][contraintesExclusion[i][0]] = 1;
    }

    for (int i = 0; i < nombreContraintesPrecedence; i++) {
        graphe.matriceAdjacencePrecedence[contraintesPrecedence[i][0]][contraintesPrecedence[i][1]] = 1;
    }

    return graphe;
}

void colorerGraphe(Graphe graphe, Station stations[], int *nombreStations) {
    int *couleurs = (int *)calloc((graphe.nombreSommets + 1), sizeof(int));
    if (couleurs == NULL) {
        printf( "Erreur \n");
        exit(0);
    }
    for (int i = 1; i <= graphe.nombreSommets; i++) {
        couleurs[i] = 0;
    }
    for (int sommet = 1; sommet <= graphe.nombreSommets; sommet++) {
        int couleurDisponible = 1;

        for (int voisin = 1; voisin <= graphe.nombreSommets; voisin++) {
            if (graphe.matriceAdjacencePrecedence[sommet][voisin] && couleurs[voisin] != 0) {
                couleurDisponible = 0;
                break;
            }
        }
        if (couleurDisponible) {
            couleurs[sommet] = 1;
        } else {
            for (int couleur = 2; couleur <= *nombreStations + 1; couleur++) {
                int couleurLibre = 1;
                for (int voisin = 1; voisin <= graphe.nombreSommets; voisin++) {
                    if (graphe.matriceAdjacencePrecedence[sommet][voisin] && couleurs[voisin] == couleur) {
                        couleurLibre = 0;
                        break;
                    }
                }
                if (couleurLibre) {
                    couleurs[sommet] = couleur;
                    break;
                }
            }
        }
        if (couleurs[sommet] > *nombreStations) {
            *nombreStations = couleurs[sommet];
        }
    }
    for (int i = 1; i <= *nombreStations; i++) {
        stations[i].operation = (int *)malloc(graphe.nombreSommets * sizeof(int));
        stations[i].nb_operation = 0;
    }

    for (int sommet = 1; sommet <= graphe.nombreSommets; sommet++) {
        stations[couleurs[sommet]].operation[stations[couleurs[sommet]].nb_operation++] = sommet;
    }
    free(couleurs);
}

void afficherStations(Station stations[], int nombreStations, int nombreOperations) {
    printf("Nombre total d'operations : %d\n", nombreOperations);
    printf("Repartition des stations :\n");
    for (int i = 1; i <= nombreStations; i++) {
        printf("Station %d : \n", i);
        printf("Operations : ");
        for (int j = 0; j < stations[i].nb_operation; j++) {
            printf(" %d ", stations[i].operation[j]);
        }
        printf("\n");
    }

    printf("Lorsqu'on considere les multicontraintes on obtient \n", nombreStations);
}

void repartition_station_exclusion_precedence(char *fichierExclusion, char *fichierPrecedence, char *operations) {
    int nombre_operations = lireNombreOperations(operations);
    int contraintesExclusion[nombre_operations][2];
    int contraintesPrecedence[nombre_operations][2];
    int nombreContraintesExclusion, nombreContraintesPrecedence;

    Graphe graphe;
    Station stations[nombre_operations];
    int nombreStations = 0;

    lireContraintes(fichierExclusion, contraintesExclusion, &nombreContraintesExclusion);
    lireContraintes(fichierPrecedence, contraintesPrecedence, &nombreContraintesPrecedence);

    graphe = creerGrapheExclusionPrecedence(contraintesExclusion, nombreContraintesExclusion, contraintesPrecedence, nombreContraintesPrecedence);

    colorerGraphe(graphe, stations, &nombreStations);
    afficherStations(stations, nombreStations, nombre_operations);
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

int main() {
    char *operation = "../operations.txt";
    char *precedences = "../precedences.txt";
    char *exclusion = "../exclusions.txt";
    char *temps_cycle = "temps_cycle.txt";

    repartition_station_exclusion_precedence(exclusion, precedences, operation);
    liretempscycle(temps_cycle);

    return 0;
}
