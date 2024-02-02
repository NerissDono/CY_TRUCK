#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour représenter une ville
typedef struct ville {
    char nom[50];
    int passage;
    int depart;
    struct ville *suivante;
}Ville;

// Structure pour représenter un nœud d'AVL
typedef struct NoeudAVL {
    Ville *ville;
    struct NoeudAVL *gauche;
    struct NoeudAVL *droite;
    int equilibre;
} NoeudAVL;

// Structure pour représenter les informations d'une ville
typedef struct Info {
    char nom[50];
    char passage[50];
    char depart[50];
} Info;

// Fonction pour libérer la mémoire de l'arbre AVL
void libererAVL(NoeudAVL *a);

// Fonction pour ajouter une ville à une liste chaînée
Ville *ajouterVille(Ville *liste, char nom[50], int passage, int depart);

// Fonction pour créer un nœud AVL
NoeudAVL *creerNoeudAVL(Ville *ville);

// Fonction pour insérer une ville dans un AVL
NoeudAVL *insererAVL(NoeudAVL *racine, Ville *ville, int *h);

// Fonction pour insérer une ville dans un AVL
NoeudAVL *insererVilleAVL(NoeudAVL *racineAVL, Ville *ville);

// Fonction pour effectuer un parcours infixe d'un AVL et écrire dans un fichier
void parcoursInfixeAVL(NoeudAVL *a, FILE *fichierSortie, int *compteur);

// Fonction de comparaison pour le tri
int compare(const void *a, const void *b);

// Fonction pour créer le fichier de fusion
void creerFichierFusion();

// Fonction pour créer le fichier de traitement T
void creerFichierTraitementT();

#endif