#include <stdlib.h>
#include <stdio.h>
#include "joueur.h"
#include "ia.h"
#include "options.h"

/* Principe de l'IA :

Note : une case détruite est une case contenant un morceau de bateau

- CAS : aucune case n'a pas été précédemment détruite
    > Tirer aléatoirement un obus sur le plateau
    > Si la case touchée a été détruite, s'en souvenir + stocker ses cases adjacentes

- CAS : une case a été précédemment détruite ET que le bateau lié à cette case n'est pas entièrement détruit
    > Tirer un obus sur une adjacente à cette case
    > Si l'adjacente touchée est détruite, s'en souvenir + stocker ses cases adjacentes

Et on recommence...
*/

////////////////////////
// Variables globales //
////////////////////////

static Case *derniereCaseDetruite = NULL; // Dernière case détruite
static Case **casesAdjacentes = NULL;     // Tableau de pointeurs vers cases adjacentes des dernières cases détruites d'un même bateau
static UBYTE nbAdj;                       // Taille du tableau casesAdjacentes

///////////////
// Fonctions //
///////////////

/**
 * @brief Allouer la liste d'adjacents, ou augmenter sa capacité
 */
void ia_allocAdjacents(void)
{
    // Si la liste n'est pas allouée ...
    if (casesAdjacentes == NULL)
    {
        // ... l'allouer avec 4 cases
        casesAdjacentes = (Case **)calloc(4, sizeof(Case *));
        nbAdj = 4;
    }
    // Sinon, la liste est déjà allouée ...
    else
    {
        // ... augmenter sa capacité de +4 cases
        casesAdjacentes = (Case **)reallocarray(casesAdjacentes, nbAdj + 4, sizeof(Case *));
        nbAdj += 4;
    }

    // Initialisation à NULL des cases ajoutées
    casesAdjacentes[nbAdj - 4] = NULL;
    casesAdjacentes[nbAdj - 3] = NULL;
    casesAdjacentes[nbAdj - 2] = NULL;
    casesAdjacentes[nbAdj - 1] = NULL;
}

/**
 * @brief Vider la liste des cases adjacentes
 */
void ia_clsAdjacents(void)
{
    free(casesAdjacentes);
    casesAdjacentes = NULL;
    nbAdj = 0;
}

/**
 * @brief Stocker les cases adjacentes à la dernière case détruite
 */
void ia_setAdjacents(void)
{
    //(Re-)Allocation de la liste des adjacents pour stocker +4 cases
    ia_allocAdjacents();

    // Position de la dernière case détruite
    Coordonnees *posCentre = &derniereCaseDetruite->pos;

    /* Append à la fin de la liste, les cases adjacentes qui respectent les limites du plateau */
    // Case adjacente du dessus
    if (posCentre->y - 1 > 0)
    {
        casesAdjacentes[nbAdj - 4] = &pJoueur1->plateau[posCentre->y - 1][posCentre->x];
    }

    // Case adjacente du dessous
    if (posCentre->y + 1 < OPT_H_PLATEAU)
    {
        casesAdjacentes[nbAdj - 3] = &pJoueur1->plateau[posCentre->y + 1][posCentre->x];
    }

    // Case adjacente de gauche
    if (posCentre->x - 1 > 0)
    {
        casesAdjacentes[nbAdj - 2] = &pJoueur1->plateau[posCentre->y][posCentre->x - 1];
    }

    // Case adjacente de droite
    if (posCentre->x + 1 < OPT_L_PLATEAU)
    {
        casesAdjacentes[nbAdj - 1] = &pJoueur1->plateau[posCentre->y][posCentre->x + 1];
    }
}

/**
 * @brief Retourne une adjacente qui n'a pas été touché
 * 
 * @return Case* Case adjacente non touchée; NULL si erreur
 */
Case *ia_getAdjacent(void)
{
    // Pour chaque cases adjacentes ...
    for (int i = 0; i < nbAdj; i++)
    {
        Case *c = casesAdjacentes[i];

        // ... chercher une case non touchée
        if (c != NULL && !c->estTouche)
        {
            casesAdjacentes[i] = NULL; // Retirer la case adjacente de la liste
            return c;                  // Retourner la case adjacente
        }
    }

    // Erreur, cette fonction devrait obligatoirement être appelée que s'il y a des adjacents
    return NULL;
}

Coordonnees ia_tirer(void)
{
    Coordonnees pos = {0, 0}; // Coordonnées où tirer

    /***************************************/
    /* Détermination de la position du tir */
    /***************************************/

    // Si aucune case n'a été détruite précédemment ...
    if (derniereCaseDetruite == NULL)
    {
        // ... tirer aléatoirement entre 1 et TAILLE_PLATEAU jusqu'à succès du tir
        do
        {
            pos.x = (rand() % (OPT_L_PLATEAU - 1)) + 1;
            pos.y = (rand() % (OPT_H_PLATEAU - 1)) + 1;
        } while (!tirerObus(pJoueur1->plateau, pos));
    }
    // Sinon, une case a été précédemment détruite ...
    else
    {
        // ... récupérer une case adjacente et tirer dessus
        pos = ia_getAdjacent()->pos;
        tirerObus(pJoueur1->plateau, pos);
    }

    Case *c = &pJoueur1->plateau[pos.y][pos.x]; // Case touchée

    /********************************/
    /* Si le tir a détruit une case */
    /********************************/

    if (c->etat == DETRUIT)
    {
        // Si le bateau lié à la case est entièrement détruit ...
        if (estDetruit(c->pBat))
        {
            // ... l'IA devra chercher une case contenant un autre bateau
            derniereCaseDetruite = NULL;
            ia_clsAdjacents();
        }
        // Sinon, il reste des cases à détruire du bateau lié à cette case ...
        else
        {
            // ... l'IA devra chercher une case contenant un autre morceau de ce bateau parmi les adjacents
            derniereCaseDetruite = c;
            ia_setAdjacents(); // Récupérer les cases adjacentes
        }
    }

    return pos;
}

void ia_placerFlotte(void)
{
    Coordonnees pos = {0, 0}; // Position où poser
    bool rot;

    // Pour chaque bateau ...
    for (int i = 0; i < OPT_T_FLOTTE; i++)
    {
        // ... le placer aléatoirement sur le plateau
        do
        {
            pos.x = (rand() % (OPT_L_PLATEAU - 1)) + 1;
            pos.y = (rand() % (OPT_H_PLATEAU - 1)) + 1;
            rot = (rand() % 2); // Générer entre 0 et 1
        } while (!poserBateau(pJoueur2->plateau, pJoueur2->flotte + i, &pos, rot));
    }
}

Coordonnees ia_getPosDerniereCaseDetruite(void)
{
    // Aucune dernière case détruite
    if (derniereCaseDetruite == NULL)
    {
        return (Coordonnees){0xFF, 0xFF};
    }

    return derniereCaseDetruite->pos;
}

Coordonnees *ia_getPosCasesAdjacentes(void)
{
    Coordonnees *c = calloc(nbAdj, sizeof(Coordonnees));

    for (int i = 0; i < nbAdj; i++)
    {
        if (casesAdjacentes[i] == NULL)
        {
            c[i].x = 0xFF;
            c[i].y = 0xFF;
        }
        else
        {
            c[i].x = casesAdjacentes[i]->pos.x;
            c[i].y = casesAdjacentes[i]->pos.y;
        }
    }

    return c;
}

UBYTE ia_getNbAdj(void)
{
    return nbAdj;
}

void ia_loadData(Coordonnees derniereCase, UBYTE nb, Coordonnees *adjacentes)
{
    // Aucune dernière case détruite
    if (derniereCase.x == 0xFF && derniereCase.y == 0xFF)
    {
        derniereCaseDetruite = NULL;
    }
    else
    {
        derniereCaseDetruite = &pJoueur1->plateau[derniereCase.y][derniereCase.x];
    }

    nbAdj = nb;

    casesAdjacentes = calloc(nb, sizeof(Case *));
    Case** c = casesAdjacentes;

    for (int i = 0; i < nb; i++)
    {
        int y = adjacentes[i].y;
        int x = adjacentes[i].x;

        // N'insérer que les adjacentes non nulles
        if (y != 0xFF && x != 0xFF) {
            casesAdjacentes[i] = &pJoueur1->plateau[y][x];
        }
    }
}