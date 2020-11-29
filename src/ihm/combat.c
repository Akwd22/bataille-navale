/**
 * @file combat.c
 * @brief Gestion de l'affichage de la phase combat
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include "../metier/utils.h"
#include "../metier/joueur.h"
#include "../metier/bateau.h"
#include "../metier/plateau.h"
#include "../metier/ia.h"
#include "divers.h"
#include "combat.h"
#include "../metier/jeu.h"
#include "../metier/options.h"

/**
 * @brief Afficher un plateau
 * @param plateau Plateau concerné
 */
void printPlateau(PPlateau plateau)
{
    printLettreCol();

    for (int i = 1; i < OPT_H_PLATEAU; i++)
    {
        printNumLigne(i);

        // Affichage de chaque case du tableau
        for (int j = 1; j < OPT_L_PLATEAU; j++)
        {
            printf("%c ", plateau[i][j].etat);
        }
    }

    printf("\n");
}

/**
 * @brief Demander l'emplacement pour poser un bateau de la flotte du joueur 1
 * 
 * @param n Numéro du bateau dans la flotte
 * @return int 1 si bateau a été posé; sinon 0
 */
int saisirPoserBateau(int n)
{
    char buffer[5] = {}; // Saisie du joueur
    UBYTE x, y;          // Coordonnées de placement
    bool rot;            // Rotation ?

    printf("\n== Placez votre bateau n°%i sur votre plateau ==\n\n", n + 1);

    /* Demander la position X */
    while ((x = lettreToNum(tolower(buffer[0]))) == 0)
    {
        printf("Lettre de colonne : ");
        lireLigne(buffer, 2);
    }

    buffer[0] = 0;

    /* Demander la position Y */
    while ((y = atoi(buffer)) == 0)
    {
        printf("Numéro de ligne : ");
        lireLigne(buffer, sizeof(buffer));
    }

    buffer[0] = 0;

    /* Demander s'il faut le rotationner */
    printf("Rotationner de 90° (o/N) : ");
    lireLigne(buffer, 2);

    if (tolower(buffer[0]) == 'o')
    {
        rot = true;
    }
    else
    {
        rot = false;
    }

    /* Placer le bateau */
    Coordonnees xy = {x, y};
    return poserBateau(pJoueur1->plateau, pJoueur1->flotte + n, &xy, rot);
}

/**
 * @brief Demander l'emplacement du tir sur le plateau adverse
 * 
 * @return true Le tir a été effectué
 * @return false Le tir a échoué
 */
bool saisirTirerObus(void)
{
    char buffer[5] = {}; // Saisie du joueur
    UBYTE x, y;          // Coordonnées de lancement

    printf("\n== Votre tour : tirez un obus ==\n\n");

    /* Demander la position X */
    while ((x = lettreToNum(buffer[0])) == 0)
    {
        printf("Lettre de colonne : ");
        lireLigne(buffer, 2);
    }

    buffer[0] = 0;

    /* Demander la position Y */
    while ((y = atoi(buffer)) == 0)
    {
        printf("Numéro de la ligne : ");
        lireLigne(buffer, sizeof(buffer));
    }

    /* Tirer l'obus */
    Coordonnees xy = {x, y};
    return tirerObus(pJoueur2->plateau, xy);
}

/**
 * @brief Afficher un plateau en mode adversaire
 * @param plt Plateau concerné
 */
void printPlateauAdverse(PPlateau plt)
{
    printf("\n");
    printLettreCol();

    for (int i = 1; i < OPT_H_PLATEAU; i++)
    {
        printNumLigne(i);

        // Affichage de chaque case du tableau
        for (int j = 1; j < OPT_L_PLATEAU; j++)
        {
            Case *c = &plt[i][j];
            char symbole;

            // Case libre
            if (c->etat == LIBRE)
            {
                if (c->estTouche)
                    symbole = LIBRE;
                else
                    symbole = INCONNU;
            }

            // Case de protection
            if (c->etat == PROTECT)
            {
                if (!estDetruit(c->pBat))
                {
                    if (c->estTouche)
                        symbole = LIBRE;
                    else
                        symbole = INCONNU;
                }
                else
                {
                    symbole = PROTECT;
                }
            }

            // Case détruite
            if (c->etat == DETRUIT)
            {
                symbole = DETRUIT;
            }

            // Case occupée
            if (c->etat == OCCUPE)
            {
                symbole = INCONNU;
            }

            printf("%c ", symbole);
        }
    }

    printf("\n");
}

/**
 * @brief Afficher le plateau du joueur et celui de l'adversaire
 */
void printTourPlateaux(void)
{
    viderConsole();
    printf("[Ctrl+C] pour sauvegarder et quitter le jeu.\n\n");
    printPlateau(pJoueur1->plateau);
    printPlateauAdverse(pJoueur2->plateau);
}

#include "../dao/dao.h"

/**
 * @brief Commencer un seul tour de combat
 * @return Joueur* Joueur gagnant s'il y a; NULL si aucun gagnant au cours de ce tour
 */
Joueur *demarrerTour(void)
{
    sauverPartie();
    printTourPlateaux();

    // Demander au joueur de jouer
    while (!saisirTirerObus())
    {
        printTourPlateaux();
    }

    printTourPlateaux();

    // Le joueur 1 a gagné
    if (getGagnant() == pJoueur1)
        return pJoueur1;

    // Faire jouer l'IA
    printf("\n== Tour de l'adversaire ==\n");
    Coordonnees posTirIA = ia_tirer();
    printf("\nL'adversaire a tiré en %c-%i", posTirIA.x + 64, posTirIA.y);
    fflush(stdout);
    sleep(3);

    printTourPlateaux();

    // Le joueur 2 a gagné
    if (getGagnant() == pJoueur2)
        return pJoueur2;

    // Aucun gagnant
    return NULL;
}

/**
 * @brief Afficher la phase de combat
 */
void demarrerCombat(void)
{
    jeu_partieEnCours = true;

    /* Démarrage du combat tour à tour */
    Joueur *gagnant = NULL;
    while (gagnant == NULL)
    {
        gagnant = demarrerTour();
    }

    if (gagnant == pJoueur1)
        printf("\n== Vous avez gagné ! ==\n");
    else
        printf("\n== Vous avez perdu ==\n");
}