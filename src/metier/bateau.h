#ifndef H_BATEAU
#define H_BATEAU

#include "plateau.h"

///////////
// Types //
///////////

/**
 * @brief Structure d'un bateau
 */
typedef struct Bateau
{
    /**
     * @brief Hauteur du bateau
     */
    UBYTE h;
    /**
     * @brief Largeur du bateau
     */
    UBYTE l;
    /**
     * @brief Indique si le bateau est posé
     */
    bool estPose;
    /**
     * @brief Tableau de pointeurs vers les cases occupées (+ protection) par le bateau, fin du tableau marquée par NULL
     */
    struct Case **cases;
} Bateau;

///////////////
// Fonctions //
///////////////

int getNbCasesOccupees(Bateau *bat);
void rotationner(Bateau *bat);
bool estDetruit(Bateau *bat);

#endif