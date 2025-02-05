#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listes.h"
#include "arbres.h"
#include "arbresphylo.h"

int DEBUG = 0;

int main(int argc, char* argv[]) {
    char *fichier = NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage:  %s [-d] <fichier>\n", argv[0]);
        fprintf(stderr, "\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "\t-d\tmode debug\n");
        exit(1);
    }
    int arg = 1;

    while (arg < argc) {
        if (!strncmp(argv[arg], "-d", 2)) {
            DEBUG = 1;
            arg++;
            continue;
        }
        if (argv[arg][0] == '-') {
            fprintf(stderr, "Option inconnue : '%s'\n", argv[arg]);
            exit(1);
        }
        if (fichier == NULL) {
            fichier = argv[arg];
            arg++;
            continue;
        } else {
            fprintf(stderr, "Trop de fichiers sur la ligne de commande : '%s'\n", argv[arg]);
            exit(1);
        }
    }

    debug("Ouverture de %s\n", fichier);
    FILE* f = fopen(fichier, "r");
    if (!f) {
        fprintf(stderr, "Erreur à l'ouverture du fichier `%s'\n", fichier);
        perror(fichier);
        exit(1);
    }

    arbre mon_arbre = lire_arbre(f);

    int nombre_especes; /* Ne pas initialiser ici, c'est analyse_arbre */
    int nombre_caract;  /* qui doit s'en charger */

    analyse_arbre(mon_arbre, &nombre_especes, &nombre_caract);

    printf("Nombre d'especes de l'arbre: %d\n", nombre_especes);
    printf("Nombre de caractéristiques de l'arbre: %d\n", nombre_caract);
    // FILE *f2 = fopen("test_niveau.txt","w");
    // afficher_par_niveau(mon_arbre, f2);
    // fclose(f);

    liste_t *seq_esp = malloc(sizeof(liste_t));
    init_liste_vide(seq_esp);

    cellule_t *c1 = nouvelleCellule();
    c1->val = "autruche";
    seq_esp->tete = c1;

    cellule_t *c2 = nouvelleCellule();
    c2->val = "autruche";
    c1->suivant = NULL;

    cellule_t *c3 = nouvelleCellule();
    c3->val = "crocodile";
    c2->suivant = c3;

    cellule_t *c4 = nouvelleCellule();
    c4->val = "carpe";
    c3->suivant = c4;
    c4->suivant = NULL;

    ajouter_carac(&mon_arbre, "venin", c1);
    affiche_arbre(mon_arbre);

    return 0;
}
