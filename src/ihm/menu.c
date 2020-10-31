#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include "divers.h"
#include "../metier/utils.h"
#include "menu.h"
#include "combat.h"
#include "../metier/jeu.h"
#include "prep.h"
#include "../dao/dao.h"

/**
 * @brief Afficher le logo du jeu
 */
void printLogo(void)
{
    int fd = open("logo.txt", O_RDONLY);

    /* Erreur lors de l'ouverture du fichier */
    if (fd == -1)
    {
        perror("Erreur lors de l'ouverture du logo :");
        return;
    }

    /* Lecture 8 par 8 caractères le fichier */
    char buffer[8];
    int nb = 1; // Nombre de caractères lus

    while (((nb = read(fd, buffer, sizeof(buffer))) > 0))
    {
        write(STDOUT_FILENO, buffer, nb);
        fflush(stdout); // write ne purge pas automatiquement stdout
    }

    close(fd);
}

/**
 * @brief Afficher le menu de titre
 */
void printMenu(void)
{
    /* Affichage du texte */
    viderConsole();
    printLogo();

    printf("\n\n=== Jouer contre l'ordinateur ===\n");
    printf("\n 1. Nouvelle partie");
    printf("\n 2. Charger la partie");

    /* Demander un choix */
    char choix;
    printf("\n\nEntrez le numéro de votre choix : ");
    lireLigne(&choix, 2);

    /* Lancer la bonne action */
    // Nouvelle partie
    if (choix == '1')
    {
        initPartie();
        demarrerPreparation();
        return;
    }

    // Charger la sauvegarde
    if (choix == '2')
    {
        // Vérifier que la partie se charge bien
        if (!chargerPartie())
        {
            fflush(stdout);
            sleep(2);
            printMenu();
        }

        demarrerCombat();
        return;
    }

    // Choix incorrect
    printMenu();
}