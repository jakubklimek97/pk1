/*
 * game.h
 *
 *  Created on: 26 cze 2018
 *      Author: klimek
 */

#ifndef GAME_H_
#define GAME_H_
#include <SDL2/SDL.h>
/*Funkcja uruchamia gui i zawiera cala logike gry*/
int run();
/*Funkcja usuwa plik z wynikami i poprzedni zapis gry (o ile istnieja)*/
void restoreDefaults();

/*Funkcja pobiera/ustawia trudnosc gry singlePlayer. Parametry:
 *difficulty - jezeli w przedziale <1;4> - ustawia nowy poziom trudnosci i go zwraca, w innym przypadku - zwraca aktualny poziom trudnosci
 */
int difficulty(int difficulty);
/*Funkcje dajace dostep do zmiennych "globalnych" */
SDL_Window* getWindow();
SDL_Renderer* getRenderer();

#endif /* GAME_H_ */
