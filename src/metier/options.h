#ifndef H_OPTIONS
#define H_OPTIONS

#include "utils.h"

///////////
// Types //
///////////

/* Structure des options du jeu */
typedef struct JOptions
{
    UBYTE l_plateau;     // Longueur du plateau
    UBYTE h_plateau;     // Hauteur du plateau
    UBYTE taille_flotte; // Nombre de bateaux
} JOptions;

///////////////
// Fonctions //
///////////////

bool opt_setLP(int l);
bool opt_setHP(int h);
bool opt_setTF(int n);
JOptions getOptions(void);

////////////
// Macros //
////////////

/* Taille maximale de la flotte */
#define OPT_T_FLOTTE_MAX 10

/* Accesseurs rapides des options */
#define OPT_L_PLATEAU getOptions().l_plateau
#define OPT_H_PLATEAU getOptions().h_plateau
#define OPT_T_FLOTTE getOptions().taille_flotte

#endif