/**
 * @file jeu.c
 * @brief Toutes les fonctions relatives au démarrage du jeu
 */

#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "../dao/dao.h"
#include "joueur.h"
#include "jeu.h"

// Indique si la partie est en cours
bool jeu_partieEnCours = false;

/**
 * @brief Handler pour SIGINT lors d'un Ctrl+C.
 * Il permet de sauvegarder la partie en cours et quitter le processus
 * Note : ce signal n'est pas envoyé par le terminal de VSCode
 * @param code Code du signal
 */
void sigint_handler(int code)
{
    // Si la partie est en cours, la sauvegarder
    if (jeu_partieEnCours)
    {
        sauverPartie();
    }

    exit(0);
}

/**
 * @brief Initialiser la partie (variables)
 */
void initPartie()
{
    /* Attribuer un handler à ce signal */
    signal(SIGINT, &sigint_handler);

    /* Allocation des joueurs */
    pJoueur1 = (Joueur *)malloc(sizeof(Joueur));
    pJoueur2 = (Joueur *)malloc(sizeof(Joueur));

    /* Initialisation de leur plateau */
    pJoueur1->plateau = initPlateau();
    pJoueur2->plateau = initPlateau();

    /* Initialisation de leur flotte par défaut */
    initFlotte(pJoueur1);
    initFlotte(pJoueur2);
}