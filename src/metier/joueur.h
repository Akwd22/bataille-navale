#ifndef H_JOUEUR
#define H_JOUEUR

#include "plateau.h"
#include "bateau.h"

///////////
// Types //
///////////

/**
 * @brief Structure d'un joueur
 */
typedef struct Joueur
{
    /**
     * @brief Tableau 2D (de pointeurs vers cases) correspondant au plateau du joueur
     */
    Case **plateau;
    /**
     * @brief Flotte du joueur
     */
    Bateau *flotte;
} Joueur;

////////////////////////
// Variables globales //
////////////////////////

/**
 * @brief Vous
 */
extern Joueur *pJoueur1;
/**
 * @brief Adversaire
 */
extern Joueur *pJoueur2;

///////////////
// Fonctions //
///////////////

void initFlotte(Joueur *j);
UBYTE getNumBateau(Joueur *j, Bateau *bat);
Joueur *getGagnant();

#endif