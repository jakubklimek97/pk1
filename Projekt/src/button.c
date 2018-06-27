/*
 * button.c
 *
 *  Created on: 8 maj 2018
 *      Author: klimek
 */
#include <stdlib.h>
#include "button.h"
#include "mediaLoader.h"
struct button* createButton(int x, int y, enum mediaList defaultTexture,  enum mediaList mouseoverTexture, enum mediaList pressedTexture, void (*pFunction)()){
	struct button* pButton = malloc(sizeof(struct button));
	pButton->position.x = x;
	pButton->position.y = y;
	pButton->bTexture[BUTTON_DEFAULT] = getTextureStr(defaultTexture);
	pButton->bTexture[BUTTON_MOUSEOVER] = getTextureStr(mouseoverTexture);
	pButton->bTexture[BUTTON_PRESSED] = getTextureStr(pressedTexture);
	pButton->width = pButton->bTexture[BUTTON_DEFAULT]->width;
	pButton->height = pButton->bTexture[BUTTON_DEFAULT]->height;
	pButton->state = BUTTON_DEFAULT;
	pButton->pFunction = (void(*)())pFunction;
	pButton->isVisible = true;
	return pButton;
}
void destroyButton(struct button* pointer){
	if(pointer != NULL){
		free(pointer);
	}
}
void renderButton(struct button* pButton){
	if(pButton && pButton->isVisible)/*Renderuj tylko wtedy, gdy przycisk jest widoczny*/
		renderTexture(pButton->bTexture[pButton->state], pButton->position.x, pButton->position.y);
}
void handeEvent(struct button* btn, SDL_Event* e){
	if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEBUTTONDOWN){ /*Jezeli event dotyczy myszki*/
		int x,y;
		SDL_GetMouseState(&x, &y); /*pobierz pozycje kursora*/
		bool inside = true;/*Na poczatek przyjmujemy, ze kursor znajduje sie w przycisku*/
		if(x < btn->position.x){/*Jezeli jest na lewo od przycisku*/
			inside = false;
		}
		else if(x > btn->position.x + btn->width){/*Jezeli jest na prawo od przycisku*/
			inside = false;
		}
		else if(y < btn->position.y){/*Jezeli jest nad przyciskiem*/
			inside = false;
		}
		else if(y > btn->position.y + btn->height){/*Jezeli jest pod przyciskiem*/
			inside = false;
		}
		if(inside == false){/*Mysz poza przyciskiem*/
			btn->state = BUTTON_DEFAULT;/*Przycisk w stanie domyslnym*/
		}
		else{/*Jezeli mysz jest w przycisku, sprawdz rodzaj eventu*/
			switch(e->type){
			case SDL_MOUSEMOTION: btn->state = BUTTON_MOUSEOVER;break;/*mysz pojawila sie nad przyciskiem, zmiana statusu na mouseover*/
			case SDL_MOUSEBUTTONDOWN: btn->state = BUTTON_MOUSEOVER; if(btn->pFunction != NULL) btn->pFunction(); break;/*Mysz kliknela, zmiana statusu na pressed i wykonanie funkcji przypisanej do przycisku*/
			case SDL_MOUSEBUTTONUP: btn->state = BUTTON_MOUSEOVER;break; /*Przycisk zostal zwolniony, wystarczy zmiana statusu*/
			}
		}
	}
}
void changeButtonTexture(struct button* pButton, enum buttonState state, enum mediaList texture){
	pButton->bTexture[state] = getTextureStr(texture);
}
void toggleButtonVisibility(struct button* pButton){
	pButton->isVisible = !(pButton->isVisible);
}
