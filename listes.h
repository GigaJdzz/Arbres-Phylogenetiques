#ifndef _LISTES_H
#define _LISTES_H

#include "arbres.h"

typedef struct liste liste_t;
/* Type de liste à compléter selon votre besoin. */

typedef char* string;

struct cellule {
    string val;
    noeud *n; // valeur noeud
    struct cellule *suivant;
};

typedef struct cellule cellule_t;

struct liste {
    cellule_t *tete;
};

typedef struct liste liste_t;

/* cree une nouvelle liste, initialement vide */
void init_liste_vide(liste_t* L);

/* libère toutes les cellules de la liste */
void liberer_liste(liste_t *L);

////
cellule_t *nouvelleCellule();

/* Ajouter une nouvelle cellule contenant c
 * en tête de la liste L.
 * Si l'ajout est réussi, le résultat est 0,
 * et 1 sinon (échec de l'ajout)
 */
int ajouter_tete(liste_t *L, string c);

int taille_liste(liste_t *liste);

void afficher_compteur_liste(liste_t *liste);

int ajouter_queue_noeud(liste_t *l, noeud *n);

int appartient(liste_t *seq, string carac);

#endif /* _LISTES_H */
