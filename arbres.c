#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "arbres.h"

noeud* nouveau_noeud(void) {
    noeud *n = (noeud*)malloc(sizeof(noeud));
    assert(n != NULL);
    n->valeur = NULL;
    n->gauche = NULL;
    n->droit = NULL;
    return n;
}

/* buffer pour lire les caractères des espèces sous forme de "mots" (words) */
#define MAX_WORD_SIZE 255
char buffer[MAX_WORD_SIZE + 1];

/* Variable globale qui contient le prochain caractère à traiter */
static char next_char = ' ';

/* Supprime tous les espaces, tabulations, retour à la ligne */
#define GLOB(f) \
    while (isspace(next_char)) { \
        next_char = fgetc(f); \
    }

/* Fonction récursive qui lit un sous-arbre */
/* Appelée une fois à la racine (debut du fichier), puis récursivement
 * pour chaque nœud interne rencontré. */
arbre lire_arbre(FILE *f) {
    arbre racine;

    GLOB(f); /* lit dans next_char le premier caractère non vide */

    if (next_char == '/') {
        next_char = ' '; /* on ne garde pas '/' en mémoire */
        return NULL;
    }

    if (next_char == ')') {
        return NULL;
    }

    if (next_char != '(') {
        fprintf(stderr, "Error while reading binary tree : '(' or ')' expected at position %ld\n", ftell(f));
        exit(1);
    }

    /* On remplit le buffer tant qu'on lit des caractères alphanumériques */
    char *p = buffer; /* début du buffer */
    next_char = ' ';
    GLOB(f);

    do {
        *p = next_char; /* sauvegarde du char courant */
        next_char = fgetc(f);
        p++;
        assert(p < buffer + MAX_WORD_SIZE);
    } while (!isspace(next_char) && next_char != '(' && next_char != ')');
    /* on arrête si le char suivant est un espace ou une parenthèse */
    *p = '\0'; /* on ferme la chaîne de caractères dans le buffer */

    racine = nouveau_noeud();
    racine->valeur = strdup(buffer); /* dupliquer le mot lu */

    GLOB(f);

    if (next_char == ')') {
        next_char = ' '; /* on est sur une feuille, on prépare la lecture du prochain nœud */
    } else {
        racine->gauche = lire_arbre(f); /* appel récursif pour le fils gauche */
        racine->droit = lire_arbre(f); /* idem pour le droit */

        GLOB(f); /* lit jusqu'au ')' fermant */

        if (next_char != ')') {
            fprintf(stderr, "Error while reading binary tree: ')' expected\n");
            exit(1);
        }
        next_char = ' '; /* on ne garde pas la parenthèse en mémoire */
    }
    return racine;
}

void afficher_arbre_rec(arbre a, FILE *f) {
    if (a == NULL) {
        return;
    }
    if (a->gauche != NULL) {
        fprintf(f, "\t\"%s\" -> \"%s\" [label = \"non\"]\n", a->valeur, a->gauche->valeur);
        afficher_arbre_rec(a->gauche, f);
    }
    if (a->droit != NULL) {
        fprintf(f, "\t\"%s\" -> \"%s\" [label = \"oui\"]\n", a->valeur, a->droit->valeur);
        afficher_arbre_rec(a->droit, f);
    }
    if (a->droit == NULL && a->gauche == NULL) {
        fprintf(f, "\t\"%s\"\n", a->valeur);
    }
}

void affiche_arbre(noeud *racine) {
    FILE *f;
    f = fopen("output.dot", "w");
    fprintf(f, "digraph arbre {\n");
    afficher_arbre_rec(racine, f);
    fprintf(f, "}");
    fclose(f);
    printf("File output.dot created. Use:\ndot -Tpng -o arbre.png output.dot\n");
}

// pour ajouter un element dans un arbre
void ajouter_dans_arbre(arbre racine, char *val, char pos1, char pos2) {
    noeud *new_n = nouveau_noeud();
    new_n->valeur = malloc(sizeof(val));
    strcpy(new_n->valeur, val);
    if (racine == NULL) {
        racine = new_n;
        return;
    }
    noeud *tmp;
    if (pos1 == 'd') {
        tmp = racine->droit;
        racine->droit = new_n;
    } else {
        tmp = racine->gauche;
        racine->gauche = new_n;
    }
    if (pos2 == 'd') {
        new_n->droit = tmp;
    } else {
        new_n->gauche = tmp;
    }
    return;
}
