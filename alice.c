#include <stdio.h>

int ouvrir_fichiers() {
    FILE *file1, *file2, *file3;

    file1 = fopen("../precedences.txt", "r");
    file2 = fopen("../operations.txt", "r");
    file3 = fopen("../exclusions.txt", "r");


    if (file1 == NULL || file2 == NULL || file3 == NULL) {

        if (file1 != NULL) fclose(file1);
        if (file2 != NULL) fclose(file2);
        if (file3 != NULL) fclose(file3);

        return 1;
    }
    fclose(file1);
    fclose(file2);
    fclose(file3);

    return 0;
}

int main() {
    int resultat = ouvrir_fichiers();
    if (resultat == 0) {
        printf("Pas de pb.\n");
    } else {
        printf("Erreur\n");
    }

    return 0;
}
