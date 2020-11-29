#ifndef H_UTILS
#define H_UTILS

///////////
// Types //
///////////

#define UBYTE unsigned char
#define USHORT unsigned short
#define UINT unsigned int
#include <stdbool.h>

/**
 * @brief Structure de coordonnées
 */
typedef struct Coordonnees
{
    UBYTE x;
    UBYTE y;
} Coordonnees;

///////////////
// Fonctions //
///////////////

char *lireLigne(char *s, int n);
int lettreToNum(char c);

#endif