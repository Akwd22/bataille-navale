#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "options.h"
#include "plateau.h"

///////////////
// Fonctions //
///////////////

/**
 * @brief Tirer un obus sur un plateau
 * 
 * @param plt Plateau concerné
 * @param pos Position d'atterrissage
 * @return true Le tir a été effectué
 * @return false Le tir a échoué
 */
bool tirerObus(PPlateau plt, Coordonnees pos)
{
    // Vérifier si les coordonnées sont dans les limites du plateau
    if (pos.x == 0 || pos.y == 0 || pos.x >= OPT_L_PLATEAU || pos.y >= OPT_H_PLATEAU)
        return false;

    Case *c = &plt[pos.y][pos.x];

    // Vérifier que la case n'est pas déjà détruite ou touchée
    if (c->etat == DETRUIT || c->estTouche)
        return false;

    // Case occupée par morceau d'un bateau ...
    if (c->etat == OCCUPE)
    {
        c->etat = DETRUIT; // ... détruire le morceau
    }

    c->estTouche = true;

    return true;
}

/**
 * @brief Retourner s'il y a une zone libre pour placer un bateau
 * 
 * @param plt Plateau concerné
 * @param bat Bateau à placer
 * @param pos Position où poser
 * @return true Zone libre
 * @return false Aucune zone libre
 */
bool estZoneLibre(PPlateau plt, Bateau *bat, Coordonnees *pos)
{
    // Vérifier si les cases dans l'aire du bateau sont libre
    for (int i = 0; i < bat->h; i++)
    {
        for (int j = 0; j < bat->l; j++)
        {
            if (plt[pos->y + i][pos->x + j].etat != LIBRE)
                return false;
        }
    }

    return true;
}

/**
 * @brief Créer un plateau et initialiser ses cases à l'état libre
 * @return PPlateau Plateau créé
 */
PPlateau initPlateau(void)
{
    // Allocation d'un tableau 2D
    PPlateau plateau = (PPlateau)calloc(OPT_H_PLATEAU, sizeof(Case));

    for (int i = 0; i < OPT_H_PLATEAU; i++)
    {
        plateau[i] = (Case *)calloc(OPT_L_PLATEAU, sizeof(Case));

        // Initialisation de chaque case à l'état libre
        for (int j = 0; j < OPT_L_PLATEAU; j++)
        {
            plateau[i][j].pos = (Coordonnees){j, i};
            plateau[i][j].etat = LIBRE;
            plateau[i][j].pBat = NULL;
            plateau[i][j].estTouche = false;
        }
    }

    return plateau;
}

/**
 * @brief Poser un bateau sur un plateau
 * 
 * @param plt Plateau concerné
 * @param bat Bateau à poser
 * @param pos Position où poser
 * @param rot Rotationner de 90°
 * @return true Le bateau a été posé
 * @return false Le bateau n'a pas pu être posé
 */
bool poserBateau(PPlateau plt, Bateau *bat, Coordonnees *pos, bool rot)
{
    // Vérifique le bateau est pas déjà posé
    if (bat->estPose)
        return false;

    // Rotationner le bateau de 90°
    if (rot)
        rotationner(bat);

    // Vérifier si le bateau ne dépasse pas du plateau
    if (pos->x == 0 || pos->y == 0 || pos->x + bat->l > OPT_L_PLATEAU || pos->y + bat->h > OPT_H_PLATEAU)
        return false;

    // Vérifier si la zone de placement est libre
    if (!estZoneLibre(plt, bat, pos))
        return false;

    // Hauteur et largeur de la zone de protection autour du bateau
    Coordonnees zoneProt = {bat->l + 2, bat->h + 2};

    // Allocation du nombre de cases occupées par le bateau (comprend les cases de protection)
    // Calcul : Aire du navire + aire de la zone de protection + 1
    bat->cases = (Case **)calloc((bat->l * bat->h) + (2 * zoneProt.x) + (2 * zoneProt.y) + 1, sizeof(Case *));

    // Case courante dans bat->cases
    UBYTE nbCase = 0;

    // Changer l'état des cases occupées par le bateau
    for (int i = 0; i < bat->h; i++)
    {
        for (int j = 0; j < bat->l; j++)
        {
            // Récupère la case actuelle
            Case *caseActuelle = &plt[pos->y + i][pos->x + j];

            // Modification des champs de la case
            caseActuelle->pBat = bat;
            caseActuelle->etat = OCCUPE;
            bat->cases[nbCase++] = caseActuelle; // Ajoute la case à la liste
        }
    }

    // Changer l'état des cases autour du bateau
    for (int i = 0; i < zoneProt.y; i++)
    {
        for (int j = 0; j < zoneProt.x; j++)
        {
            // Coin supérieur gauche de la zone de protection
            Coordonnees csg = {pos->x + j - 1, pos->y + i - 1};

            // Passer cette itération, car la case est dehors du plateau
            if (csg.x == 0 || csg.y == 0 || csg.x >= OPT_L_PLATEAU || csg.y >= OPT_H_PLATEAU)
                continue;

            // Récupère la case actuelle
            Case *caseActuelle = &plt[csg.y][csg.x];

            // Passer cette itération, car la case est occupée par le bateau
            if (caseActuelle->etat == OCCUPE)
                continue;

            // Modification des champs de la case
            caseActuelle->pBat = bat;
            caseActuelle->etat = PROTECT;
            bat->cases[nbCase++] = caseActuelle; // Ajoute la case à la liste
        }
    }

    bat->estPose = true;

    return true;
}