/*
 * button.h
 *
 *  Created on: 8 maj 2018
 *      Author: klimek
 */

#ifndef BUTTON_H_
#define BUTTON_H_
#include <SDL2/SDL.h>
#include "mediaList.h"
#include "texture.h"
#include "bool.h"
/*stany przycisku*/
enum buttonState {
	BUTTON_DEFAULT,
	BUTTON_MOUSEOVER,
	BUTTON_PRESSED
};
/* struktura przycisku */
struct button {
	SDL_Point position;
	enum buttonState state;
	int width;
	int height;
	struct texture* bTexture[3];
	void (*pFunction)(void);
	bool isVisible;
};
/*funkcja tworzy nowy przycisk i zwraca jego adres. Parametry:
 * x,y - pozycja przycisku (lewo gorny rog)
 * defaultTexture - nazwa domyslnej tekstury przycisku
 * mouseoverTexture - nazwa tekstury, ktora ma sie wyswietlic po najechaniu na przycisk myszka
 * pressedTexture - nazwa tekstury, ktora ma sie wyswietlic po kliknieciu przycisku
 * pFunction - wskaznik na funkcje, ktora ma sie wykonac po nacisnieciu przycisku
 */
struct button* createButton(int x, int y, enum mediaList defaultTexture,  enum mediaList mouseoverTexture, enum mediaList pressedTexture, void (*pFunction)());
/*funkcja renderuje przycisk na domyslnym ekranie. Pamametry:
 * pButton - adres przycisku, ktory ma zostac wyrenderowany
 */
void renderButton(struct button* pButton);
/*Funkcja sprawdza, czy event ktory wystapil dotyczy przycisku. Jezeli tak, wykonuje funkcje,
 * ktorej adres zostal przekazany podczas tworzenia przycisku. Parametry:
 * btn - adres przycisku
 * e - wskaznik na zmienna przechowujaca informacje o evencie
 */
void handeEvent(struct button* btn, SDL_Event* e);
/*Funkcja usuwa przycisk z pamieci. Parametry:
 * pointer - adres przycisku
 */
void destroyButton(struct button* pointer);
/* Funkcja zmienia teksture przycisku na inna teksture (tekstura musi zostac zaladowana podczas uruchamiania).
 * Parametry:
 * pButton - adres przycisku
 * state - stan przycisku, dla ktorego teksture chcemy zmienic
 * texture - nazwa tekstury, ktora chcemy ustawic dla danego stanu
 */
void changeButtonTexture(struct button* pButton, enum buttonState state, enum mediaList texture);
/*Funkcja wylacza/wlacza widocznosc przycisku (domyslnie przycisk jest widoczny). Parametry:
 * pButton - adres przycisku
 */
void toggleButtonVisibility(struct button* pButton);

#endif /* BUTTON_H_ */
