#ifndef H_PLATEAU
#define H_PLATEAU

#include "utils.h"
#include "bateau.h"

///////////
// Types //
///////////

// État d'une case
typedef enum EtatCase
{
    LIBRE = 'O',   // Case qui ne contient rien
    INCONNU = '?', // Case qui n'a pas été touché (usage pour l'IHM uniquement)
    PROTECT = '#', // Case de protection autour d'un bateau
    DETRUIT = 'X', // Case dont le morceau d'un bateau est détruit
    OCCUPE = 'B'   // Case occupée par le morceau d'un bateau
} EtatCase;

struct Bateau;

// Structure d'une case
typedef struct Case
{
    Coordonnees pos;     // Position de la case sur le plateau
    EtatCase etat;       // État de la case
    bool estTouche;      // Indique si la case a été touché
    struct Bateau *pBat; // Pointeur vers le bateau contenu dans la case
} Case;

// Pointeur vers un plateau
typedef Case **PPlateau;

///////////////
// Fonctions //
///////////////

PPlateau initPlateau(void);
bool tirerObus(PPlateau plt, Coordonnees pos);
bool estZoneLibre(PPlateau plt, Bateau *bat, Coordonnees *pos);
bool poserBateau(PPlateau plt, Bateau *bat, Coordonnees *pos, bool rot);

#endif