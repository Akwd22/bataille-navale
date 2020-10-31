#include "options.h"

////////////////////////
// Variables globales //
////////////////////////

/* Options par défaut */
static JOptions options = {.l_plateau = 15 + 1, .h_plateau = 15 + 1, .taille_flotte = 5};

///////////////
// Fonctions //
///////////////

/**
 * @brief Modifier l'option : longueur du plateau
 * 
 * @param l Longueur
 * @return true Modification avec succès
 * @return false Contraintes non-respectées
 */
bool opt_setLP(int l)
{
    /* Vérifications des contraintes */
    if (l < 5 || l > 25)
        return false;

    options.l_plateau = l + 1;
    return true;
}

/**
 * @brief Modifier l'option : hauteur du plateau
 * 
 * @param h Hauteur
 * @return true Modification avec succès
 * @return false Contraintes non-respectées
 */
bool opt_setHP(int h)
{
    /* Vérifications des contraintes */

    if (h < 5 || h > 25)
        return false;

    options.h_plateau = h + 1;
    return true;
}

/**
 * @brief Modifier l'option : taille de la flotte
 * 
 * @param n Nombre de bateaux
 * @return true Modification avec succès
 * @return false Contraintes non-respectées
 */
bool opt_setTF(int n)
{
    /* Vérifications des contraintes */
    if (n < 1 || n > OPT_T_FLOTTE_MAX)
        return false;

    /* Modifications des options */
    options.taille_flotte = n;
    return true;
}

/**
 * @brief Retourner les options du jeu
 * @return JOptions Options du jeu
 */
JOptions getOptions(void)
{
    return options;
}