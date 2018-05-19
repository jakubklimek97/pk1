/*
 * texture.h
 *
 *  Created on: 8 maj 2018
 *      Author: klimek
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <SDL2/SDL.h>
#include "bool.h"

struct texture{
	int width;
	int height;
	SDL_Texture* lTexture;
};
void renderTexture(struct texture* pTexture, int x, int y);
bool createFromText(struct texture* pTexture, int fontName, const char* text, SDL_Color textColor);

#endif /* TEXTURE_H_ */
