/*
 * inputBox.h
 *
 *  Created on: 21 maj 2018
 *      Author: klimek
 *
 */

#ifndef INPUTBOX_H_
#define INPUTBOX_H_
#include "bool.h"
#include "texture.h"
#include "mediaList.h"
/*Struktura przechowujaca informacje o polu o zmiennej zawartosci (tekstu) */
struct inputBox{
	char* textField;
	size_t textFieldSize;
	int charsWritten;
	bool updateNeeded;
	struct texture* texture;
	struct texture* backgroundTexture;
	bool isVisible;
	SDL_Point position;
	enum fontList font;
	SDL_Color color;
};
/*Funkcja tworzy nowe pole i zwraca jego adres. Parametry:
 * textFieldSize - ile (maksymalnie) liter na przechowywac pole
 * x,y - pozycja pola na ekranie (lewy, gorny rog)
 * isVisible - czy pole ma byc widoczne
 * font - nazwa czcionki, ktorej pole ma uzywac do wyswietlania tekstu
 * r,g,b - kolor czcionki
 * addBackground - czy pole ma tlo
 * defaultTexture - nazwa tekstury, ktora ma byc tlem dla pola (zaladowana podczas ladowania programu).
 * Jezeli wczesniejszy parametr wynosil false, wartosc tego pola nie ma znaczenia
 */
struct inputBox* createInputBox(size_t textFieldSize, int x, int y, bool isVisible, enum fontList font,uint8_t r, uint8_t g, uint8_t b, bool addBackground, enum mediaList defaultTexture);
/*Funkcja zwalnia z pamieci pole. Parametry:
 * pInputBox - adres pola
 */
void destroyInputBox(struct inputBox* pInputBox);
/*Funkcja dodaje do pola jeden znak. Parametry:
 * pInputBox - adres pola
 * character - znak, ktory ma zostac dodany do pola
 */
void inputBoxAppendChar(struct inputBox* pInputBox, char character);
/* Funkcja usuwa z pola jeden znak (o ile taki istnieje). Parametry:
 * pinputBox - adres pola
 */
void inputBoxPopCharacter(struct inputBox* pInputBox);
/*Funkcja renderuje pole na domyslnym ekranie. Parametry:
 * pInputBox - adres pola
 */
void inputBoxRender(struct inputBox* pInputBox);
/*Funkcja wlacza/wylacza widocznosc pola. Parametry:
 * pInputBox - adres pola
 */
void inputBoxToggleVisibility(struct inputBox* pInputBox);
/*Funkcja czysci zawartosc pola (Jako zawartosc rozumiemy ciag znakow, ktory pole wyswietla). Parametry:
 * pInputBox - adres pola
 */
void inputBoxClearTextField(struct inputBox* pInputBox);
#endif /* INPUTBOX_H_ */
