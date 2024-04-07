#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "arbres.h"
#include "arbresphylo.h"
#include "listes.h"

#define TRUE 1
#define FALSE 0

int analyse_arbre_rec(arbre a, int *nb_esp) { 
    if (a == NULL) {
        return 0;
    }
    if (a->gauche == NULL && a->droit == NULL) {
        (*nb_esp)++;
        return 0;
    }
    return 1 + analyse_arbre_rec(a->gauche, nb_esp) + analyse_arbre_rec(a->droit, nb_esp);
}

void analyse_arbre(arbre racine, int* nb_esp, int* nb_carac) {
    assert(nb_esp);
    assert(nb_carac);
    *nb_esp = 0;
    *nb_carac = analyse_arbre_rec(racine, nb_esp);
}

/* ACTE II */
/* Recherche l'espece dans l'arbre. Modifie la liste passée en paramètre pour y mettre les
 * caractéristiques. Retourne 0 si l'espèce a été retrouvée, 1 sinon.
 */
int rechercher_espece_rec(arbre racine, char *espece, liste_t* seq) {
    if (racine == NULL) {
        return 1;
    }
    if (strcmp(racine->valeur, espece) == 0) {
        return 0;
    }
    if (rechercher_espece_rec(racine->gauche, espece, seq) == 0) {
        return 0;
    }
    if (rechercher_espece_rec(racine->droit, espece, seq) == 0) {
        ajouter_tete(seq, racine->valeur); //O(n)
        return 0;
    }
    return 1;
}

int rechercher_espece(arbre racine, char *espece, liste_t* seq) { 
    init_liste_vide(seq);
    if (espece == NULL) {
        return 1;
    }
    return rechercher_espece_rec(racine, espece, seq);
}

/* Doit renvoyer 0 si l'espece a bien ete ajoutee, 1 sinon, et ecrire un 
 * message d'erreur.
 */
/* ACTE III */

int ajouter_espece_rec(arbre* a, char *espece, cellule_t* seq) { //O(n)
    if ((*a) == NULL) {
        (*a) = nouveau_noeud();
        if (seq == NULL) {
            (*a)->valeur = espece;
            return 0;
        } else {
            (*a)->valeur = seq->val;
            return ajouter_espece_rec(&((*a)->droit), espece, seq->suivant);
        }
    }

    if (seq == NULL) {
        return 1;
    }

    if ((*a)->droit == NULL && (*a)->gauche == NULL) {
        if (seq == NULL) {
            printf("Ne peut ajouter %s: possède les mêmes caractères que %s.", espece, (*a)->valeur);
            return 1;
        }
        noeud *new_c = nouveau_noeud();
        new_c->valeur = (*a)->valeur;
        (*a)->valeur = seq->val;
        (*a)->gauche = new_c;

        return ajouter_espece_rec(&((*a)->droit), espece, seq->suivant);
    }

    if (strcmp(seq->val, (*a)->valeur) == 0)
        return ajouter_espece_rec(&((*a)->droit), espece, seq->suivant);
    else
        return ajouter_espece_rec(&((*a)->gauche), espece, seq);
}

int ajouter_espece(arbre* a, char *espece, cellule_t* seq) {
    assert(a);
    assert(espece);
    return ajouter_espece_rec(a, espece, seq);
}

/* Doit afficher la liste des caractéristiques niveau par niveau, de gauche
 * à droite, dans le fichier fout.
 * Appeler la fonction avec fout=stdin pour afficher sur la sortie standard.
 */
void afficher_par_niveau(arbre racine, FILE* fout) {
    if (racine == NULL) {
        return;
    }
    liste_t *file_parents = malloc(sizeof(liste_t));
    liste_t *file_fils = malloc(sizeof(liste_t));
    init_liste_vide(file_parents);
    init_liste_vide(file_fils);
    cellule_t *cel = nouvelleCellule();
    cellule_t *tmp_fils_parents = file_fils->tete;
    ajouter_queue_noeud(file_parents, racine);
    while (file_parents->tete != NULL && file_fils != NULL) {
        cel = file_parents->tete;
        file_parents->tete = file_parents->tete->suivant;
        fprintf(fout, "%s ", cel->n->valeur);
        if (cel->n->gauche != NULL && (cel->n->gauche->gauche != NULL || cel->n->gauche->droit != NULL))
            ajouter_queue_noeud(file_fils, cel->n->gauche);
        if (cel->n->droit != NULL && (cel->n->droit->gauche != NULL || cel->n->droit->droit != NULL))
            ajouter_queue_noeud(file_fils, cel->n->droit);
        if (file_parents->tete == NULL) {
            tmp_fils_parents = file_fils->tete;
            file_fils->tete = file_parents->tete;
            file_parents->tete = tmp_fils_parents;
            fprintf(fout, "\n");
        }
    }
}

// Acte 4

int verifier_liste_especes(arbre a, liste_t *seq, int nb_especes) {
    if (a == NULL)
        return 0;
    if (a->droit == NULL && a->gauche == NULL && appartient(seq, a->valeur)) {
        return 1;
    }
    return verifier_liste_especes(a->gauche, seq, nb_especes) + verifier_liste_especes(a->droit, seq, nb_especes);
}

int ajouter_carac_rec(arbre *a, char *carac, liste_t *seq, int nb_especes) {
    if ((*a) == NULL)
        return 1;
    if ((*a)->droit == NULL && (*a)->gauche == NULL) {
        if (appartient(seq, (*a)->valeur) == 1) {
            return 1;
        }
        return 0;
    }
    int g = ajouter_carac_rec(&((*a)->gauche), carac, seq, nb_especes);
    int d = ajouter_carac_rec(&((*a)->droit), carac, seq, nb_especes);
    if (g == 3 || d == 3)
        return 3;
    if (g == 1 && d == 1)
        return 1;
    if (((g == 0 && d == 1) || (d == 0 && g == 1))) {
        if (d == 1) {
            int tout_appartient = verifier_liste_especes((*a)->droit, seq, nb_especes);
            if (tout_appartient == nb_especes) {
                noeud *new_carac = nouveau_noeud();
                new_carac->valeur = carac;
                new_carac->droit = (*a)->droit;
                (*a)->droit = new_carac;
                return 3;
            }
        }
        if (g == 1) {
            int tout_appartient = verifier_liste_especes((*a)->gauche, seq, nb_especes);
            if (tout_appartient == nb_especes) {
                noeud *new_carac = nouveau_noeud();
                new_carac->valeur = carac;
                new_carac->droit = (*a)->gauche;
                (*a)->gauche = new_carac;
                return 3;
            }
        }
        return 2;
    }
    if ((g == 2 || d == 2)) {
        return 2;
    }
    return 0;
}

int ajouter_carac(arbre *a, char *carac, cellule_t *seq) {
    if ((*a) == NULL) {
        return 0;
    }
    if (seq == NULL) {
        noeud *new_carac = nouveau_noeud();
        new_carac->valeur = malloc(sizeof(carac));
        strcpy(new_carac->valeur, carac);
        new_carac->gauche = (*a);
        (*a) = new_carac;
        return 1;
    }
    liste_t *l_seq = malloc(sizeof(liste_t));
    init_liste_vide(l_seq);
    l_seq->tete = seq;
    int nb_especes = taille_liste(l_seq);
    int res = ajouter_carac_rec(a, carac, l_seq, nb_especes);
    if (res == 1) {
        noeud *new_carac = nouveau_noeud();
        new_carac->valeur = malloc(sizeof(carac));
        strcpy(new_carac->valeur, carac);
        new_carac->droit = (*a);
        (*a) = new_carac;
        return 1;
    }
    if (res == 3)
        return 1;
    printf("Ne peut ajouter <%s>: ne forme pas un sous-arbre.\n", carac);
    return 0;
}
