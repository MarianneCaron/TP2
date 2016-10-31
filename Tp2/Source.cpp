#include <iostream>
using namespace std;

#include <conio.h>	// pour _kbhit() et _getch()
#include <time.h>	// pour time(int)
#include <stdlib.h>	// pour srand(int) et rand()
#include "RockUtiles.h"	// pour la gestion de l'�cran

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
bool testerCollision(int x, int y, int sourisX[], int sourisY[], int &nbSouris);


// Partie III (BONUS)
// ------------------
void deplacerSouris(int sourisX[], int sourisY[], int nbSouris);


// AJOUT DE FONCTIONS SUPPL�MENTAIRES

int validerNumerique();
int validerPlage(int borneMin, int borneMax);
char oui_non(); // Va servir � saisir les r�ponses oui/non et uniformise celles-ci en majuscule


// D�claration des constantes (caract�res ASCII utilis� pour dessiner le serpent et les souris
const  char dessinSouris = 15;
const  char dessinTeteSerpent = 2;
const  char dessinCorpsSerpent = 254;

//  fonction principale
//  -------------------
int main()
{
	bool gain; // boolean afin de retenir si le joeur a gagne (donc peut passer au prochain niveau) ou perdu (doit reprendre le niveau)
	bool quitter = false; // Pour arr�ter la boucle si le joueur veut quitter la partie
	char reponse; // Pour conserver les r�ponse en oui/non

	// Affichage du d�but du jeu
	gotoXY(10, 10);
	cout << "              LE JEU DU SERPENT";
	
	// ---- D�terminer le niveau de la partie 
	gotoXY(10, 15);
	cout << "A quel niveau desirez-vous commencer?: ";
	int niveau = saisirNiveau();



	// -------------------------------------- Boucle pour l'option de rejouer -------------------------------
	do
	{
		// Affichage des variables
		bool partieTerminee = false; // Variable pour d�terminer la fin de la partie (la boucle while dans le code)
		int touche, direction; // Variables pour conserver la touche et la direction du serpent
		int tailleSerpent = 1;

		// D�terminer le srand() pour les valeurs al�atoires
		srand((unsigned int)time(NULL));

		// D�claration des tableaux pour les souris
		int sourisX[20], sourisY[20];


		//----------------------------------------AFFICHAGE DU JEU -----------------------------------------------

		//D�terminer la largeur de la fenetre � appara�tre  -- utilise la fonction setDimensionFenetre de RockUtiles
		setDimensionFenetre(0, 0, 75, 40);

		// Affichage du terrain -- Utilise la fonction afficher terrain de la partie 1
		int nbLignes = 30, nbColonnes = 70;
		afficherTerrain(nbLignes, nbColonnes);

		// D�terminer le nombre de souris
		int nbSouris = niveau; // Le niveau correspond au nombre de souris � afficher
		//int nbMurs = niveau * 2; 

		// ---- Cr�er et afficher les souris selon le niveau s�lectionn�
		creerSouris(nbLignes, nbColonnes, sourisX, sourisY, nbSouris);
		afficherSouris(sourisX, sourisY, nbSouris);

		// Faire appara�tre le serpent
		// D�terminier la position
		int posX = 0, posY = 0;
		positionAleatoire(nbLignes, nbColonnes, posX, posY);
		gotoXY(posX, posY);

		// D�claration et initialisation des tableaux pour les coordonn�es du serpent
		int serpentX[100] = { posX };
		int serpentY[100] = { posY };

		//Afficher le serpent 
		color(FOREGROUND_RED + BACKGROUND_GREEN);
		cout << dessinTeteSerpent;

		// Vitesse selon le niveau
		int delaiSepent = 460 - 20 * (niveau - 1);

		// D�claration et initialisation du nombre de tours (pour distinguer le premier tour des subs�quents)
		int tour = 0;

		// D�claration et initialisation d'une variable pour conserver en m�moire la derni�re touche appuy�e valide. 
		int ancienneTouche = 0;



		// -------------------------------------------Boucle de la partie--------------------------------

		while (!partieTerminee) {

			Sleep(delaiSepent); // Afin de ralentir le jeu - Va varier selon le niveau

			// R�cup�rer la touche		

			if (tour == 0) { // Pour attendre que le joeur ait appuyer sur sa premi�re touche
				do {
					touche = recupererTouche();
				} while (touche == -1);
			}
			else {
				if (touche != -1) {
					ancienneTouche = touche; // Conserver en m�moire la derniere touche valide
				}
				touche = recupererTouche();
			}

			// D�terminer la direction

			if (touche == -1) {
				direction = calculerDirectionTouche(ancienneTouche); // s'il n'y a pas de touche appuy�, va utiliser la derni�re touche en m�moire pour calculer la direction
			}
			else {
				direction = calculerDirectionTouche(touche); // S'il y a une nouvelle touche, on va calculer la destination avec celle-ci
			}

			// D�placement des souris

			if (tour % 5 == 0) { // Ralentissement pour que les souris bougent moins vite que le serpent
				deplacerSouris(sourisX, sourisY, nbSouris);
			}

			// D�placement du serpent

			if (direction == -1) { // Traitement des cas des touches invalides
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

			// Afin de pouvoir utiliser les lignes de fin de partie avec les deux  fonctions de d�placements
			posY = serpentY[0];
			posX = serpentX[0];

			// Fin de partie en cas de collision avec les limites du terrain

			if (posY == 0 || posY == nbLignes || posX == 0 || posX == nbColonnes) {
				color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY);
				gotoXY(0, 33);
				cout << "Vous avez perdu - Fin de la partie";
				partieTerminee = true;
				gain = false;
			}

			// Fin de la partie en cas de collision avec le serpent

			for (int i = 1; i < tailleSerpent; i++) { // On commence � 1 pour ne pas comparer avec la t�te
				if (posY == serpentY[i] && posX == serpentX[i]) {
					color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY);
					gotoXY(0, 33);
					cout << "Vous avez perdu - Fin de la partie";
					partieTerminee = true;
					gain = false;
				}
			}


			// Fin de la partie si toute les souris ont �t� mang�es

			if (nbSouris == 0) {
				gotoXY(0, 33);
				cout << "Vous avez gagne !!! Bravo ";
				partieTerminee = true;
				gain = true;
			}

			// incr�ment du nombre de tour
			tour++;

		}  // ------------------------Fin de la boucle de la partie --------------------

		// Traitement afin de reprendre un niveau si le joueur a perdu

		if (gain == false) {
			cout << endl << "Voulez- vous reprendre le niveau (O/N)?";
			reponse = oui_non();
			if (reponse == 'N') {
				quitter = true;
			}
		}
		else { // Traitement lorsque le joueur a gagne le niveau
			if (niveau == 20) { // S'il est au dernier niveau, on lui demande a quel niveau il veut reprendre
				cout << endl << "Dernier niveau termine - Fin de la partie" << endl;
				cout << "Voulez- vous continuer a jouer?:";
				reponse = oui_non();
				if (reponse == 'N') {
					quitter = true;
				}
				else {
					cout << endl << " A quel niveau desirez-vous recommencer ? : ";
					niveau = saisirNiveau();
				}
			}
			else { // Pour les niveaux de 1 � 19, on sugg�re de continuer au prochain niveau
				cout << endl << "Voulez-vous jouer au prochain niveau (O/N)?";
				reponse = oui_non();
				if (reponse == 'N') {
					quitter = true;
				}
				else {
					niveau++;
				}
			}
		}

		system("cls"); // Nettoie l'�cran avant de reprendre la boucle d'Affichage et de partie. 

	} while (!quitter); //---------------------------- FIN DE LA BOUCLE avec l'option rejouer---------------------------------



	// Pour envoyer le "appuyer... � une autre ligne
	color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY);
	gotoXY(20, 15);
	cout << "Merci d'avoir joue !! - A la prochaine" << endl;
	return 0;
}





// ************
// * PARTIE I *
// ************
void afficherTerrain(int nbLignes, int nbColonnes)
/*
T�che: afficher le contour du terrain
Param�tres: le nombre de lignes et de colonnes du terrain rectangulaire
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

//T�che: tester si le joueur a appuy� sur une touche
//Retour: retourner le code ASCII de la touche press�e, -1 sinon

{
	// 	utiliser la fonction _kbhit() de conio.h qui retourne une valeur non
	//	nulle si une touche a �t� press�e

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
T�che: calculer la direction correspondant � une touche ou -1
Param�tre: le code d'une touche (w, a, s ou d)
Retour: la direction qui correspond � la touche
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
T�che: calculer une position al�atoire sur le terrain
Param�tres: les dimensions du terrain en entr�e et les coordonn�es de la
position al�atoire en sortie
*/
{
	posY = rand() % (nbLignes - 2) + 1;
	posX = rand() % (nbColonnes - 2) + 1;
}

void deplacerSerpentI(int direction, int &posX, int &posY)
/*
T�che: d�placer le serpent d'une seule case dans la direction donn�e. Le
serpent est � l'�cran avant l'appel et au retour de la fonction
Param�tres: la direction du serpent en entr�e, et la position du serpent en
entr�e / sortie
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
	cout << dessinTeteSerpent;

}




// *************
// * PARTIE II *
// *************


int saisirNiveau()

//T�che: lire le niveau de difficult� avec tests de validation d'entr�e
//Retour: le niveau (= le nombre de souris initialement sur le terrain)

{
	int niveau;

	color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY);
	gotoXY(0, 32);
	cout << "Niveau de 1 a 20: ";
	niveau = validerPlage(1, 20);
	return niveau;
}

void creerSouris(int nbLignes, int nbColonnes,
	int sourisX[], int sourisY[], int nbSouris)
	/*
	T�che: g�n�rer les nbSouris al�atoirement sur le terrain
	Param�tres: les dimensions du terrain, les tableaux de coordonn�es et le
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
T�che: afficher les souris
Param�tres: les tableaux de coordonn�es et le nombre de souris
*/
{
	// Faire appara�tre les souris

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
	T�che: d�placer le serpent d'une seule case dans la direction donn�e. Le
	serpent est � l'�cran avant l'appel et au retour de la fonction
	Param�tres: en entr�e : 	la direction du serpent,
	en entr�e/sortie :	les tableaux de coordonn�es du serpent,
	la taille du serpent,
	les tableaux de coordonn�es des souris et
	le nombre de souris
	*/
{
	// Effacer le serpent

	gotoXY(serpentX[0], serpentY[0]);
	color(FOREGROUND_RED + BACKGROUND_GREEN);
	cout << dessinCorpsSerpent;

	gotoXY(serpentX[tailleSerpent - 1], serpentY[tailleSerpent - 1]);
	color(BACKGROUND_GREEN);
	cout << " ";


	// D�placer les coordonn�es
	int indice = 0;
	indice = tailleSerpent;
	while (indice >= 1) {
		serpentX[indice] = serpentX[indice - 1];
		serpentY[indice] = serpentY[indice - 1];
		indice--;
	}

	// Trouver la nouvelle coordonn�e de la t�te

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
		gotoXY(serpentX[tailleSerpent - 1], serpentY[tailleSerpent - 1]);
		color(FOREGROUND_RED + BACKGROUND_GREEN);
		cout << dessinCorpsSerpent;
	}

	// Afficher la  nouvelle t�te du serpent

	gotoXY(serpentX[0], serpentY[0]);
	color(FOREGROUND_RED + BACKGROUND_GREEN);
	cout << dessinTeteSerpent;

	// Afficher le nombre de souris restantes
	color(FOREGROUND_BLUE + FOREGROUND_GREEN + FOREGROUND_RED + FOREGROUND_INTENSITY);
	gotoXY(40, 32);
	cout << "Nombre de souris restantes: " << nbSouris << "      " << endl;
}

bool testerCollision(int posX, int posY, int sourisX[], int sourisY[], int &nbSouris)

/**
T�che: tester si (posX, posY) est la position d'une souris. Si oui, retirer
la souris de la population de souris
Param�tres: la position de la t�te du serpent, les tableaux de coordonn�es
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
T�che: d�placer les souris (al�atoirement ou intelligemment)
Param�tres: les tableaux de coordonn�es et le nombre de souris
*/
{

	bool bonneDestination = false;

	for (int i = 0; i < nbSouris; i++) {

		gotoXY(sourisX[i], sourisY[i]);
		color(BACKGROUND_GREEN);
		cout << " ";

		bonneDestination = false;

		while (!bonneDestination) {
			int directionAleatoire = rand() % 4;
			switch (directionAleatoire) {
			case 0: if (sourisX[i] < 68) sourisX[i]++;
				break;
			case 1:if (sourisX[i] > 2) sourisX[i]--;
				break;
			case 2: if (sourisY[i] > 2) sourisY[i]--;
				break;
			default:if (sourisY[i] < 28)sourisY[i]++;
			}

			if (getCharXY(sourisX[i], sourisY[i]) == ' ')
				bonneDestination = true;
		}
		gotoXY(sourisX[i], sourisY[i]);
		color(FOREGROUND_BLUE + BACKGROUND_GREEN);
		cout << dessinSouris;
	}
}





//*********************************
//    FONCTION SUPPL�MENTAIRES
//*********************************

// Fonction pour la validation num�rique des saisies
int validerNumerique() {
	int n;
	bool estValide = false;

	do
	{
		cin >> n;

		if (cin.fail() || cin.peek() != '\n') {
			cin.clear();
			cin.ignore(512, '\n');
			cout << "Ceci n'est pas un nombre valide" << endl;
		}
		else {
			estValide = true;
		}
	} while (estValide == false);

	return n;
}

// Fonction pour la validation des plages de la saisie
int validerPlage(int borneMin, int borneMax) {
	int nombreSaisi;
	bool estValide;

	do
	{
		nombreSaisi = validerNumerique();
		if (nombreSaisi<borneMin || nombreSaisi>borneMax) {
			cout << "Ce nombre n'est pas entre " << borneMin << " et " << borneMax << "." << endl;
			estValide = false;
		}
		else {
			estValide = true;
		}
	} while (estValide == false);

	return nombreSaisi;
}

// Fonction pour valider et uniformiser les r�ponses en oui/non

char oui_non() {
	char caractere;
	do
	{
		caractere = toupper(_getche());
		if ((caractere != 'O') && (caractere != 'N')) { //Faire afficher un message d'erreur 
			cout << endl << "Valeur invalide! - Veuillez entrer 'o' ou 'n': ";
		}
	} while ((caractere != 'O') && (caractere != 'N'));
	return caractere;
}
