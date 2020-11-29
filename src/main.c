/**
 * @file main.c
 * @brief Gestion des arguments de lancement, démarrage du jeu
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "metier/options.h"
#include "ihm/menu.h"

////////////////////////
// Variables globales //
////////////////////////

/* Nom de l'exécutable */
static char *nom_prog;

/////////////////////////////////////////
// Handlers des arguments de lancement //
/////////////////////////////////////////

/**
 * @brief Afficher l'aide
 * @param prg Nom de l'exécutable du programme
 */
void helpHandler(void)
{
	printf("Bataille Navale\n\n");
	printf("	Utilisation :\n");
	printf("		%s [options]\n", nom_prog);
	printf("	Options :\n");
	printf("		--help		Afficher la liste des paramètres de lancement\n");
	printf("		--lp <entier>	Longueur du plateau de jeu (entre 5-25 compris)\n");
	printf("		--hp <entier>	Hauteur du plateau de jeu (entre 5-25 compris)\n");
	printf("		--tf <entier>	Taille de la flotte (entre 1-10 compris)\n");
	exit(EXIT_SUCCESS);
}

/**
 * @brief Afficher une erreur de paramètre
 * 
 * @param prg Nom de l'exécutable du programme
 * @param msg Message d'erreur
 */
void errHandler(char *msg)
{
	fprintf(stderr, "Erreur : '%s'\n", msg);
	helpHandler();
}

/**
 * @brief Modifier l'option du jeu : longueur du plateau
 * @param l Longueur
 */
void lpHandler(int l)
{
	if (!opt_setLP(l))
		errHandler("--lp <entier>, l'entier ne respecte pas les contraintes");
}
/** 
 * @brief Modifier l'option du jeu : hauteur du plateau
 * @param h Hauteur
 */
void hpHandler(int h)
{
	if (!opt_setHP(h))
		errHandler("--hp <entier>, l'entier ne respecte pas les contraintes");
}

/**
 * @brief Modifier l'option du jeu : taille de la flotte
 * @param n Nombre de bateaux
 */
void tfHandler(int n)
{
	if (!opt_setTF(n))
		errHandler("--tf <entier>, l'entier ne respecte pas les contraintes");
}

///////////////
// Fonctions //
///////////////

/**
 * @brief Parser les arguments de lancement
 * [Note] :
 * Il est aussi possible de parser les arguments avec http://manpagesfr.free.fr/man/man3/getopt.3.html
 * 
 * @param argc Nombre d'arguments précisés
 * @param argv Pointeurs vers les arguments
 */
void parseArgs(int argc, char *argv[])
{
	nom_prog = argv[0];

	/* Aucun argument précisé */
	if (argc == 1)
		return;

	/* Parser les arguments */
	int i = 1;
	while (i < argc)
	{
		char *arg = argv[i]; // Argument en cours

		// Aide
		if (strcmp(arg, "--help") == 0)
		{
			helpHandler();
		}

		// Longueur de plateau
		if (strcmp(arg, "--lp") == 0)
			// Vérifier qu'il y a bien un entier précisé l'argument d'après
			if ((argv[i + 1] != NULL) && (atoi(argv[i + 1]) != 0))
			{
				lpHandler(atoi(argv[i + 1])); // Passer en paramètre l'entier
				i = i + 2;					  // Sauter le prochain argument vu que c'était l'entier de cet arg.
				continue;
			}
			// Aucun entier n'a été précisé  à l'argument d'après
			else
			{
				errHandler("--lp <entier>");
			}

		// Hauteur de plateau
		if (strcmp(arg, "--hp") == 0)
			if ((argv[i + 1] != NULL) && (atoi(argv[i + 1]) != 0))
			{
				hpHandler(atoi(argv[i + 1]));
				i = i + 2;
				continue;
			}
			else
			{
				errHandler("--hp <entier>");
			}

		// Taille de la flotte
		if (strcmp(arg, "--tf") == 0)
			if ((argv[i + 1] != NULL) && (atoi(argv[i + 1]) != 0))
			{
				tfHandler(atoi(argv[i + 1]));
				i = i + 2;
				continue;
			}
			else
			{
				errHandler("--tf <entier>");
			}

		// Argument inconnu
		errHandler("paramètre inconnu");
	}
}

int main(int argc, char *argv[])
{
	parseArgs(argc, argv);
	srand(time(NULL)); // Définir la seed du générateur avec l'heure du système
	printMenu();
	return EXIT_SUCCESS;
}