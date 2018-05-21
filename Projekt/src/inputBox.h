/*
 * inputBox.h
 *
 *  Created on: 21 maj 2018
 *      Author: klimek
 *
 *  TODO:
 *  usun inputBox
 *  aktualizuj teksture inputboxa
 *  renderuj inputBox
 *  dodaj znak do inputBoxa
 *  usun znak z inputBoxa
 */

#ifndef INPUTBOX_H_
#define INPUTBOX_H_
#include "bool.h"
#include "texture.h"
#include "mediaList.h"

struct inputBox{
	char* textField;
	size_t textFieldSize;
	int charsWritten; /*ilosc znakow zapisanych w polu tekstowym*/
	bool updateNeeded;
	struct texture* texture;
	struct texture* backgroundTexture;
	bool isVisible;
	SDL_Point position;
};

struct inputBox* createInputBox(size_t textFieldSize, int x, int y, bool isVisible, bool addBackground, enum mediaList defaultTexture){
	struct inputBox* pTmp = malloc(sizeof(struct inputBox));
	pTmp->updateNeeded = false;
	pTmp->isVisible = isVisible;
	pTmp->charsWritten = 0;
	pTmp->texture = malloc(sizeof(struct texture));
	pTmp->position.x = x;
	pTmp->position.y = y;
	pTmp->textField = malloc(sizeof(char)*textFieldSize);
	pTmp->textFieldSize = textFieldSize;

	if(addBackground == true){
		pTmp->backgroundTexture = pTexture[defaultTexture];
	}
	else{
		pTmp->backgroundTexture = NULL;
	}

	return pTmp;
}
#endif /* INPUTBOX_H_ */
