#ifndef H_BATEAU
#define H_BATEAU

#include "plateau.h"

///////////
// Types //
///////////

// Structure d'un bateau
typedef struct Bateau
{
    UBYTE h;             // Hauteur du bateau
    UBYTE l;             // Largeur du bateau
    bool estPose;        // Indique si le bateau est posé
    struct Case **cases; // Tableau de pointeurs vers les cases occupées (+ protection) par le bateau, fin du tableau marquée par NULL
} Bateau;

///////////////
// Fonctions //
///////////////

int getNbCasesOccupees(Bateau *bat);
void rotationner(Bateau *bat);
bool estDetruit(Bateau *bat);

#endif