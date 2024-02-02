#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

typedef struct Stats {
    int identifiant;
    double min_distance;
    double max_distance;
    double total_distance;
    double difference_max_min;
} Stats;

typedef struct NoeudAVL {
    Stats *noeud_courant;
    struct NoeudAVL *gauche;
    struct NoeudAVL *droite;
    int equilibre;
} NoeudAVL;

NoeudAVL *creerNoeudAVL(Stats *routeId) {
    NoeudAVL *noeud = (NoeudAVL *)malloc(sizeof(NoeudAVL));
    if (noeud == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    noeud->noeud_courant = routeId;
    noeud->gauche = NULL;
    noeud->droite = NULL;
    noeud->equilibre = 0;

    return noeud;
}

NoeudAVL *rotationGauche(NoeudAVL *y) {
    NoeudAVL *x = y->droite;
    NoeudAVL *T2 = x->gauche;

    x->gauche = y;
    y->droite = T2;

    y->equilibre = 0;
    x->equilibre = 0;

    return x;
}

NoeudAVL *rotationDroite(NoeudAVL *x) {
    NoeudAVL *y = x->gauche;
    NoeudAVL *T2 = y->droite;

    y->droite = x;
    x->gauche = T2;

    y->equilibre = 0;
    x->equilibre = 0;

    return y;
}

NoeudAVL *insererAVL(NoeudAVL *racine, Stats *nouvelleRoute, int *h) {
    if (racine == NULL) {
        *h = 1;
        return creerNoeudAVL(nouvelleRoute);
    }

    if (nouvelleRoute->difference_max_min < racine->noeud_courant->difference_max_min) {
        racine->gauche = insererAVL(racine->gauche, nouvelleRoute, h);
        if (*h != 0) {
            racine->equilibre += *h;
            if (racine->equilibre == 2) {
                if (nouvelleRoute->difference_max_min < racine->gauche->noeud_courant->difference_max_min) {
                    racine = rotationDroite(racine);
                    *h = 0;
                } else {
                    racine->gauche = rotationGauche(racine->gauche);
                    racine = rotationDroite(racine);
                    *h = 0;
                }
            }
        }
    } else if (nouvelleRoute->difference_max_min > racine->noeud_courant->difference_max_min) {
        racine->droite = insererAVL(racine->droite, nouvelleRoute, h);
        if (*h != 0) {
            racine->equilibre -= *h;
            if (racine->equilibre == -2) {
                if (nouvelleRoute->difference_max_min > racine->droite->noeud_courant->difference_max_min) {
                    racine = rotationGauche(racine);
                    *h = 0;
                } else {
                    racine->droite = rotationDroite(racine->droite);
                    racine = rotationGauche(racine);
                    *h = 0;
                }
            }
        }
    } else {
        *h = 0;
        return racine;
    }

    return racine;
}

void parcours_decroissant_AVL(NoeudAVL *a, FILE *fichier, int *compteur) {
    if (a != NULL && *compteur < 50) {
        parcours_decroissant_AVL(a->droite, fichier, compteur);

        if (*compteur < 50) {
            (*compteur)++;
            fprintf(fichier, "%d;%d;%.2f;%.2f;%.2f;%.2f\n", (*compteur), a->noeud_courant->identifiant, a->noeud_courant->min_distance, a->noeud_courant->total_distance, a->noeud_courant->max_distance, a->noeud_courant->difference_max_min);
        }

        parcours_decroissant_AVL(a->gauche, fichier, compteur);
    }
}

void libererMemoireAVL(NoeudAVL *racine) {
    if (racine != NULL) {
        libererMemoireAVL(racine->gauche);
        libererMemoireAVL(racine->droite);
        free(racine->noeud_courant);
        free(racine);
    }
}

int main() {
    FILE *fichier_d_entrer = fopen("../../s-filtrer.csv", "r");
    if (fichier_d_entrer == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    FILE *fichier_de_sortie = fopen("../../s-final.csv", "w");
    if (fichier_de_sortie == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    NoeudAVL *racineAVL = NULL;

    char ligne[MAX_LINE_LENGTH];
    while (fgets(ligne, MAX_LINE_LENGTH, fichier_d_entrer) != NULL) {
        Stats *nouvelleRoute = (Stats *)malloc(sizeof(Stats));
        int valeur_analyser = sscanf(ligne, "%d;%lf;%lf;%lf;%lf", &nouvelleRoute->identifiant, &nouvelleRoute->min_distance, &nouvelleRoute->max_distance, &nouvelleRoute->total_distance, &nouvelleRoute->difference_max_min);

        if (valeur_analyser != 5) {
            fprintf(stderr, "Erreur de lecture du fichier. Assurez-vous que le fichier est correctement formaté.\n");
            free(nouvelleRoute);
            continue;  
        }

        int h = 0;
        racineAVL = insererAVL(racineAVL, nouvelleRoute, &h);
    }

    int compteur = 0;
    parcours_decroissant_AVL(racineAVL, fichier_de_sortie, &compteur);

    libererMemoireAVL(racineAVL);

    fclose(fichier_d_entrer);
    fclose(fichier_de_sortie);

    return 0;
}
