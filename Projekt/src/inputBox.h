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

struct inputBox{
	char* textField;
	size_t textFieldSize;
	int charsWritten; /*ilosc znakow zapisanych w polu tekstowym*/
	bool updateNeeded;
	struct texture* texture;
	struct texture* backgroundTexture;
	bool isVisible;
	SDL_Point position;
	enum fontList font;
	SDL_Color color;
};

struct inputBox* createInputBox(size_t textFieldSize, int x, int y, bool isVisible, enum fontList font,uint8_t r, uint8_t g, uint8_t b, bool addBackground, enum mediaList defaultTexture);
void destroyInputBox(struct inputBox* pInputBox);
void inputBoxAppendChar(struct inputBox* pInputBox, char character);
void inputBoxPopCharacter(struct inputBox* pInputBox);
void inputBoxRender(struct inputBox* pInputBox);
void inputBoxToggleVisibility(struct inputBox* pInputBox);
void inputBoxClearTextField(struct inputBox* pInputBox);
#endif /* INPUTBOX_H_ */
