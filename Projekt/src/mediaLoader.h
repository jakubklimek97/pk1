/*
 * mediaLoader.h
 *
 *  Created on: 30 kwi 2018
 *      Author: klimek
 */

#ifndef MEDIALOADER_H_
#define MEDIALOADER_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include "bool.h"
#include "texture.h"
/*tablice wskaznikow na tekstury (2 typy - wlasny + sdl'owski */
struct texture** pTexture;
SDL_Texture** pTextures;

/*tablica wskaznikow na czcionki */
TTF_Font** pFonts;

/*Funkcja laduje pliki okreslone w plikach mediaList do pamieci, tworzy tablice wskaznikow na te elementy i ustawia powyzsze wskazniki.
 * Wynikien dzialania funkcji jest informacja, czy udalo sie wszystko zaladowac */
bool loadMedia();

/*Funkcja zwraca wskaznik teksture o podanej nazwie. Parametry:
 * mediaNumber - nazwa tekstury
 */
SDL_Texture* getTexture(int mediaNumber);

/*Funkcja zwraca wskaznik na czcionke o podanej nazwie. Parametry:
 * fontNumber - nazwa czcionki
 */
TTF_Font* getFont(int fontNumber);

/*Funkcja usuwa z pamieci wszystkie elementy, ktore zostaly zaladowane przez funkcje loadMedia()
*/
void unloadMedia();

#endif /* MEDIALOADER_H_ */
