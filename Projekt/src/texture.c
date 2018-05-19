/*
 * texture.c
 *
 *  Created on: 8 maj 2018
 *      Author: klimek
 */


#include "texture.h"
#include "mediaLoader.h"
#include <string.h>
#include <SDL2/SDL_ttf.h>
extern SDL_Renderer* gRenderer;
void renderTexture(struct texture* pTexture, int x, int y){
	SDL_Rect renderSquare = {x, y, pTexture->width, pTexture->height};
	SDL_RenderCopy(gRenderer, pTexture->lTexture, NULL, &renderSquare);
}
bool createFromText(struct texture* pTexture, int fontName, const char* text, SDL_Color textColor){
	if(pTexture->lTexture != NULL){
		free(pTexture->lTexture);
		pTexture->lTexture = NULL;
	}
	SDL_Surface* textSurface = TTF_RenderText_Solid(getFont(fontName),text,textColor);
	if( textSurface == NULL )
	{
	    printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else{
		pTexture->lTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if(pTexture->lTexture == NULL){
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else{
			pTexture->width = textSurface->w;
			pTexture->height = textSurface->h;
		}
		SDL_FreeSurface(textSurface);

	}
	return (pTexture->lTexture != NULL ? true : false);
}
