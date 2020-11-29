/**
 * @file prep.c
 * @brief Gestion de l'affichage de la phase préparation
 */

#include <stdio.h>
#include "../metier/joueur.h"
#include "../metier/ia.h"
#include "divers.h"
#include "combat.h"
#include "prep.h"
#include "../metier/options.h"

/**
 * @brief Afficher l'ensemble de la flotte d'un joueur
 * @param j Joueur concerné
 */
void printFlotte(Joueur *j)
{
    printf("\n== Votre flotte ==");

    // Pour chaque bateau ...
    for (int i = 0; i < OPT_T_FLOTTE; i++)
    {
        Bateau *b = &j->flotte[i];

        printf("\n\n [Bateau n°%i]", i + 1);

        // ... dessiner l'aire du bateau
        for (int h = 0; h < b->h; h++)
        {
            printf("\n\t");

            for (int l = 0; l < b->l; l++)
            {
                printf("#");
            }
        }
    }

    printf("\n");
}

/**
 * @brief Afficher la phase de préparation
 */
void demarrerPreparation(void)
{
    /* Demander placement de la flotte du joueur */
    for (int i = 0; i < OPT_T_FLOTTE; i++)
    {
        // Répéter la demande tant que saisie invalide
        do
        {
            viderConsole();
            printPlateau(pJoueur1->plateau);
            printFlotte(pJoueur1);
        } while (!saisirPoserBateau(i));
    }

    /* Placement aléatoire de la flotte de l'IA */
    ia_placerFlotte();

    /* Démarrer la phase de combat */
    demarrerCombat();
}