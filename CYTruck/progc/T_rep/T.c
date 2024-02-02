#include "header.h"

// Fonction pour libérer la mémoire de l'arbre AVL
void libererAVL(NoeudAVL *a){
    if (a != NULL){
        libererAVL(a->gauche);
        libererAVL(a->droite);
        free(a->ville);
        free(a);
    }
}

// Fonction pour ajouter une ville à une liste chaînée
Ville *ajouterVille(Ville *liste, char nom[50], int passage, int depart){
    Ville *nouvelleVille = malloc(sizeof(Ville));
    if(nouvelleVille == NULL){
        perror("Erreur d'allocation mémoire");
        exit(1);
    }
    strcpy(nouvelleVille->nom, nom);
    nouvelleVille->passage = passage;
    nouvelleVille->depart = depart;
    nouvelleVille->suivante = liste;
    return nouvelleVille;
}

// Fonction pour créer un nœud AVL
NoeudAVL *creerNoeudAVL(Ville *ville){
    NoeudAVL *noeud = (NoeudAVL *)malloc(sizeof(NoeudAVL));
    if(noeud == NULL){
        perror("Erreur d'allocation mémoire");
        exit(1);
    }

    noeud->ville = ville;
    noeud->gauche = NULL;
    noeud->droite = NULL;
    noeud->equilibre = 0;

    return noeud;
}

// Fonction pour insérer une ville dans un AVL
NoeudAVL *insererAVL(NoeudAVL *racine, Ville *ville, int *h){
    if(racine == NULL){
        *h = 1;
        return creerNoeudAVL(ville);
    }

    if(ville->passage < racine->ville->passage){
        racine->gauche = insererAVL(racine->gauche, ville, h);
        *h = -*h;
    }
    else if(ville->passage > racine->ville->passage){
        racine->droite = insererAVL(racine->droite, ville, h);
    }
    else{
        *h = 0;
        return racine;
    }

    if(*h != 0){
        racine->equilibre = racine->equilibre + *h;

        if(racine->equilibre == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }

    return racine;
}

// Fonction pour insérer une ville dans un AVL
NoeudAVL *insererVilleAVL(NoeudAVL *racineAVL, Ville *ville){
    int h = 0;
    return insererAVL(racineAVL, ville, &h);
}

// Fonction pour effectuer un parcours infixe d'un AVL et écrire dans un fichier
void parcoursInfixeAVL(NoeudAVL *a, FILE *fichierSortie, int *compteur){
    if(a != NULL){
        static struct Info topVilles[10];

        parcoursInfixeAVL(a->droite, fichierSortie, compteur);

        if(*compteur < 10 || a->ville->passage > atoi(topVilles[0].passage)) {
            strcpy(topVilles[*compteur].nom, a->ville->nom);
            sprintf(topVilles[*compteur].passage, "%d", a->ville->passage);
            sprintf(topVilles[*compteur].depart, "%d", a->ville->depart);

            qsort(topVilles, *compteur + 1, sizeof(struct Info), compare);

            if(*compteur < 10){
                (*compteur)++;
            }
        }

        parcoursInfixeAVL(a->gauche, fichierSortie, compteur);

        if (*compteur == 10) {
            for (int i = 0; i < 10; i++) {
                fprintf(fichierSortie, "%s %s %s\n", topVilles[i].nom, topVilles[i].passage, topVilles[i].depart);
            }
            *compteur = 0;
        }
    }
}

// Fonction de comparaison pour le tri
int compare(const void *a, const void *b){
    return strcmp(((struct Info *)a)->nom, ((struct Info *)b)->nom);
}

// Fonction pour créer le fichier de fusion
void creerFichierFusion(){
    FILE *fichier1, *fichier2, *fichierSortie;
    struct Info *infos = NULL;
    int nombreDeLignes = 0;

    fichier1 = fopen("../../city_counts.txt", "r");
    fichier2 = fopen("../../start_counts.txt", "r");

    if(fichier1 == NULL || fichier2 == NULL){
        perror("Erreur lors de l'ouverture des fichiers");
        exit(1);
    }

    infos = malloc(sizeof(struct Info));
    if (infos == NULL){
        perror("Erreur lors de l'allocation de mémoire");
        fclose(fichier1);
        fclose(fichier2);
        exit(1);
    }

    while(fscanf(fichier1, "%s %s", infos[nombreDeLignes].nom, infos[nombreDeLignes].passage) == 2){
        strcpy(infos[nombreDeLignes].depart, "");
        nombreDeLignes++;

        infos = realloc(infos, (nombreDeLignes + 1) * sizeof(struct Info));
        if(infos == NULL){
            perror("Erreur lors de la réallocation de la mémoire");
            fclose(fichier1);
            fclose(fichier2);
            exit(1);
        }
    }

    while(fscanf(fichier2, "%s %s", infos[nombreDeLignes].nom, infos[nombreDeLignes].depart) == 2){
        int i;
        for(i = 0; i < nombreDeLignes; i++){
            if(strcmp(infos[i].nom, infos[nombreDeLignes].nom) == 0){
                break;
            }
        }

        if(i < nombreDeLignes){
            strcpy(infos[i].depart, infos[nombreDeLignes].depart);
        }
        else{
            strcpy(infos[nombreDeLignes].passage, "");
            nombreDeLignes++;

            infos = realloc(infos, (nombreDeLignes + 1) * sizeof(struct Info));
            if(infos == NULL){
                perror("Erreur lors de la réallocation de la mémoire");
                fclose(fichier1);
                fclose(fichier2);
                exit(1);
            }
        }
    }

    qsort(infos, nombreDeLignes, sizeof(struct Info), compare);

    fichierSortie = fopen("../../fusion.txt", "w");
    if(fichierSortie == NULL){
        perror("Erreur lors de l'ouverture du fichier de sortie");
        free(infos);
        fclose(fichier1);
        fclose(fichier2);
        exit(1);
    }

    for(int i = 0; i < nombreDeLignes; i++){
        fprintf(fichierSortie, "%s %s %s\n", infos[i].nom, infos[i].passage, infos[i].depart);
    }

    free(infos);

    fclose(fichier1);
    fclose(fichier2);
    fclose(fichierSortie);
}

// Fonction pour créer le fichier de traitement T
void creerFichierTraitementT(){
    FILE *fichierTraitement;
    Ville *listeVilles = NULL;
    NoeudAVL *racineAVL = NULL;

    FILE *fichier = fopen("../../fusion.txt", "r");
    if(fichier == NULL){
        perror("Erreur lors de l'ouverture du fichier de fusion");
        exit(1);
    }

    while(!feof(fichier)){
        char nom[50];
        int passage, depart;

        if(fscanf(fichier, "%s %d %d", nom, &passage, &depart) == EOF){
            break;
        }

        listeVilles = ajouterVille(listeVilles, nom, passage, depart);
    }

    fclose(fichier);

    Ville *courante = listeVilles;
    while(courante != NULL){
        racineAVL = insererVilleAVL(racineAVL, courante);
        courante = courante->suivante;
    }

    fichierTraitement = fopen("../../traitement_t.txt", "w");
    if(fichierTraitement == NULL){
        perror("Erreur lors de l'ouverture du fichier de traitement");
        libererAVL(racineAVL);
        exit(1);
    }

    int compteur = 0;
    parcoursInfixeAVL(racineAVL, fichierTraitement, &compteur);

    fclose(fichierTraitement);

    libererAVL(racineAVL);
}


int main() {
    creerFichierFusion();
    creerFichierTraitementT();
    return 0;
}