/*
 * button.c
 *
 *  Created on: 8 maj 2018
 *      Author: klimek
 */

#include "button.h"
#include "mediaLoader.h"
struct button* createButton(int x, int y, enum mediaList defaultTexture,  enum mediaList mouseoverTexture, enum mediaList pressedTexture){
	struct button* pButton = malloc(sizeof(struct button));
	pButton->position.x = x;
	pButton->position.y = y;
	pButton->bTexture[BUTTON_DEFAULT] = pTexture[defaultTexture];
	pButton->bTexture[BUTTON_MOUSEOVER] = pTexture[mouseoverTexture];
	pButton->bTexture[BUTTON_PRESSED] = pTexture[pressedTexture];
	pButton->state = BUTTON_DEFAULT;
	return pButton;
}
void renderButton(struct button* pButton){
	renderTexture(pButton->bTexture[pButton->state], pButton->position.x, pButton->position.y);
}
