#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
    double min_distance;
    double max_distance;
    double total_distance;
    int count;
}Info_Conducteur;

int main() {
    FILE *fichier_d_entrer = fopen("../../s.csv", "r");
    if (fichier_d_entrer == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    char *token;

    // Tableau associatif pour stocker les informations sur les conducteurs (alloué dynamiquement)
    Info_Conducteur *conducteur = NULL;
    int max_route_id = 0;  // Taille maximale du tableau associatif

    while (fgets(line, sizeof(line), fichier_d_entrer) != NULL) {
        // Extraction des informations pertinentes du CSV
        token = strtok(line, ";");
        int route_id = atoi(token);

        // Vérifier et ajuster la taille du tableau associatif si nécessaire
        if (route_id >= max_route_id) {
            int nouvelle_taille = route_id + 1;
            conducteur = realloc(conducteur, nouvelle_taille * sizeof(Info_Conducteur));
            if (conducteur == NULL) {
                perror("Erreur lors de l'allocation de mémoire");
                exit(EXIT_FAILURE);
            }
            for (int i = max_route_id; i < nouvelle_taille; ++i) {
                // Initialiser les nouvelles entrées
                conducteur[i].min_distance = 0.0;
                conducteur[i].max_distance = 0.0;
                conducteur[i].total_distance = 0.0;
                conducteur[i].count = 0;
            }
            max_route_id = nouvelle_taille;
        }

        token = strtok(NULL, ";");
        double distance = atof(token);

        // Mise à jour des informations du conducteur
        if (distance > conducteur[route_id].max_distance || conducteur[route_id].count == 0) {
            conducteur[route_id].max_distance = distance;
        }

        if (distance < conducteur[route_id].min_distance || conducteur[route_id].count == 0) {
            conducteur[route_id].min_distance = distance;
        }

        conducteur[route_id].total_distance += distance;
        conducteur[route_id].count++;
    }

    fclose(fichier_d_entrer);

    // Ouverture du fichier de sortie en écriture
    FILE *fichier_de_sortie = fopen("../../s-filtrer.csv", "w");
    if (fichier_de_sortie == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    // Écriture des résultats dans le fichier de sortie
    for (int i = 1; i < max_route_id; i++) {
        if (conducteur[i].count > 0) {
            double moyenne_distance = conducteur[i].total_distance / conducteur[i].count;
            double difference_max_min = conducteur[i].max_distance - conducteur[i].min_distance;
            fprintf(fichier_de_sortie, "%d;%.2f;%.2f;%.2f;%.2f\n",
                   i, conducteur[i].min_distance, conducteur[i].max_distance, moyenne_distance, difference_max_min);
        }
    }

    // Libération de la mémoire allouée dynamiquement
    free(conducteur);

    // Fermeture du fichier de sortie
    fclose(fichier_de_sortie);

    return 0;
}

