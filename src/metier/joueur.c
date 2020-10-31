#include <stdio.h>
#include <stdlib.h>
#include "options.h"
#include "joueur.h"

////////////////////////
// Variables globales //
////////////////////////

/* Joueurs */
Joueur *pJoueur1; // Vous
Joueur *pJoueur2; // Adversaire

/* Modèles des bateaux */
const Bateau templateBat[OPT_T_FLOTTE_MAX] = {
    {4, 1, NULL, false},
    {5, 1, NULL, false},
    {6, 1, NULL, false},
    {2, 3, NULL, false},
    {2, 1, NULL, false},
    {4, 2, NULL, false},
    {2, 3, NULL, false},
    {4, 4, NULL, false},
    {2, 3, NULL, false},
    {1, 1, NULL, false}
};

///////////////
// Fonctions //
///////////////

/**
 * @brief Initialiser la flotte d'un joueur
 * @param j Joueur concerné
 */
void initFlotte(Joueur *j)
{
    getOptions().taille_flotte;
    j->flotte = calloc(OPT_T_FLOTTE, sizeof(Bateau));

    // Affecter à la flotte du joueur, les modèles de bateaux
    for (int i = 0; i < OPT_T_FLOTTE; i++)
    {
        j->flotte[i] = templateBat[i];
    }
}

/**
 * @brief Retourner le numéro d'un bateau dans la flotte d'un joueur
 * 
 * @param j Joueur concerné
 * @param bat Bateau concerné
 * @return UBYTE Numéro du bateau dans la flotte; 0xFF si bateau non trouvé
 */
UBYTE getNumBateau(Joueur *j, Bateau *bat)
{
    // Chercher le bateau dans la flotte ...
    for (UBYTE i = 0; i < OPT_T_FLOTTE; i++)
    {
        // ... et retourner son numéro quand trouvé
        if (j->flotte + i == bat)
        {
            return i;
        }
    }

    // Le bateau n'est pas présent dans la flotte
    return 0xFF;
}

/**
 * @brief Retourner le joueur gagnant
 * @return Joueur* Joueur gagnant s'il y a; NULL si aucun gagnant
 */
Joueur *getGagnant()
{
    bool j1_gagnant = true;
    bool j2_gagnant = true;

    // Pour chaque bateau de la flotte des deux joueurs
    for (int i = 0; i < OPT_T_FLOTTE; i++)
    {
        // Si au moins 1 bateau non détruit dans le plateau du joueur 2, joueur 1 pas gagnant
        if (!estDetruit(pJoueur2->flotte + i))
        {
            j1_gagnant = false;
        }

        // Si au moins 1 bateau non détruit dans le plateau du joueur 1, joueur 2 pas gagnant
        if (!estDetruit(pJoueur1->flotte + i))
        {
            j2_gagnant = false;
        }
    }

    // Retourner le gagnant s'il y en a un
    if (j1_gagnant)
    {
        return pJoueur1;
    }
    else if (j2_gagnant)
    {
        return pJoueur2;
    }

    // Aucun joueur n'est gagnant
    return NULL;
}