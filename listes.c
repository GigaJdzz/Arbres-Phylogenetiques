#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "listes.h"
#include "arbres.h"

/* fichier à compléter au besoin */

void init_liste_vide(liste_t* L) { //O(1)
    L->tete = NULL;
}

void liberer_liste(liste_t* L) { //O(n)
    cellule_t *c = L->tete;
    while (c != NULL) {
        L->tete = c->suivant;
        free(c);
        c = L->tete;
    }
}

cellule_t *nouvelleCellule() {
    cellule_t *new_cel = malloc(sizeof(cellule_t));
    new_cel->suivant = NULL;
    new_cel->val = malloc(256 * sizeof(char));
    new_cel->n = NULL;
    return new_cel;
}

int ajouter_tete(liste_t* L, string c) { //O(n)
    if (c == NULL) {
        return 1;
    }

    cellule_t *cel = nouvelleCellule();
    if (cel == NULL) {
        return 1;
    }

    if (cel->val == NULL) {
        free(cel);
        return 1;
    }

    strcpy(cel->val, c);

    cel->suivant = L->tete;
    L->tete = cel;
    return 0;
}

int taille_liste(liste_t *liste) {
    cellule_t *tmp = liste->tete;
    int n = 0;
    while (tmp != NULL) {
        n++;
        tmp = tmp->suivant;
    }
    return n;
}

void afficherListe(liste_t *liste) {
    cellule_t *courant = liste->tete;

    printf("Contenu de la liste : \n");
    while (courant != NULL) {
        printf("%s", courant->val);
        if (courant->suivant != NULL) {
            printf(" -> ");
        }
        courant = courant->suivant;
    }
    printf("\n");
}

int ajouter_queue_noeud(liste_t *l, noeud *noeudd) {
    if (l == NULL) {
        return 1;
    }
    cellule_t *tmp = l->tete;
    cellule_t *new_cel = nouvelleCellule();
    new_cel->suivant = NULL;
    new_cel->n = noeudd;
    if (tmp == NULL) {
        l->tete = new_cel;
        return 0;
    }
    while (tmp->suivant != NULL) {
        tmp = tmp->suivant;
    }
    tmp->suivant = new_cel;

    return 0;
}

int appartient(liste_t *seq, string carac) {
    cellule_t *tmp = seq->tete;
    while (tmp != NULL) {
        if (strcmp(carac, tmp->val) == 0) {
            return 1;
        }
        tmp = tmp->suivant;
    }
    return 0;
}
