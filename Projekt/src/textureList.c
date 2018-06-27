/*
 * textureList.c
 *
 *  Created on: 27 cze 2018
 *      Author: klimek
 */
#include "textureList.h"

struct textureList* listAddElement(struct textureList* root, unsigned int id){
	if(root == NULL){ /*jezeli glowa nie istnieje*/
		 struct textureList* pointer;
		 pointer = malloc(sizeof(struct textureList));
		 pointer->id = id;
		 pointer->pElement = NULL;
		 pointer->pNext = NULL;
		 return pointer;/*zwracany wskaznik to jednoczesnie glowa nowej listy*/
	}
	struct textureList* pointer;
	for(pointer = root; pointer->pNext != NULL; pointer = pointer->pNext);/*Przejdz na koniec listy*/
	/*Wstaw nowy element*/
	pointer->pNext = malloc(sizeof(struct textureList));
	pointer->pNext->id = id;
	pointer->pNext->pElement = NULL;
	pointer->pNext->pNext = NULL;
	return pointer->pNext;/*Zwroc jego adres*/
}
struct textureList* listAddTexture(struct textureList* root, unsigned int id, struct texture* pTexture){
	struct textureList* pointer = listAddElement(root,id);/*Dodaj element bez tekstury*/
	pointer->pElement = pTexture;/*Ustaw mu teksture*/
	return pointer;/*Zwroc jego adres*/
}
struct textureList* listGetElement(struct textureList* root, unsigned int id){
	struct textureList* pointer;
	for(pointer = root; pointer != NULL && pointer->id != id; pointer = pointer->pNext);/*Przeszukaj liste w poszukiwaniu elementu*/
	return pointer;/*Jezeli znaleziono element zwroc jego adres, w przeciwnym wypadku NULL*/
}
struct texture* listGetTexture(struct textureList* root, unsigned int id){
	struct textureList* pointer = listGetElement(root,id);/*Wyszukaj element*/
	return pointer == NULL ? NULL : pointer->pElement;/*Jezeli element nie istnieje zwroc NULL, w przeciwnym wypadku przechowywana teksture*/
}
struct textureList* listRemoveItem(struct textureList** root, unsigned int id){
	if(root == NULL) return NULL;/*Jezeli lista nie istnieje zakoncz*/
	struct textureList* pointer = *root;
	if(pointer != NULL && pointer->id == id){/*Jezeli usuwanym elementem jest glowa*/
		if(pointer->pElement)destroyTexture(pointer->pElement);/*Jezeli tekstura istnieje nalezy ja usunac*/
		*root = pointer->pNext;/*Mamy nowa glowe*/
		free(pointer);
		return *root;
	}
	/*Przeszukaj liste w poszukiwaniu elementu*/
	for(;pointer->pNext != NULL && pointer->pNext->id != id; pointer = pointer->pNext);
	if(pointer->pNext == NULL){/*Jezeli nie znaleziono*/
		return NULL;
	}
	else{/*W przeciwnym wypadku*/
		if(pointer->pNext->pElement)destroyTexture(pointer->pNext->pElement);/*Jezeli tekstura istnieje nalezy ja usunac*/
		struct textureList* pNextElement =  pointer->pNext->pNext;/*Element 1 za usuwanym*/
		free(pointer->pNext);
		pointer->pNext = pNextElement ? pNextElement : NULL;/*Jezeli za usunietym elementem cos bylo, "wstaw" to w miejsce usunietego elementu*/

	}
	return pointer;/*Wskaznik na poprzedzajacy element*/
}
void freeList(struct textureList* root){
	struct textureList* pNext;
	while(root){/*Tak dlugo jak wskaznik na cos pokazuje*/
		pNext = root->pNext;/*Nastepny element*/
		if(root->pElement)destroyTexture(root->pElement);/*Jezeli tekstura istnieje, nalezy ja usunac*/
		free(root);
		root = pNext;/*Ustaw wskaznik na nastepny element (jezeli lista sie skonczyla, zostanie ustawiony NULL)*/
	}

}
