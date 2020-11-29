/**
 * @file dao.c
 * @brief Toutes les fonctions relatives à la sauvegarde et le chargement d'une partie
 */

#include <stdio.h>
#include <stdbool.h>
#include "dao.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <error.h>
#include <fcntl.h>
#include "../metier/utils.h"
#include "../metier/joueur.h"
#include "../metier/bateau.h"
#include "../metier/plateau.h"
#include "../metier/ia.h"
#include "../metier/options.h"
#include "../metier/jeu.h"

#define NOM_FICHIER "sauvegarde.sav"

///////////
// Types //
///////////

/**
 * @brief Case dans le fichier de sauvegarde
 * @see Case
 */
struct CaseSauvegarde
{
    /**
     * @brief Position de la case sur le plateau
     */
    Coordonnees pos;
    /**
     * @brief État de la case
     */
    EtatCase etat;
    /**
     * @brief Indique si la case a été touché
     */
    bool estTouche;
    /**
     * @brief Numéro du bateau lié à la case (0xFF si aucun)
     */
    UBYTE pBat;
};

/**
 * @brief Bateau dans le fichier de sauvegarde
 * @see Bateau
 */
struct BateauSauvegarde
{
    /**
     * @brief Hauteur du bateau
     */
    UBYTE h;
    /**
     * @brief Longueur du bateau
     */
    UBYTE l;
    /**
     * @brief Indique si le bateau est posé
     */
    bool estPose;
    /**
     * @brief Nombre de cases occupées par le bateau
     */
    UBYTE nbCases;
    /**
     * @brief Tableau des positions (dans le plateau) des cases occupées par le bateau
     */
    Coordonnees *pCases;
};

/**
 * @brief Données de l'IA dans le fichier de sauvegarde
 */
struct IASauvegarde
{
    /**
     * @brief Position (dans le plateau) de la dernière case détruite
     */
    Coordonnees pDerniereCaseDetruite;
    /**
     * @brief Taille du tableau casesAdjacentes
     */
    UBYTE nbAdj;
    /**
     * @brief Tableau des positions (dans le plateau) des cases adjcentes
     */
    Coordonnees *pCasesAdjacentes;
};

/**
 * @brief Format du fichier de sauvegarde
 */
struct FichierSauvegarde
{
    /**
     * @brief Options du jeu
     */
    JOptions options;

    /* Données du joueur 1 */
    /**
     * @brief Plateau du joueur 1, ensemble des cases
     */
    struct CaseSauvegarde *cases_j1;

    /**
     * @brief Flotte du joueur 1, ensemble des bateaux
     */
    struct BateauSauvegarde *bateaux_j1;

    /* Données du joueur 2 */
    // Plateau
    struct CaseSauvegarde *cases_j2; // Ensemble des cases

    // Flotte
    struct BateauSauvegarde *bateaux_j2; // Ensemble des bateaux

    /**
     * @brief Données de l'IA
     */
    struct IASauvegarde ia;
};

///////////////
// Fonctions //
///////////////

/* Pourquoi utiliser fopen et pas open ? : https://stackoverflow.com/a/1658517 */
/* Ouvrir le fichier sauvegarde avec un éditeur hexadécimal et observer le résultat
Les champs dans le fichier n'ont pas même taille en octet que les champs dans la structure.
Raison : il y a un mécanisme de padding : https://fresh2refresh.com/c-programming/c-structure-padding/
*/

/**
 * @brief Sauvegarder la partie
 * 
 * @return true Sauvegarde avec succès
 * @return false Problème lors de la sauvegarde
 */
bool sauverPartie(void)
{
    /* Ouverture en mode écriture seule et binaire */
    FILE *f = fopen(NOM_FICHIER, "wb");

    // Erreur lors de l'ouverture
    if (f == NULL)
    {
        perror("Impossible de créer la sauvegarde :");
        return false;
    }

    // Préambule
    struct FichierSauvegarde fs = {};
    USHORT nbCases = (OPT_L_PLATEAU - 1) * (OPT_H_PLATEAU - 1); // Nombre de cases dans les limites du plateau

    /******************/
    /* Options du jeu */
    /******************/

    fs.options = (JOptions){OPT_L_PLATEAU - 1, OPT_H_PLATEAU - 1, OPT_T_FLOTTE}; // Ne sauver que les dimensions sans compter les limites du plateau
    fwrite(&fs.options, sizeof(fs.options), 1, f);

    /***********************/
    /* Données du joueur 1 */
    /***********************/

    /* Plateau */
    fs.cases_j1 = calloc(nbCases + 1, sizeof(struct CaseSauvegarde));

    // Pour chaque case du plateau ...
    for (int ligne = 1; ligne < OPT_H_PLATEAU; ligne++)
    {
        for (int col = 1; col < OPT_L_PLATEAU; col++)
        {
            // Calcul combientième case
            int n = (ligne * (OPT_L_PLATEAU - 1)) - ((OPT_L_PLATEAU - 1) - col);

            // ... les insérer dans la structure de sauvegarde du plateau
            fs.cases_j1[n].pos = pJoueur1->plateau[ligne][col].pos;
            fs.cases_j1[n].etat = pJoueur1->plateau[ligne][col].etat;
            fs.cases_j1[n].estTouche = pJoueur1->plateau[ligne][col].estTouche;
            fs.cases_j1[n].pBat = getNumBateau(pJoueur1, pJoueur1->plateau[ligne][col].pBat);
            fwrite(fs.cases_j1 + n, sizeof(struct CaseSauvegarde), 1, f);
        }
    }

    free(fs.cases_j1);

    /* Flotte */
    fs.bateaux_j1 = calloc(OPT_T_FLOTTE, sizeof(struct BateauSauvegarde));

    // Pour chaque bateau de la flotte ...
    for (int i = 0; i < OPT_T_FLOTTE; i++)
    {
        // ... les insérer dans la structure de sauvegarde de la flotte
        fs.bateaux_j1[i].h = pJoueur1->flotte[i].h;
        fs.bateaux_j1[i].l = pJoueur1->flotte[i].l;
        fs.bateaux_j1[i].estPose = pJoueur1->flotte[i].estPose;

        // Sauvegarde du nombres de cases occupées par le bateau
        UBYTE nbCases = getNbCasesOccupees(pJoueur1->flotte + i);
        fs.bateaux_j1[i].nbCases = nbCases;

        // Sauvegarde des positions de chaque case occupée par le bateau ...
        fs.bateaux_j1[i].pCases = calloc(nbCases, sizeof(Coordonnees));

        // ... en faisant le lien avec le plateau réel
        for (int j = 0; j < nbCases; j++)
        {
            fs.bateaux_j1[i].pCases[j].x = pJoueur1->flotte[i].cases[j]->pos.x;
            fs.bateaux_j1[i].pCases[j].y = pJoueur1->flotte[i].cases[j]->pos.y;
        }

        fwrite(fs.bateaux_j1 + i, sizeof(struct BateauSauvegarde) - sizeof(Coordonnees *), 1, f); // Écriture des 4 permiers champs
        fwrite(fs.bateaux_j1[i].pCases, sizeof(Coordonnees), nbCases, f);                         // Écriture du dernier champ
        free(fs.bateaux_j1[i].pCases);
    }

    free(fs.bateaux_j1);

    /***********************/
    /* Données du joueur 2 */
    /***********************/

    // Plateau
    fs.cases_j2 = calloc(nbCases + 1, sizeof(struct CaseSauvegarde));
    for (int ligne = 1; ligne < OPT_H_PLATEAU; ligne++)
    {
        for (int col = 1; col < OPT_L_PLATEAU; col++)
        {
            int n = (ligne * (OPT_L_PLATEAU - 1)) - ((OPT_L_PLATEAU - 1) - col);

            fs.cases_j2[n].pos = pJoueur2->plateau[ligne][col].pos;
            fs.cases_j2[n].etat = pJoueur2->plateau[ligne][col].etat;
            fs.cases_j2[n].estTouche = pJoueur2->plateau[ligne][col].estTouche;
            fs.cases_j2[n].pBat = getNumBateau(pJoueur2, pJoueur2->plateau[ligne][col].pBat);
            fwrite(fs.cases_j2 + n, sizeof(struct CaseSauvegarde), 1, f);
        }
    }

    free(fs.cases_j2);

    // Flotte
    fs.bateaux_j2 = calloc(OPT_T_FLOTTE, sizeof(struct BateauSauvegarde));
    for (int i = 0; i < OPT_T_FLOTTE; i++)
    {
        fs.bateaux_j2[i].h = pJoueur2->flotte[i].h;
        fs.bateaux_j2[i].l = pJoueur2->flotte[i].l;
        fs.bateaux_j2[i].estPose = pJoueur2->flotte[i].estPose;

        UBYTE nbCases = getNbCasesOccupees(pJoueur2->flotte + i);
        fs.bateaux_j2[i].nbCases = nbCases;

        fs.bateaux_j2[i].pCases = calloc(nbCases, sizeof(Coordonnees));

        for (int j = 0; j < nbCases; j++)
        {
            fs.bateaux_j2[i].pCases[j].x = pJoueur2->flotte[i].cases[j]->pos.x;
            fs.bateaux_j2[i].pCases[j].y = pJoueur2->flotte[i].cases[j]->pos.y;
        }

        fwrite(fs.bateaux_j2 + i, sizeof(struct BateauSauvegarde) - sizeof(Coordonnees *), 1, f);
        fwrite(fs.bateaux_j2[i].pCases, sizeof(Coordonnees), nbCases, f);
        free(fs.bateaux_j2[i].pCases);
    }

    free(fs.bateaux_j2);

    /*******************/
    /* Données de l'IA */
    /*******************/

    // Récupération des données de l'IA
    fs.ia = (struct IASauvegarde){ia_getPosDerniereCaseDetruite(), ia_getNbAdj(), ia_getPosCasesAdjacentes()};

    // Écriture dans le fichier des données de l'IA
    fwrite(&fs.ia.pDerniereCaseDetruite, sizeof(Coordonnees), 1, f);
    fwrite(&fs.ia.nbAdj, sizeof(fs.ia.nbAdj), 1, f);
    fwrite(fs.ia.pCasesAdjacentes, sizeof(Coordonnees), fs.ia.nbAdj, f);

    free(fs.ia.pCasesAdjacentes);

    fclose(f);
    return true;
}

/**
 * @brief Charger une partie
 * 
 * @return true Partie chargé avec succès
 * @return false Échec du chargement
 */
bool chargerPartie(void)
{
    /* Ouverture en mode lecture seule et binaire */
    FILE *f = fopen(NOM_FICHIER, "rb");

    // Erreur lors de l'ouverture
    if (f == NULL)
    {
        perror("Impossible d'ouvrir la sauvegarde :");
        return false;
    }

    // Préambule
    struct FichierSauvegarde fs = {};

    /******************/
    /* Options du jeu */
    /******************/

    /* Lecture des options à partir du fichier */
    fread(&fs.options, sizeof(fs.options), 1, f);

    /* Application des options du jeu */
    opt_setLP(fs.options.l_plateau);
    opt_setHP(fs.options.h_plateau);
    opt_setTF(fs.options.taille_flotte);
    initPartie();

    /***********************/
    /* Données du joueur 1 */
    /***********************/

    /* Plateau */
    USHORT nbCases = (OPT_L_PLATEAU - 1) * (OPT_H_PLATEAU - 1);

    // Lire toutes les cases depuis le fichier
    fs.cases_j1 = calloc(nbCases, sizeof(struct CaseSauvegarde));
    fread(fs.cases_j1, sizeof(struct CaseSauvegarde), nbCases, f);

    // Affectation au plateau réel
    // Pour chaque case du plateau ...
    for (int ligne = 1; ligne < OPT_H_PLATEAU; ligne++)
    {
        for (int col = 1; col < OPT_L_PLATEAU; col++)
        {
            // Calcul combientième case
            int n = (ligne * (OPT_L_PLATEAU - 1)) - ((OPT_L_PLATEAU - 1) - col) - 1;

            // ... affecter la case de la sauvegarde
            pJoueur1->plateau[ligne][col].pos = fs.cases_j1[n].pos;
            pJoueur1->plateau[ligne][col].etat = fs.cases_j1[n].etat;
            pJoueur1->plateau[ligne][col].estTouche = fs.cases_j1[n].estTouche;

            // On ne peut pas encore remplir ce champ, nous avons pas encore chargé les bateaux
            pJoueur1->plateau[ligne][col].pBat = NULL; // VERS ...
        }
    }

    /* Flotte */
    fs.bateaux_j1 = calloc(OPT_T_FLOTTE, sizeof(struct BateauSauvegarde));

    // Lire chaque bateau depuis le fichier
    for (int i = 0; i < OPT_T_FLOTTE; i++)
    {
        // Lecture des 4 premiers champs
        fread(&fs.bateaux_j1[i], sizeof(struct BateauSauvegarde) - sizeof(Coordonnees *), 1, f); // h,l,estPose,nbCases

        // Lecture des cases occupées par le bateau
        fs.bateaux_j1[i].pCases = calloc(fs.bateaux_j1[i].nbCases, sizeof(Coordonnees));
        fread(fs.bateaux_j1[i].pCases, sizeof(Coordonnees), fs.bateaux_j1[i].nbCases, f);
    }

    // Affectation à la flotte réel
    // Pour chaque bateau de la flotte ...
    for (int i = 0; i < OPT_T_FLOTTE; i++)
    {
        // ... affecter le bateau de la sauvegarde ...
        pJoueur1->flotte[i].h = fs.bateaux_j1[i].h;
        pJoueur1->flotte[i].l = fs.bateaux_j1[i].l;
        pJoueur1->flotte[i].estPose = fs.bateaux_j1[i].estPose;

        // ... et affectation des cases occupées par le bateau ...
        UBYTE nbCases = fs.bateaux_j1[i].nbCases;                        // Nombre de cases occupées par le bateau
        pJoueur1->flotte[i].cases = calloc(nbCases + 1, sizeof(Case *)); // +1 pour le NULL de fin

        // ... pour chaque case occupée par le bateau de le sauvegarde
        for (int j = 0; j < nbCases; j++)
        {
            // ... l'affecter au bateau réel
            int x = fs.bateaux_j1[i].pCases[j].x;
            int y = fs.bateaux_j1[i].pCases[j].y;
            Case *c = &pJoueur1->plateau[y][x];
            pJoueur1->flotte[i].cases[j] = c;
        }

        free(fs.bateaux_j1[i].pCases);
    }

    free(fs.bateaux_j1);

    // ... ICI; lier les cases du plateau réel aux bateaux
    for (int ligne = 1; ligne < OPT_H_PLATEAU; ligne++)
    {
        for (int col = 1; col < OPT_L_PLATEAU; col++)
        {
            // Calcul combientième case
            int n = (ligne * (OPT_L_PLATEAU - 1)) - ((OPT_L_PLATEAU - 1) - col) - 1;
            UBYTE numBateau = fs.cases_j1[n].pBat;

            // S'il y a bien un bateau associé à cette case, l'associer
            if (numBateau != 0xFF)
            {
                pJoueur1->plateau[ligne][col].pBat = &pJoueur1->flotte[numBateau];
            }
        }
    }

    free(fs.cases_j1);

    /***********************/
    /* Données du joueur 2 */
    /***********************/

    /* Plateau */
    nbCases = (OPT_L_PLATEAU - 1) * (OPT_H_PLATEAU - 1);

    fs.cases_j2 = calloc(nbCases, sizeof(struct CaseSauvegarde));
    fread(fs.cases_j2, sizeof(struct CaseSauvegarde), nbCases, f);

    // Remplissage réel
    for (int ligne = 1; ligne < OPT_H_PLATEAU; ligne++)
    {
        for (int col = 1; col < OPT_L_PLATEAU; col++)
        {
            // Calcul combientième case
            int n = (ligne * (OPT_L_PLATEAU - 1)) - ((OPT_L_PLATEAU - 1) - col) - 1;

            pJoueur2->plateau[ligne][col].pos = fs.cases_j2[n].pos;
            pJoueur2->plateau[ligne][col].etat = fs.cases_j2[n].etat;
            pJoueur2->plateau[ligne][col].estTouche = fs.cases_j2[n].estTouche;
            pJoueur2->plateau[ligne][col].pBat = NULL; // VERS ...
        }
    }

    /* Flotte */
    fs.bateaux_j2 = calloc(OPT_T_FLOTTE, sizeof(struct BateauSauvegarde));

    for (int i = 0; i < OPT_T_FLOTTE; i++)
    {
        fread(&fs.bateaux_j2[i], sizeof(struct BateauSauvegarde) - sizeof(Coordonnees *), 1, f); // h,l,estPose,nbCases

        fs.bateaux_j2[i].pCases = calloc(fs.bateaux_j2[i].nbCases, sizeof(Coordonnees));
        fread(fs.bateaux_j2[i].pCases, sizeof(Coordonnees), fs.bateaux_j2[i].nbCases, f);
    }

    // Remplissage réel
    for (int i = 0; i < OPT_T_FLOTTE; i++)
    {
        pJoueur2->flotte[i].h = fs.bateaux_j2[i].h;
        pJoueur2->flotte[i].l = fs.bateaux_j2[i].l;
        pJoueur2->flotte[i].estPose = fs.bateaux_j2[i].estPose;

        UBYTE nbCases = fs.bateaux_j2[i].nbCases;
        pJoueur2->flotte[i].cases = calloc(nbCases + 1, sizeof(Case *)); // +1 pour le NULL de fin

        for (int j = 0; j < nbCases; j++)
        {
            int y = fs.bateaux_j2[i].pCases[j].y;
            int x = fs.bateaux_j2[i].pCases[j].x;
            Case *c = &pJoueur2->plateau[y][x];
            pJoueur2->flotte[i].cases[j] = c;
        }

        free(fs.bateaux_j2[i].pCases);
    }

    free(fs.bateaux_j2);

    // ... ICI; lier les cases du plateau réel aux bateaux
    for (int ligne = 1; ligne < OPT_H_PLATEAU; ligne++)
    {
        for (int col = 1; col < OPT_L_PLATEAU; col++)
        {
            // Calcul combientième case
            int n = (ligne * (OPT_L_PLATEAU - 1)) - ((OPT_L_PLATEAU - 1) - col) - 1;
            UBYTE numBateau = fs.cases_j2[n].pBat;

            // S'il y a bien un bateau associé à cette case, l'associer
            if (numBateau != 0xFF)
            {
                pJoueur2->plateau[ligne][col].pBat = &pJoueur2->flotte[numBateau];
            }
        }
    }

    free(fs.cases_j2);

    /*******************/
    /* Données de l'IA */
    /*******************/

    // Lecture des premiers champs depuis le fichier
    fread(&fs.ia.pDerniereCaseDetruite, sizeof(fs.ia.pDerniereCaseDetruite), 1, f);
    fread(&fs.ia.nbAdj, sizeof(fs.ia.nbAdj), 1, f);

    // Lecture des positions de cases adjacentes
    fs.ia.pCasesAdjacentes = calloc(fs.ia.nbAdj, sizeof(Coordonnees));
    fread(fs.ia.pCasesAdjacentes, sizeof(Coordonnees), fs.ia.nbAdj, f);

    // Affecter les données de la sauvegarde à l'IA
    ia_loadData(fs.ia.pDerniereCaseDetruite, fs.ia.nbAdj, fs.ia.pCasesAdjacentes);

    free(fs.ia.pCasesAdjacentes);

    fclose(f);
    return true;
}

/**
 * @brief Retourner si une sauvegarde existe
 */
bool sauvegardeExiste(void)
{
    int fd = open(NOM_FICHIER, O_RDONLY);
    close(fd);
    return fd > 0;
}