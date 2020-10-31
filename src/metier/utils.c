#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

///////////////
// Fonctions //
///////////////

/**
 * @brief Lire une ligne de stdin
 * 
 * @param s Buffer de stockage
 * @param n Nombre maximal de caractères à lire
 * @return char* Valeur de retour de fgets
 */
char *lireLigne(char *s, int n)
{
    // Lire la ligne saisie
    char *p = fgets(s, n, stdin);

    // Retirer le '\n' si présent (dans le cas où caractères lus < n)
    s[strcspn(s, "\n")] = 0;

    /* Purger stdin

    > Pourquoi :
        Dans le cas où la (saisie > n), il reste des caractères dans le buffer stdin que fgets n'a pas consommé
        Problème, lors de la prochaine lecture du stdin (ex. avec fgets), les caractères non lus de la précédente lecture seront lus par cette lecture

    > Solutions :
        Purger stdin avec fflush (uniquement sous Win32)
        Consommer un par un les caractères du buffer stdin
        Utiliser des fonctions spécifiques à certains OS
    */
    __fpurge(stdin); // Ne fonctionne que sous Unix

    return p;
}

/**
 * @brief Retourner le numéro de la lettre dans l'alphabet
 * 
 * @param c Lettre de l'alphabet
 * @return int 0 si pas une lettre; sinon numéro de la lettre
 */
int lettreToNum(char c)
{
    c = tolower(c);

    // Vérifier que c'est bien une lettre A-Z
    if (c < 'a' || c > 'z')
        return 0;

    return c - 'a' + 1;
}