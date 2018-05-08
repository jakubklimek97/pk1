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
enum buttonState {
	BUTTON_DEFAULT,
	BUTTON_MOUSEOVER,
	BUTTON_PRESSED
};
struct button {
	SDL_Point position;
	enum buttonState state;
	struct texture* bTexture[3];
};
struct button* createButton(int x, int y, enum mediaList defaultTexture,  enum mediaList mouseoverTexture, enum mediaList pressedTexture);
void renderButton(struct button* pButton);
void handeEvent(SDL_Event* e);


#endif /* BUTTON_H_ */
