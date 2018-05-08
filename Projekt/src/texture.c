/*
 * texture.c
 *
 *  Created on: 8 maj 2018
 *      Author: klimek
 */


#include "texture.h"
extern SDL_Renderer* gRenderer;
void renderTexture(struct texture* pTexture, int x, int y){
	SDL_Rect renderSquare = {x, y, pTexture->width, pTexture->height};
	SDL_RenderCopy(gRenderer, pTexture->lTexture, NULL, &renderSquare);
}
