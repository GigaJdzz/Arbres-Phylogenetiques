#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listes.h"
#include "arbres.h"
#include "arbresphylo.h"
typedef struct
{
    int nb_caracs;
    int nb_especes;
    char **tab_caracs;
    char **tab_especes;
    int **matrice_c_e;
    int *nb_esp_par_carac;
} tableau;
char **alloc_tabchar_2d(int n, int m)
{
    char **tab = malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
    {
        tab[i] = malloc(m * sizeof(char));
    }
    return tab;
}

int **alloc_tabint_2d(int n, int m)
{
    int **tab = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        tab[i] = malloc(m * sizeof(int));
    }
    return tab;
}

tableau *lire_tableau(char *file)
{
    FILE *f = fopen(file, "r");
    if (f == NULL)
    {
        return NULL;
    }

    tableau *tab = malloc(sizeof(tableau));
    if (tab == NULL)
    {
        fclose(f);
        return NULL;
    }

    int nb_e, nb_c;
    if (fscanf(f, "%d", &nb_e) != 1 || fscanf(f, "%d", &nb_c) != 1)
    {
        free(tab);
        fclose(f);
        return NULL;
    }

    char **tab_c = alloc_tabchar_2d(nb_c, 256);
    char **tab_e = alloc_tabchar_2d(nb_e, 256);
    int **mat = alloc_tabint_2d(nb_e, nb_c);
    int *nb_e_c = malloc(nb_c * sizeof(int));

    if (tab_c == NULL || tab_e == NULL || mat == NULL || nb_e_c == NULL)
    {

        free(tab_c);
        free(tab_e);
        free(mat);
        free(nb_e_c);
        free(tab);
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < nb_e; i++)
    {
        fscanf(f, "%s", tab_e[i]);
    }

    for (int i = 0; i < nb_c; i++)
    {
        fscanf(f, "%s", tab_c[i]);
    }

    for (int i = 0; i < nb_e; i++)
    {
        for (int j = 0; j < nb_c; j++)
        {
            fscanf(f, "%d", &mat[i][j]);
        }
    }

    for (int i = 0; i < nb_c; i++)
    {
        if (fscanf(f, "%d", &nb_e_c[i]) != 1)
        {
            printf("Erreur de lecture: i = %d\n", i);
        }
    }

    fclose(f);

    tab->nb_especes = nb_e;
    tab->nb_caracs = nb_c;
    tab->tab_caracs = tab_c;
    tab->tab_especes = tab_e;
    tab->matrice_c_e = mat;
    tab->nb_esp_par_carac = nb_e_c;

    return tab;
}

void afficher_tableau(tableau *tab){
    if (tab == NULL){
        printf("Le tableau est vide.\n");
        return;
    }
        // Afficher le nombre d'espèces et de caractéristiques
    printf("Nombre d'espèces: %d\n", tab->nb_especes);
    printf("Nombre de caractéristiques: %d\n", tab->nb_caracs);

    // Afficher les espèces
    printf("Espèces:\n");
    for (int i = 0; i < tab->nb_especes; i++){
        printf("%s\n", tab->tab_especes[i]);
    }

    // Afficher les caractéristiques
    printf("Caractéristiques:\n");
    for (int i = 0; i < tab->nb_caracs; i++){
        printf("%s\n", tab->tab_caracs[i]);
    }

    // Afficher la matrice caractéristiques-espèces
    printf("Matrice caractéristiques-espèces:\n");
    for (int i = 0; i < tab->nb_especes; i++){
        for (int j = 0; j < tab->nb_caracs; j++){
            printf("%d ", tab->matrice_c_e[i][j]);
        }
        printf("\n");
    }

    // Afficher le nombre d'espèces par caractéristique
    printf("Nombre d'espèces par caractéristique:\n");
    for (int i = 0; i < tab->nb_caracs; i++){
        printf("%d ", tab->nb_esp_par_carac[i]);
    }
    printf("\n");
}

arbre constarbrerap(tableau*tab)
{
    if(tab->nb_especes==0){
        return NULL;
    }
    arbre a = nouveau_noeud();
    if (tab->nb_caracs == 0)
    {
        if (tab->nb_especes > 1)
        {
            return NULL;
        }
        a->droit = NULL;
        a->gauche = NULL;
        a->valeur = tab->tab_especes[0];
        return a;
    }

    char *z = tab->tab_especes[0];
    printf("%s\n", z);

    tableau* tg=malloc(sizeof(tableau));
    tableau *td = malloc(sizeof(tableau));
    tg->nb_caracs=0;
    tg->matrice_c_e = alloc_tabint_2d(tab->nb_especes, tab->nb_caracs);
    tg->nb_esp_par_carac = malloc(tab->nb_caracs * sizeof(int));
    tg->nb_especes = 0;
    tg->tab_caracs = alloc_tabchar_2d(tab->nb_caracs, 256);
    tg->tab_especes = alloc_tabchar_2d(tab->nb_especes, 256);
    td->nb_caracs = 0;
    td->matrice_c_e = alloc_tabint_2d(tab->nb_especes, tab->nb_caracs);
    td->nb_esp_par_carac = malloc(tab->nb_caracs * sizeof(int));
    td->nb_especes = 0;
    td->tab_caracs = alloc_tabchar_2d(tab->nb_caracs, 256);
    td->tab_especes = alloc_tabchar_2d(tab->nb_especes, 256);

    int *tab_verite_d = malloc(tab->nb_caracs * sizeof(int));
    int *tab_verite_g = malloc(tab->nb_caracs * sizeof(int));

    int plus_eloigne_d = 0;
    int plus_eloigne_g = 0;

    int *nb_eloigne_g = malloc(tab->nb_caracs * sizeof(int));
    int *nb_eloigne_d = malloc(tab->nb_caracs * sizeof(int));

    int tmp;
    int ae = 0;
    int m = -1;
    for (int i = 0; i < tab->nb_caracs; i++)
    {
        if (tab->nb_esp_par_carac[i] > m)
        {
            ae = i;
            m = tab->nb_esp_par_carac[i];
        }
    }
    int g = 0;
    for (int i = 0; i < tab->nb_caracs; i++)
    {
        tg->nb_esp_par_carac[i] = 0;
        td->nb_esp_par_carac[i] = 0;
        tab_verite_g[i] = 0;
        tab_verite_d[i] = 0;
        nb_eloigne_d[i] = 0;
        nb_eloigne_g[i] = 0;
    }
    for (int i = 0; i < tab->nb_especes; i++)
    {
        for (int j = 0; j < tab->nb_caracs; j++){
        tg->matrice_c_e[i][j] = 0;
        td->matrice_c_e[i][j] = 0;  
        }
           
    }
    for (int i = 0; i < tab->nb_especes; i++)
    {
        if (tab->matrice_c_e[i][ae] == 0)
        {
            tg->tab_especes[tg->nb_especes] = tab->tab_especes[i];
            g = 0;
            for (int j = 0; j < tab->nb_caracs; j++)
            {
                if (j != ae && tab_verite_d[j] != 1 && tab->matrice_c_e[i][j] == 1)
                {
                    if (j > plus_eloigne_g){
                        plus_eloigne_g = j;
                        for (int k = j + 1; k < tab->nb_caracs; k++){
                            nb_eloigne_d[k]++;
                        }
                    }
                    if (tab_verite_g[j] == 0)
                    {
                        tg->tab_caracs[tg->nb_caracs]= tab->tab_caracs[j];
                        tg->nb_caracs++;
                        tab_verite_g[j] = 1;
                    }

                    tg->matrice_c_e[tg->nb_especes][tg->nb_caracs - 1 - nb_eloigne_d[j]]=tab->matrice_c_e[i][j];
                    tg->nb_esp_par_carac[tg->nb_caracs - 1]++;
                }
            }
            // if (g > tg->nb_caracs)
            // {
            //     tg->nb_caracs = g;
            // }
            tg->nb_especes++;
        }
        else if (tab->matrice_c_e[i][ae] == 1)
        {
            z = tab->tab_especes[i];
            td->tab_especes[td->nb_especes]= tab->tab_especes[i];
            g = 0;
            for (int j = 0; j < tab->nb_caracs; j++)
            {
                tmp = tab_verite_d[j];
                if (j != ae && tab_verite_g[j] != 1 && tab->matrice_c_e[i][j] == 1)
                {
                    if (j > plus_eloigne_d){
                        plus_eloigne_d = j;
                        for (int k = j + 1; k < tab->nb_caracs; k++){
                            nb_eloigne_g[k]++;
                        }
                    }
                    if (tab_verite_d[j] == 0) 
                    {
                        td->tab_caracs[td->nb_caracs]= tab->tab_caracs[j];
                        td->nb_caracs++;
                        tab_verite_d[j] = 1;
                    }
                    td->matrice_c_e[td->nb_especes][td->nb_caracs - 1 - nb_eloigne_g[j]] = tab->matrice_c_e[i][j];
                    td->nb_esp_par_carac[td->nb_caracs - 1]++;
                }
            }
            // if (g > td->nb_caracs)
            // {
            //     td->nb_caracs = g;
            // }
            td->nb_especes++;
        }
    }
    printf("-----------------TG\n");
    afficher_tableau(tg);
    printf("-----------------TD\n");
    afficher_tableau(td);
    a->droit = constarbrerap(td);
    a->gauche = constarbrerap(tg);
    a->valeur = tab->tab_caracs[ae];
    return a;
}
int main(int argc,char**argv){
    tableau *tab = lire_tableau(argv[1]);
    afficher_tableau(tab);
    arbre a = constarbrerap(tab);
    affiche_arbre(a);
    return 0;
}