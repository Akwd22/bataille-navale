#ifndef H_PLATEAU
#define H_PLATEAU

#include "utils.h"
#include "bateau.h"

///////////
// Types //
///////////

/**
 * @brief État d'une case
 */
typedef enum EtatCase
{
    /**
     * @brief Case vide
     */
    LIBRE = 'O',
    /**
     * @brief Case adverse dont l'état est inconnu (usage pour l'IHM uniquement)
     */
    INCONNU = '?',
    /**
     * @brief Case adjacente à un morceau de bateau (impossible de placer un bateau sur cette case)
     */
    PROTECT = '#', 
    /**
     * @brief Case contenant un morceau de bateau détruit
     */
    DETRUIT = 'X',
    /**
     * @brief Case contenant un morceau de bateau
     */
    OCCUPE = 'B'
} EtatCase;

struct Bateau;

/**
 * @brief Structure d'une case
 */
typedef struct Case
{
    /**
     * @brief Position de la case sur le plateau
     */
    Coordonnees pos;
    /**
     * @brief État de la case
     */
    EtatCase etat;
    /**
     * @brief Indique si la case a été touché
     */
    bool estTouche;
    /**
     * @brief Pointeur vers le bateau contenu dans la case
     */
    struct Bateau *pBat;
} Case;

/**
 * @brief Pointeur vers un plateau
 */
typedef Case **PPlateau;

///////////////
// Fonctions //
///////////////

PPlateau initPlateau(void);
bool tirerObus(PPlateau plt, Coordonnees pos);
bool estZoneLibre(PPlateau plt, Bateau *bat, Coordonnees *pos);
bool poserBateau(PPlateau plt, Bateau *bat, Coordonnees *pos, bool rot);

#endif