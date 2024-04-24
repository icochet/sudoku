/*!
 * @file sudoku.c
 * @brief Jeu de sudoku
 * @author COCHET Iwan
 * @version 1.0
 * @date 06/11/2023
 *
 * Programme permettant à l'utilisateur de jouer au jeu du Sudoku, avec une grille en caractères ASCII et des instructions sur comment jouer au jeu.
 */

#include <stdlib.h>
#include <stdio.h>

/*!
 *@def N
 *@brief taille des blocs d'une grille de Sudoku.
 */
#define N 3
/*!
 *@def TAILLE
 *@brief taille de la grille du Sudoku.
 */
#define TAILLE N *N

const char REGLES[] = "Le but du jeu est de compléter la grille fournie partiellement remplie, en respectant la règle suivante : dans chaque ligne, chaque colonne et chaque bloc, les chiffres de 1 à 9 doivent apparaitre une et une seule fois. Autrement dit, chaque ligne, chaque colonne et chaque bloc doit contenir au moins une fois tous les chiffres de 1 à 9.\n";
const char ERREUR_TEMPLATE[] = "Erreur ! La case choisie contient un chiffre présent de base sur le Sudoku.\n";
const char ERREUR_ENTIER[] = "Erreur ! La valeur entrée doit être un entier.\n";
const char ERREUR_COMPRISE[] = "Erreur ! La valeur entrée doit être comprise entre 1 et 9.\n";
const char ERREUR_POSSIBLE_LIG[] = "Attention ! Le chiffre choisi est déjà présent dans sa ligne.\n";
const char ERREUR_POSSIBLE_COL[] = "Attention ! Le chiffre choisi est déjà présent dans sa colonne.\n";
const char ERREUR_POSSIBLE_BLOC[] = "Attention ! Le chiffre choisi est déjà présent dans son bloc.\n";

/*!
 *@typedef t_grille
 *@brief type tableau à 2 dimensions de TAILLE*TAILLE entiers
 *
 * Le type t_grille sert à représenter une grille de Sudoku, avec ses lignes et ses colonnes de taille TAILLE.
 */
typedef int t_grille[TAILLE][TAILLE];

void chargerGrille(t_grille g, t_grille gT);
void afficherGrille(t_grille g);
void saisir(int *S);

int isGrilleRemplie(t_grille g);
int possible(t_grille g, int numLigne, int numColonne, int valeur);

/*!
 * @fn int main()
 * @brief Programme principal.
 * @return Code de sortie du programme (0 : sortie normale).
 *
 * Le programme principal utilise les différentes fonctions et procédures définies pour faire jouer le joueur au Sudoku. En utilisant une boucle tant que la grille n'est pas remplie, il redemande à chaque fois les entrées de l'utilisateur, puis affiche en sortant de la boucle que le jeu est fini.
 */
int main()
{
    t_grille grille, grilleTemplate;
    int numLigne, numColonne, valeur;

    chargerGrille(grille, grilleTemplate);

    printf("RÈGLES :\n%s", REGLES);

    while (isGrilleRemplie(grille) == 0)
    {
        afficherGrille(grille);

        printf("Indices de la case ? ");
        saisir(&numLigne);
        numLigne--;
        saisir(&numColonne);
        numColonne--;

        if (grilleTemplate[numLigne][numColonne] != 0)
        {
            printf("%s", ERREUR_TEMPLATE);
        }
        else
        {
            printf("Valeur à insérer ? ");
            saisir(&valeur);

            if (possible(grille, numLigne, numColonne, valeur))
            {
                grille[numLigne][numColonne] = valeur;
            }
        }
    }
    afficherGrille(grille);
    printf("Bravo ! Vous avez complété le Sudoku ! Fin de partie.");

    return EXIT_SUCCESS;
}

/*!
 * @fn void chargerGrille(t_grille g, t_grille gT)
 * @brief Procédure qui charge une grille en mémoire
 * @param g: paramètre de sortie, tableau à 2 dimensions d’entiers de taille TAILLExTAILLE, c’est la grille qui sera modifiée tout au long du jeu.
 * @param gT: paramètre de sortie, tableau à 2 dimensions d’entiers de taille TAILLExTAILLE, cette grille ne changera pas et servira uniquement à connaître à quelle position sont les chiffres préremplis pour la gestion d’erreurs.
 *
 * Permet de charger en mémoire une grille de sudoku existante à partir d’un fichier dont le nom est lu au clavier.
 */
void chargerGrille(t_grille g, t_grille gT)
{
    char nomFichier[30];
    FILE *f;
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f == NULL)
    {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    }
    else
    {
        fread(g, sizeof(int), TAILLE * TAILLE, f);
        fclose(f);
        f = fopen(nomFichier, "rb");
        fread(gT, sizeof(int), TAILLE * TAILLE, f);
    }
    fclose(f);
}
/*!
 * @fn void afficherGrille(t_grille g)
 * @brief Affiche la grille du Sudoku
 * @param g: paramètre d'entrée, tableau à 2 dimensions d’entiers de taille TAILLExTAILLE.
 *
 * Affiche la grille de jeu, en remplaçant les zéros par des points, et en faisant la mise en page comme les séparations avec des caractères ASCII et la numérotation des lignes et colonnes.
 */
void afficherGrille(t_grille g)
{
    int lig; // indice de ligne
    int col; // indice de colonne

    printf("\n\n");
    printf("      1  2  3   4  5  6   7  8  9\n\n");
    printf("    +---------+---------+---------+\n");
    for (lig = 1; lig < TAILLE + 1; lig++)
    {
        printf("%d   |", lig);
        for (col = 1; col < TAILLE + 1; col++)
        {
            if (g[lig - 1][col - 1] == 0)
                printf(" . ");
            else
                printf(" %d ", g[lig - 1][col - 1]);
            if (col % N == 0)
                printf("|");
        }
        printf("\n");
        if (lig % N == 0) printf("    +---------+---------+---------+\n");
    }
    printf("\n\n");
}
/*!
 * @fn void saisir(int *S)
 * @brief Lit au clavier une valeur.
 * @param S: paramètre de sortie, la valeur lue, entier compris entre 1 et N*N.
 *
 * Cette procédure est chargée de lire au clavier une valeur. La saisie se répète tant que la valeur n’est pas valide. En effet, la valeur lue doit être un entier, et cet entier doit être compris entre 1 et N*N. Cette procédure sert aussi bien à saisir une numéro de ligne ou un numéro de colonne qu’à saisir une valeur à insérer dans la grille.
 */
void saisir(int *S)
{
    char ch[2]; // valeur temporaire pour tester si S est un entier

    scanf("%s", ch);

    while (sscanf(ch, "%d", S) == 0)
    {
        printf("%s", ERREUR_ENTIER);
        printf("Veuillez rentrer une valeur correcte : ");
        scanf("%s", ch);
    }
    while ((*S < 1) || (*S > N * N))
    {
        printf("%s", ERREUR_COMPRISE);
        printf("Veuillez rentrer une valeur correcte : ");
        scanf("%d", S);
    }
}
/*!
 * @fn int isGrilleRemplie(t_grille g)
 * @brief Fonction qui indique si la grille de jeu est remplie.
 * @param g: paramètre d'entrée, tableau à 2 dimensions d’entiers de taille TAILLExTAILLE.
 * @return 1 si la grille est remplie et 0 sinon.
 *
 * Fonction qui renvoit 1 si la grille est remplie, cad que aucun 0 n'a été trouvé dans le tableau g, et 0 sinon.
 */
int isGrilleRemplie(t_grille g)
{
    int condition = 1;
    int lig; // indice de ligne
    int col; // indice de colonne

    for (lig = 0; lig < TAILLE; lig++)
    {
        for (col = 0; col < TAILLE; col++)
        {
            if (g[lig][col] == 0)
            {
                condition = 0;
            }
        }
    }

    return condition;
}
/*!
 * @fn int possible(t_grille g, int numLigne, int numColonne, int valeur)
 * @brief Fonction qui indique si une valeur peut être insérée à tel endroit de la grille de Sudoku.
 * @param g: paramètre d'entrée, tableau à 2 dimensions d’entiers de taille TAILLExTAILLE.
 * @param numLigne: paramètre d'entrée, entier compris entre 1 et N*N, indique le numéro de ligne de la valeur passée en paramètre.
 * @param numColonne: paramètre d'entrée, entier compris entre 1 et N*N, indique le numéro de colonne de la valeur passée en paramètre.
 * @param valeur: paramètre d'entrée, entier compris entre 1 et N*N, est la valeur à tester.
 * @return 1 si la valeur peut être insérée dans la grille et 0 sinon.
 *
 * Cette fonction vérifie si une valeur peut être ajoutée dans une case particulière de la grille, cad si cela respecte les règles du sudoku :
 * - la valeur n’est pas déjà présente sur la même ligne que la case
 * - la valeur n’est pas déjà présente sur la même colonne que la case
 * - la valeur n’est pas déjà présente dans le même bloc que la case
 */
int possible(t_grille g, int numLigne, int numColonne, int valeur)
{
    int condition = 1;
    int lig;    // indice de ligne
    int col;    // indice de colonne
    int debLig; // le début de la ligne du bloc
    int debCol; // le début de la colonne du bloc

    for (col = 0; col < TAILLE; col++)
    {
        if (g[numLigne][col] == valeur)
        {
            condition = 0;
            printf("%s", ERREUR_POSSIBLE_LIG);
        }
    }
    for (lig = 0; lig < TAILLE; lig++)
    {
        if (g[lig][numColonne] == valeur)
        {
            condition = 0;
            printf("%s", ERREUR_POSSIBLE_COL);
        }
    }

    switch (numLigne)
    {
    case 0:
    case 1:
    case 2:
        debLig = 0;
        break;
    case 3:
    case 4:
    case 5:
        debLig = 3;
        break;
    case 6:
    case 7:
    case 8:
        debLig = 6;
        break;
    }
    switch (numColonne)
    {
    case 0:
    case 1:
    case 2:
        debCol = 0;
        break;
    case 3:
    case 4:
    case 5:
        debCol = 3;
        break;
    case 6:
    case 7:
    case 8:
        debCol = 6;
        break;
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (g[debLig + i][debCol + j] == valeur)
            {
                condition = 0;
                printf("%s", ERREUR_POSSIBLE_BLOC);
            }
        }
    }

    return condition;
}