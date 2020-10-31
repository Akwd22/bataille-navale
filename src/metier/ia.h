#ifndef H_IA
#define H_IA

///////////////
// Fonctions //
///////////////

/**
 * @brief Faire tirer un obus à l'IA
 * @return Coordonnees Où le tir a été fait
 */
Coordonnees ia_tirer(void);

/**
 * @brief Placer aléatoirement la flotte de l'IA sur le plateau
 * 
 */
void ia_placerFlotte(void);

/**
 * @brief Retourner la position de la dernière case détruite
 * @return Coordonnees De la case détruite; {0xFF, 0xFF} si aucune
 */
Coordonnees ia_getPosDerniereCaseDetruite(void);

/**
 * @brief Retourner la liste des positions de chaque cases adjcentes
 * @return Coordonnees* Tableau des positions (les cases NULL sont caractérisées par {0xFF, 0xFF})
 */
Coordonnees* ia_getPosCasesAdjacentes(void);

/**
 * @brief Retourner le nombre d'ajdacentes
 * @return UBYTE Nombre d'adjacentes
 */
UBYTE ia_getNbAdj(void);

/**
 * @brief Charger les données de l'IA depuis une sauvegarde
 */
void ia_loadData(Coordonnees derniereCase, UBYTE nb, Coordonnees* casesAdjacentes);

#endif