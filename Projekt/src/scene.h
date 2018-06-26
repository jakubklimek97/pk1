/*
 * scene.h
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */

#ifndef SCENE_H_
#define SCENE_H_
#include <SDL2/SDL.h>
#include "sceneList.h"
/*struktura reprezentujaca scene.
 *sklada sie z czterech wskaznikow na funkcje, ktore sa uzywane przez glowna logike gry
*/
struct scene{
	void (*handleEvents)(SDL_Event *e);
	void (*renderScene)();
	void (*initScene)();
	void (*destroyScene)();
};

/*typ wyliczeniowy na aktualnie wybrana scene */
enum sceneList selectedScene;
/*adres na aktualnie wybrana scene */
struct scene* currScene;

/*Funkcja ustawiajaca dana scene na aktualna.
 * Podczas zmiany sceny nastepuje usuniecie z pamieci wczesniejszej sceny i zaladowanie nastepnej.
 * Parametry:
 * scene - nazwa sceny
 */
void selectScene(enum sceneList scene);

#endif /* SCENE_H_ */
