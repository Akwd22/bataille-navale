#ifndef H_OPTIONS
#define H_OPTIONS

#include "utils.h"

///////////
// Types //
///////////

/**
 * @brief Structure des options du jeu
 */
typedef struct JOptions
{
    /**
     * @brief Longueur du plateau
     */
    UBYTE l_plateau;
    /**
     * @brief Hauteur du plateau
     */
    UBYTE h_plateau;
    /**
     * @brief Nombre de bateaux
     */
    UBYTE taille_flotte;
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

/**
 * @brief Taille maximale de la flotte
 */
#define OPT_T_FLOTTE_MAX 10

/**
 * @brief Accesseurs rapides des options
 */
#define OPT_L_PLATEAU getOptions().l_plateau
#define OPT_H_PLATEAU getOptions().h_plateau
#define OPT_T_FLOTTE getOptions().taille_flotte

#endif