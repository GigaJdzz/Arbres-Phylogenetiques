#ifndef ARBRES_H
#define ARBRES_H

#include <stdio.h>

/* Structure d'un noeud d'arbre */
struct noeud_s;
typedef struct noeud_s noeud;

/* Un arbre binaire est défini comme une référence vers le noeud racine de l'arbre.
 * Un arbre binaire vide est représenté par une référence NULL.
 * Un arbre binaire est une structure de données récursive.
 * S'il n'est pas vide, ses fils sont des arbres.
 */
typedef noeud* arbre;

struct noeud_s {
    char* valeur;
    arbre gauche;
    arbre droit;
};

/* Crée un nouveau nœud et initialise ses champs à null */
noeud* nouveau_noeud(void);

/* Construit un arbre depuis le fichier ouvert f.
 * Fonction récursive (s'appelle elle-même pour la lecture des fils
 * gauche et droit).
 */
arbre lire_arbre(FILE *f);

/* Fonction d'affichage, à remplir */
void affiche_arbre(arbre);

void ajouter_dans_arbre(arbre racine, char *val, char pos1, char pos2);

/* Macros pour de l'affichage uniquement si DEBUG est != 0 */
extern int DEBUG;

#define debug(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, fmt, ##__VA_ARGS__); } while (0)

#endif
