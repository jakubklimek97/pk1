/*
 * sceneMenu.h
 *
 *  Created on: 21 maj 2018
 *      Author: klimek
 */

#ifndef SCENESCOREBOARD_H_
#define SCENESCOREBOARD_H_
#include "scene.h"
#include "sceneList.h"
#include "mediaLoader.h"
#include "mediaList.h"
#include "button.h"
#include "inputBox.h"
#include <SDL2/SDL.h>
#include "bool.h"

extern struct scene scoreboardScene;

/*Funkcja dodaje do tablicy wynikow nowy wynik
 * Informuje o tym, ze gracz wygral. Jezeli przekazany wynik mozna
 * umiescic na tablicy, nalezy zapytac uzytkownika o imie. Parametry:
 * score - wynik uzytkownika
 */
void addScore(int score);

#endif /* SCENESCOREBOARD_H_ */
