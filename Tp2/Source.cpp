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
//void deplacerSerpentII(int direction, int serpentX[], int serpentY[],
//	int &indiceTete, int &indiceQueue,
//	int sourisX[], int sourisY[], int &nbSouris);
bool testerCollision(int x, int y, int sourisX[], int sourisY[], int &nbSouris);


// Partie III (BONUS)
// ------------------
void deplacerSouris(int sourisX[], int sourisY[], int nbSouris);
void placerMurs(int nbLignes, int nbColonnes, int nbMurs);


// Déclaration des constantes
const  char dessinSouris = 15;
const  char dessinSerpent = 254;
const  char dessinMur = 221;

//  fonction principale
//  -------------------
int main()
{
	// Affichage des variables
	bool partieTerminee = false; // Variable pour déterminer la fin de la partie (la boucle while dans le code)
	int touche, direction;
	int indiceTete = 0, indiceQueue = 0;
	int tailleSerpent = 1;

	// Déterminer le srand() pour les valeurs aléatoires
	srand((unsigned int)time(NULL));

	// Déclaration des tableaux
	int sourisX[20], sourisY[20];


	//----------------------------------------AFFICHAGE DU JEU -----------------------------------------------

	//Déterminer la largeur de la fenetre à apparaître  -- utilise la fonction setDimensionFenetre de RockUtiles
	setDimensionFenetre(0, 0, 71, 35);

	// Affichage du terrain -- Utilise la fonction afficher terrain de la partie 1
	int nbLignes = 30, nbColonnes = 70;
	afficherTerrain(nbLignes, nbColonnes);

	// ---- Déterminer le niveau de la partie 

	int niveau = saisirNiveau();
	int nbSouris = niveau;

	// ---- Créer et afficher les souris selon le niveau sélectionné
	creerSouris(nbLignes, nbColonnes, sourisX, sourisY, nbSouris);
	afficherSouris(sourisX, sourisY, nbSouris);

	// Faire apparaître le serpent
	// Déterminier la position
	int posX = 0, posY = 0;
	positionAleatoire(nbLignes, nbColonnes, posX, posY);
	gotoXY(posX, posY);
	int serpentX[100] = { posX };
	int serpentY[100] = { posY };

	//Afficher le serpent (un bloc dans le terrain que l'on colore d'une autre couleur dans le jeu)
	color(FOREGROUND_RED + BACKGROUND_GREEN);
	cout << dessinSerpent;


	// -------------------------------------------Boucle de la partie--------------------------------

	int tour = 0;
	int ancienneTouche = 0;
	while (!partieTerminee) {

		Sleep(100);

		// Récupérer la touche		

		if (tour == 0) {
			do {
				touche = recupererTouche();
			} while (touche == -1);
		}
		else {
			if (touche != -1) {
				ancienneTouche = touche;
			}
			touche = recupererTouche();
		}

		// Déterminer la direction

		if (touche == -1) {
			direction = calculerDirectionTouche(ancienneTouche);
		}
		else {
			direction = calculerDirectionTouche(touche);
		}

		// Déplacement du serpent

		if (direction == -1) {
			color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY);
			gotoXY(0, 32);
			cout << "Touche invalide -- Veuillez utiliser les touches w a s d";
			partieTerminee = true;
		}
		else {
			//deplacerSerpentI(direction, posX, posY);
			deplacerSerpentII(direction, serpentX, serpentY, tailleSerpent, sourisX, sourisY, nbSouris);
		}

		// FIN DE LA PARTIE

		// Afin de pouvoir utiliser les lignes de fin de partie avec les deux  fonctions de déplacements
		posY = serpentY[0];
		posX = serpentX[0];

		// Fin de partie en cas de collision avec le mur

		if (posY == 0 || posY == nbLignes || posX == 0 || posX == nbColonnes) {
			color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY);
			gotoXY(0, 33);
			cout << "Vous avez perdu - Fin de la partie";
			partieTerminee = true;
		}

		// Fin de la partie en cas de collision avec le serpent

		for (int i = 1; i < tailleSerpent; i++) { // On commence à 1 pour ne pas comparer avec la tête
			if (posY == serpentY[i] && posX == serpentX[i]) {
				color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY);
				gotoXY(0, 33);
				cout << "Vous avez perdu - Fin de la partie";
				partieTerminee = true;
			}
		}

		// Fin de la partie si toute les souris ont été mangées

		if (nbSouris == 0) {
			gotoXY(0, 33);
			cout << "Vous avez gagne !!! Bravo ";
			partieTerminee = true;
		}

		// incrément du nombre de tour
		tour++;

	}  // ------------------------Fin de la boucle de la partie --------------------

	Sleep(1000);
	// Pour envoyer le "appuyer... à une autre ligne
	color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY);
	gotoXY(0, 34);
	return 0;
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
	cadre(0, 0, nbColonnes, nbLignes, FOREGROUND_GREEN);

	for (int ligne = 1; ligne < nbLignes; ligne++)	//	parcours des lignes
		for (int colonne = 1; colonne < nbColonnes; colonne++) {	//	parcours des colonnes
			gotoXY(colonne, ligne);
			color(BACKGROUND_GREEN);
			cout << " ";
		}
}

int recupererTouche()

//Tâche: tester si le joueur a appuyé sur une touche
//Retour: retourner le code ASCII de la touche pressée, -1 sinon

{
	// 	utiliser la fonction _kbhit() de conio.h qui retourne une valeur non
	//	nulle si une touche a été pressée

	int touche;
	if (_kbhit() != 0)
		touche = _getch();
	else {
		touche = -1;
	}
	return touche;
}

int calculerDirectionTouche(int touche)
/*
Tâche: calculer la direction correspondant à une touche ou -1
Paramètre: le code d'une touche (w, a, s ou d)
Retour: la direction qui correspond à la touche
(0: droite, 1: gauche, 2: haut, 3: bas)
*/
{
	int direction;
	switch (touche) {
	case 97: direction = 1;
		break;
	case 100: direction = 0;
		break;
	case 115: direction = 3;
		break;
	case 119:direction = 2;
		break;
	default: direction = -1;
	}
	return direction;
}

void positionAleatoire(int nbLignes, int nbColonnes, int &posX, int &posY)
/*
Tâche: calculer une position aléatoire sur le terrain
Paramètres: les dimensions du terrain en entrée et les coordonnées de la
position aléatoire en sortie
*/
{
	posY = rand() % (nbLignes - 2) + 1;
	posX = rand() % (nbColonnes - 2) + 1;
}

void deplacerSerpentI(int direction, int &posX, int &posY)
/*
Tâche: déplacer le serpent d'une seule case dans la direction donnée. Le
serpent est à l'écran avant l'appel et au retour de la fonction
Paramètres: la direction du serpent en entrée, et la position du serpent en
entrée / sortie
*/
{
	gotoXY(posX, posY);
	color(BACKGROUND_GREEN);
	cout << " ";

	if (direction == 0)
		posX++;
	else if (direction == 1)
		posX--;
	else if (direction == 2)
		posY--;
	else
		posY++;

	gotoXY(posX, posY);
	color(FOREGROUND_RED + BACKGROUND_GREEN);
	cout << dessinSerpent;
}




// *************
// * PARTIE II *
// *************


int saisirNiveau()

//Tâche: lire le niveau de difficulté avec tests de validation d'entrée
//Retour: le niveau (= le nombre de souris initialement sur le terrain)

{
	int niveau;

	color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY);
	gotoXY(0, 32);
	cout << "Niveau de 1 a 20: ";
	cin >> niveau;
	return niveau;
}

void creerSouris(int nbLignes, int nbColonnes,
	int sourisX[], int sourisY[], int nbSouris)
	/*
	Tâche: générer les nbSouris aléatoirement sur le terrain
	Paramètres: les dimensions du terrain, les tableaux de coordonnées et le
	nombre de souris
	*/
{
	for (int i = 0; i < nbSouris; i++) {
		sourisY[i] = rand() % (nbLignes - 2) + 1;
	}

	for (int i = 0; i < nbSouris; i++) {
		sourisX[i] = rand() % (nbColonnes - 2) + 1;
	}
}

void afficherSouris(int sourisX[], int sourisY[], int nbSouris)
/*
Tâche: afficher les souris
Paramètres: les tableaux de coordonnées et le nombre de souris
*/
{
	// Faire apparaître les souris

	for (int i = 0; i < nbSouris; i++) {
		gotoXY(sourisX[i], sourisY[i]);
		color(FOREGROUND_BLUE + BACKGROUND_GREEN);
		cout << dessinSouris;
	}
}

void deplacerSerpentII(int direction, int serpentX[], int serpentY[],
	int &tailleSerpent,
	int sourisX[], int sourisY[], int &nbSouris)
	/*
	Tâche: déplacer le serpent d'une seule case dans la direction donnée. Le
	serpent est à l'écran avant l'appel et au retour de la fonction
	Paramètres: en entrée : 	la direction du serpent,
	en entrée/sortie :	les tableaux de coordonnées du serpent,
	la taille du serpent,
	les tableaux de coordonnées des souris et
	le nombre de souris
	*/
{
	// Effacer le serpent

	int indice = 0;
	while (indice < tailleSerpent) {
		gotoXY(serpentX[indice], serpentY[indice]);
		color(BACKGROUND_GREEN);
		cout << " ";
		indice++;
	}

	// Déplacer les coordonnées

	indice = tailleSerpent;
	while (indice >= 1 ) {
		serpentX[indice] = serpentX[indice - 1];
		serpentY[indice] = serpentY[indice - 1];
		indice--;
	}

	// Trouver la nouvelle coordonnée de la tête

	if (direction == 0)
		serpentX[0]++;
	else if (direction == 1)
		serpentX[0]--;
	else if (direction == 2)
		serpentY[0]--;
	else
		serpentY[0]++;

	// Tester s'il y a eu collision avec une souris

	int posX = serpentX[0];
	int posY = serpentY[0];
	bool collision = testerCollision(posX, posY, sourisX, sourisY, nbSouris);
	if (collision == true) {
		tailleSerpent++;
	}

	color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY);
	gotoXY(55, 33);
	cout << "Nb souris: " << nbSouris  << endl;

	// Afficher de nouveau le serpent

	indice = 0;
	while (indice < tailleSerpent) {
		gotoXY(serpentX[indice], serpentY[indice]);
		color(FOREGROUND_RED + BACKGROUND_GREEN);
		cout << dessinSerpent;
		indice++;
	}
}

bool testerCollision(int posX, int posY, int sourisX[], int sourisY[], int &nbSouris)

/**
Tâche: tester si (posX, posY) est la position d'une souris. Si oui, retirer
la souris de la population de souris
Paramètres: la position de la tête du serpent, les tableaux de coordonnées
des souris et le nombre de souris
Retour: true si collision, false sinon
*/
{
	bool collision = false;

	for (int i = 0; i < nbSouris; i++) {
		if (posX == sourisX[i] && posY == sourisY[i]) {
			collision = true;

			for (int j = i; j < nbSouris; j++) {
				sourisX[j] = sourisX[j + 1];
				sourisY[j] = sourisY[j + 1];
			}
			nbSouris--;
		}
	}
	return collision;
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
