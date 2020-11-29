/**
 * @file bateau.c
 * @brief Toutes les fonctions relatives aux bateaux
 */

#include <stdio.h>
#include "plateau.h"
#include "bateau.h"

///////////////
// Fonctions //
///////////////

/**
 * @brief Retourner le nombre de cases occupées par un bateau
 * 
 * @param bat Bateau concerné
 * @return int Nombre de cases occupées
 */
int getNbCasesOccupees(Bateau *bat)
{
    int i = 0;
    Case *p = bat->cases[0];

    // Compter le nombre de cases jusqu'à fin du tableau
    while (p != NULL)
        p = bat->cases[i++];

    return i - 1;
}

/**
 * @brief Rotationner un bateau de 90°
 * @param bat Bateau concerné
 */
void rotationner(Bateau *bat)
{
    UBYTE _ = bat->h;
    bat->h = bat->l;
    bat->l = _;
}

/**
 * @brief Retourner si le bateau est entièrement détruit
 * 
 * @param bat Bateau concerné
 * @return true Toutes les cases du bateau sont détruites
 * @return false Pas toutes les cases du bateau sont détruites
 */
bool estDetruit(Bateau *bat)
{
    // Si le bateau n'est pas posé, considérer qu'il est détruit
    if (!bat->estPose)
        return true;

    Case *c; // Case en cours
    int i = 0;

    // Itérer les cases occupées par le bateau
    while ((c = bat->cases[i++]) != NULL)
    {
        // S'il y a au moins 1 case occupée, c'est qu'elle n'est pas détruite
        // donc le bateau n'est pas entièrement détruit
        if (c->etat == OCCUPE)
        {
            return false;
        }
    }

    return true;
}