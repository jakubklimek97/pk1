/*
 * texture.h
 *
 *  Created on: 8 maj 2018
 *      Author: klimek
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <SDL2/SDL.h>
#include "bool.h"
/*struktura reprezentujaca teksture */
struct texture{
	int width;
	int height;
	SDL_Texture* lTexture;
};

/*Funkcja wyswietlajaca teksture na domyslnym wyswietlaczu. Parametry:
 * pTexture - adres tekstury
 * x,y - wspolrzedne tekstury (lewy gorny rog)
 */
void renderTexture(struct texture* pTexture, int x, int y);

/*Funkcja tworzy nowa teksture zawierajaca podany tekst i zapisuje ja.
 * Wynikiem funkcji jest informacja, czy udalo sie utworzyc teksture.
 * Parametry:
 * pTexture - adres, pod ktorym ma byc dostepna tekstura (jezeli znajduje sie tam inna tekstura, usuwa ja)
 * fontName - nazwa czcionki, ktora ma zostac uzyta
 * text - ciag znakowy, ktory ma wystapic na teksturze
 * textColor - kolor czcionki
 */
bool createFromText(struct texture* pTexture, int fontName, const char* text, SDL_Color textColor);

/*Funkcja usuwa z pamieci teksture. Parametry:
 * pTexture - adres tekstury
 */
void destroyTexture(struct texture* pTexture);

#endif /* TEXTURE_H_ */
