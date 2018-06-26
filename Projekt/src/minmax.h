/*
 * minmax.h
 *
 *  Created on: 25 maj 2018
 *      Author: klimek
 */
#ifndef MINMAX_H_
#define MINMAX_H_
#include "bool.h"
/*typ wyliczeniowy wskazujacy na zwyciezce */
enum whoWon{
	OPPONENT = -10,
	NONE = 0,
	PLAYER = 10
};/*typ wyliczeniowy okreslajacy symbole graczy */
enum signs {
	X = 'x',
	O = 'o'
};
/*struktura zawierajaca nastepny preferowany ruch*/
struct minMax_Move{
	int row, col;
};
/*znaki gracza i przeciwnika*/
char player;
char opponent;
/*poprzednia plansza */
char prevBoard[3][3];
/*aktualna plansza */
char board[3][3];

/*Funkcja sprawdza, czy na planszy istnieje wolne pole i zwraca wynik tej operacji. Parametry:
 * board - plansza, ktorej dotyczy sprawdzenie
 */
bool isEmptyPlace(char board[3][3]);

/*Funkcja sprawdza, czy ktos wygral gre. Parametry:
 * board - plansza, ktorej dotyczy sprawdzenie
 */
enum whoWon lookForWinner(char board[3][3]);

/*Funkcja sprawdza, jak dobra dla danego gracza jest plansza. Zwraca wynik w postaci punktowej. Parametry:
 * board - plansza, ktorej dotyczy sprawdzenie
 * depth - "glebokosc" pomiaru
 * isMaximalising - czy oczekujemy najwiekszego czy najmniejszego wyniku
 */
int minMax(char board[3][3], int depth, bool isMaximalising);

/*Funkcja ma za zadanie wyszukac najlepszy ruch dla komputera. Parametry
 * board - plansza, na ktorej rozgrywa komputer
 */
struct minMax_Move findBestMove(char board[3][3]);

/*Funkcja ustawia komputerowi znak. Parametry
 * sign - znak komputerowego gracza
 */
void setupBot(enum signs sign);
#endif /* MINMAX_H_ */
