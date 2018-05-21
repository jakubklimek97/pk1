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
	pButton->bTexture[BUTTON_DEFAULT] = pTexture[defaultTexture];
	pButton->bTexture[BUTTON_MOUSEOVER] = pTexture[mouseoverTexture];
	pButton->bTexture[BUTTON_PRESSED] = pTexture[pressedTexture];
	pButton->width = pButton->bTexture[BUTTON_DEFAULT]->width;
	pButton->height = pButton->bTexture[BUTTON_DEFAULT]->height;
	pButton->state = BUTTON_DEFAULT;
	pButton->pFunction = (void(*)())pFunction;
	return pButton;
}
void destroyButton(struct button* pointer){
	if(pointer != NULL){
		free(pointer);
	}
}
void renderButton(struct button* pButton){
	if(pButton)
		renderTexture(pButton->bTexture[pButton->state], pButton->position.x, pButton->position.y);
}
void handeEvent(struct button* btn, SDL_Event* e){
	if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEBUTTONDOWN){
		int x,y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;
		if(x < btn->position.x){
			inside = false;
		}
		else if(x > btn->position.x + btn->width){
			inside = false;
		}
		else if(y < btn->position.y){
			inside = false;
		}
		else if(y > btn->position.y + btn->height){
			inside = false;
		}
		if(inside == false){
			btn->state = BUTTON_DEFAULT;
		}
		else{
			switch(e->type){
			case SDL_MOUSEMOTION: btn->state = BUTTON_MOUSEOVER;break;
			case SDL_MOUSEBUTTONDOWN: btn->state = BUTTON_MOUSEOVER; if(btn->pFunction != NULL) btn->pFunction(); break;
			case SDL_MOUSEBUTTONUP: btn->state = BUTTON_MOUSEOVER;break;
			}
		}
	}
}
