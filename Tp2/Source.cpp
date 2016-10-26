#include <iostream>
using namespace std;

#include <conio.h>	// pour _kbhit() et _getch()
#include <time.h>	// pour time(int)
#include <stdlib.h>	// pour srand(int) et rand()
#include "RockUtiles.h"	// pour la gestion de l'écran

// Partie I
// --------
void afficherTerrain(int nbLignes, int nbColonnes);
int recupererTouche();
int calculerDirectionTouche(int touche);
void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY);
void deplacerSerpentI(int direction, int &posX, int &posY);

// Partie II
// ---------
int saisirNiveau();
void creerSouris(int nbLignes, int nbColonnes,
	int sourisX[], int sourisY[], int nbSouris);
void afficherSouris(int sourisX[], int sourisY[], int nbSouris);
void deplacerSerpentII(int direction, int serpentX[], int serpentY[],
	int &tailleSerpent,
	int sourisX[], int sourisY[], int &nbSouris);
/*	ou	*/
void deplacerSerpentII(int direction, int serpentX[], int serpentY[],
	int &indiceTete, int &indiceQueue,
	int sourisX[], int sourisY[], int &nbSouris);
bool testerCollision(int x, int y, int sourisX[], int sourisY[], int &nbSouris);


// Partie III (BONUS)
// ------------------
void deplacerSouris(int sourisX[], int sourisY[], int nbSouris);
void placerMurs(int nbLignes, int nbColonnes, int nbMurs);




//  fonction principale
//  -------------------
int main()
{
	// à compléter
}




// ************
// * PARTIE I *
// ************
void afficherTerrain(int nbLignes, int nbColonnes)
/*
Tâche: afficher le contour du terrain
Paramètres: le nombre de lignes et de colonnes du terrain rectangulaire
*/
{
	cadre(0, 0, 120, 30, FOREGROUND_GREEN);
}

int recupererTouche()
/*
Tâche: tester si le joueur a appuyé sur une touche
Retour: retourner le code ASCII de la touche pressée, -1 sinon
*/
{
	// 	utiliser la fonction _kbhit() de conio.h qui retourne une valeur non 
	//	nulle si une touche a été pressée
	// à compléter
}

int calculerDirectionTouche(int touche)
/*
Tâche: calculer la direction correspondant à une touche ou -1
Paramètre: le code d'une touche (w, a, s ou d)
Retour: la direction qui correspond à la touche
(0: droite, 1: gauche, 2: haut, 3: bas)
*/
{
	// à compléter
}

void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY)
/*
Tâche: calculer une position aléatoire sur le terrain
Paramètres: les dimensions du terrain en entrée et les coordonnées de la 				position aléatoire en sortie
*/
{
	// à compléter
}

void deplacerSerpentI(int direction, int &posX, int &posY)
/*
Tâche: déplacer le serpent d'une seule case dans la direction donnée. Le 			serpent est à l'écran avant l'appel et au retour de la fonction
Paramètres: la direction du serpent en entrée, et la position du serpent en 			entrée / sortie
*/
{
	// à compléter
}




// *************
// * PARTIE II *
// *************
int saisirNiveau()
/*
Tâche: lire le niveau de difficulté avec tests de validation d'entrée
Retour: le niveau (= le nombre de souris initialement sur le terrain)
*/
{
	// à compléter
}

void creerSouris(int nbLignes, int nbColonnes,
	int sourisX[], int sourisY[], int nbSouris)
	/*
	Tâche: générer les nbSouris aléatoirement sur le terrain
	Paramètres: les dimensions du terrain, les tableaux de coordonnées et le
	nombre de souris
	*/
{
	// à compléter
}

void afficherSouris(int sourisX[], int sourisY[], int nbSouris)
/*
Tâche: afficher les souris
Paramètres: les tableaux de coordonnées et le nombre de souris
*/
{
	// à compléter
}

void deplacerSerpentII(int direction, int serpentX[], int serpentY[],
	int &tailleSerpent,
	int sourisX[], int sourisY[], int &nbSouris)
	/*
	Tâche: déplacer le serpent d'une seule case dans la direction donnée. Le 			serpent est à l'écran avant l'appel et au retour de la fonction
	Paramètres: en entrée : 	la direction du serpent,
	en entrée/sortie :	les tableaux de coordonnées du serpent,
	la taille du serpent,
	les tableaux de coordonnées des souris et
	le nombre de souris
	*/
{
	// à compléter
}

/*	ou	*/

void deplacerSerpentII(int direction, int serpentX[], int serpentY[],
	int &indiceTete, int &indiceQueue,
	int sourisX[], int sourisY[], int &nbSouris)
	/*
	Tâche: déplacer le serpent dans la direction. Le 	serpent est à l'écran avant 		l'appel et au retour de la fonction
	Paramètres: en entrée :	la direction du serpent,
	en entrée/sortie :	les tableaux de coordonnées du serpent, les 				indices de tête et de queue du serpent,
	les tableaux de coordonnées des souris et
	le nombre de souris
	*/
{
	// à compléter
}

bool testerCollision(int posX, int posY,
	int sourisX[], int sourisY[], int &nbSouris)
	/*
	Tâche: tester si (posX, posY) est la position d'une souris. Si oui, retirer
	la souris de la population de souris
	Paramètres: la position de la tête du serpent, les tableaux de coordonnées
	des souris et le nombre de souris
	Retour: true si collision, false sinon
	*/
{
	// à compléter
}






// **********************
// * PARTIE III : BONUS *
// **********************

void deplacerSouris(int sourisX[], int sourisY[], int nbSouris)
/*
Tâche: déplacer les souris (aléatoirement ou intelligemment)
Paramètres: les tableaux de coordonnées et le nombre de souris
*/
{
	// à compléter
}

void placerMurs(int nbLignes, int nbColonnes, int nbMurs)
/*
Tâche: placer des murs aléatoirement sur le terrain de jeu
Paramètres: les dimensions du terrain et le nombre de murs à placer
*/
{
	// à compléter
}
