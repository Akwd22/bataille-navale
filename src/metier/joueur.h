#ifndef H_JOUEUR
#define H_JOUEUR

#include "plateau.h"
#include "bateau.h"

///////////
// Types //
///////////

// Structure d'un joueur
typedef struct Joueur
{
    Case **plateau; // Tableau 2D (de pointeurs vers cases) correspondant au plateau du joueur
    Bateau *flotte; // Flotte du joueur
} Joueur;

////////////////////////
// Variables globales //
////////////////////////

extern Joueur *pJoueur1; // Vous
extern Joueur *pJoueur2; // Adversaire

///////////////
// Fonctions //
///////////////

void initFlotte(Joueur *j);
UBYTE getNumBateau(Joueur *j, Bateau *bat);
Joueur *getGagnant();

#endif