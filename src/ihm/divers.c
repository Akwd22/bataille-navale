/**
 * @file divers.c
 * @brief Quelques fonctions utilitaires de gestion de l'affichage
 */

#include <stdio.h>
#include <stdlib.h>
#include "../metier/options.h"

/**
 * @brief Vider la console
 */
void viderConsole(void)
{
    system("clear");
}

/**
 * @brief Afficher la lettre de chaque colonne d'un plateau
 */
void printLettreCol(void)
{
    printf("\t");

    // Affichage des lettres de chaque colonne
    for (int i = 1; i < OPT_L_PLATEAU; i++)
    {
        printf("%c ", 'A' + i - 1);
    }

    printf("\n");
}

/**
 * @brief Afficher le numéro d'une ligne d'un plateau
 * @param n Numéro de la ligne
 */
void printNumLigne(int n)
{
    // Affichage des chiffres de chaque ligne
    printf("\n%d\t", n);
}