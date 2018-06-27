/*
 * textureList.h
 *
 *  Created on: 27 cze 2018
 *      Author: klimek
 */

#ifndef TEXTURELIST_H_
#define TEXTURELIST_H_
#include "texture.h"
#include "SDL2/SDL.h"

/*Struktura eeenty listy*/
struct textureList{
	unsigned int id;
	struct texture* pElement;
	struct textureList* pNext;
};
/*Funkcja dodaje teksture do listy. Parametry
 * root - glowa listy
 * id - nazwa(id) tekstury
 * pTexture - wskaznik na teksture
 * Funkcja zwraca adres nowo utworzonego elementu
 */
struct textureList* listAddTexture(struct textureList* root, unsigned int id, struct texture* pTexture);

/*Funkcja dodaje do listy element o danym id, ale z pusta tekstura. Parametry:
 * root - glowa listy
 * id - nazwa(id) elementu(tekstury).
 * Funkcja zwraca adres nowo utworzonego elementu
 */
struct textureList* listAddElement(struct textureList* root, unsigned int id);

/*Funkcja szuka w liscie elementu o danym id i zwraca jego adres (lub NULL, jezeli taki nie istnieje). Parametry:
 * root - glowa listy
 * id - numer(id) elementu(tekstury)
 */
struct textureList* listGetElement(struct textureList* root, unsigned int id);

/*Funkcja dziala analogicznie do listGetElement(...), z ta roznica, ze zwraca adres tekstury, ktora przechowywal element
 */
struct texture* listGetTexture(struct textureList* root, unsigned int id);

/*Funkcja usuwa z listy element o danym id i zwraca adres wczesniejszego elementu. Parametry:
 * root - glowa listy(moze ulec zmianie jezeli usuwamy pierwszy element)
 * id - nazwa(id) elementu
 */
struct textureList* listRemoveItem(struct textureList** root, unsigned int id);

/*Funkcja zwalnia pamiec zajmowana przez liste. UWAGA: Zwolnieniu podlegaja takze tekstury, ktore sa przechowywane w liscie. Parametry
 * root - glowa listy
 */
void freeList(struct textureList* root);



#endif /* TEXTURELIST_H_ */
