#ifndef H_COMBAT
#define H_COMBAT

#include <stdbool.h>
#include "../metier/joueur.h"
#include "../metier/plateau.h"

int saisirPoserBateau(int n);
bool saisirTirerObus(void);
void printPlateauAdverse(PPlateau plt);
void printTourPlateaux(void);
void printPlateau(PPlateau plateau);
Joueur *demarrerTour(void);
void demarrerCombat(void);

#endif