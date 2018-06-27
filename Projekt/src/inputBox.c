/*
 * inputBox.c
 *
 *  Created on: 22 maj 2018
 *      Author: klimek
 */
#include "inputBox.h"
#include "mediaLoader.h"

struct inputBox* createInputBox(size_t textFieldSize, int x, int y, bool isVisible, enum fontList font,uint8_t r, uint8_t g, uint8_t b, bool addBackground, enum mediaList defaultTexture){
	struct inputBox* pTmp = malloc(sizeof(struct inputBox));
	pTmp->updateNeeded = false;
	pTmp->isVisible = isVisible;
	pTmp->charsWritten = 0;
	pTmp->texture = malloc(sizeof(struct texture));
	pTmp->texture->lTexture = NULL;
	pTmp->position.x = x;
	pTmp->position.y = y;
	pTmp->textField = malloc(sizeof(char)*(textFieldSize+1));
	memset(pTmp->textField,0,sizeof(char)*(textFieldSize+1));
	pTmp->textFieldSize = textFieldSize;
	pTmp->font = font;
	pTmp->color.a = 255;
	pTmp->color.r = r;
	pTmp->color.g = g;
	pTmp->color.b= b;
	if(addBackground == true){
		pTmp->backgroundTexture = getTextureStr(defaultTexture);
	}
	else{
		pTmp->backgroundTexture = NULL;
	}
	return pTmp;
}
void destroyInputBox(struct inputBox* pInputBox){
	free(pInputBox->textField);
	destroyTexture(pInputBox->texture);
	free(pInputBox);
}
void inputBoxAppendChar(struct inputBox* pInputBox, char character){
	if(pInputBox->charsWritten < pInputBox->textFieldSize){
		pInputBox->textField[pInputBox->charsWritten++] = character;
		pInputBox->updateNeeded = true;
	}
}
void inputBoxPopCharacter(struct inputBox* pInputBox){
	if(pInputBox->charsWritten > 0){
		pInputBox->textField[--(pInputBox->charsWritten)] = '\0';
		if(pInputBox->charsWritten == 0) pInputBox->textField[0] = ' ';/*Nie mozna wyswietlic pustego ciagu znakow*/
		pInputBox->updateNeeded = true;
	}
}
void inputBoxRender(struct inputBox* pInputBox){
	if(pInputBox->isVisible){
		if(pInputBox->updateNeeded == true){
			createFromText(pInputBox->texture,pInputBox->font,pInputBox->textField, pInputBox->color);
			pInputBox->updateNeeded = false;
		}
		if(pInputBox->backgroundTexture != NULL){
			renderTexture(pInputBox->backgroundTexture, pInputBox->position.x-5, pInputBox->position.y-5);
		}
		renderTexture(pInputBox->texture, pInputBox->position.x, pInputBox->position.y);
	}
}
void inputBoxToggleVisibility(struct inputBox* pInputBox){
	pInputBox->isVisible = !(pInputBox->isVisible);
}
void inputBoxClearTextField(struct inputBox* pInputBox){
	memset(pInputBox->textField,0,sizeof(char)*pInputBox->textFieldSize);
	pInputBox->textField[0] = ' ';/*Nie mozna wyswietlic pustego ciagu znakow*/
	pInputBox->charsWritten = 0;
	pInputBox->updateNeeded = true;
}
