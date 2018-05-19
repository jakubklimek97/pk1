/*
 * mediaLoader.h
 *
 *  Created on: 30 kwi 2018
 *      Author: klimek
 */

#ifndef MEDIALOADER_H_
#define MEDIALOADER_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include "bool.h"
#include "texture.h"
/*SDL_Surface** pSurfaces;*/
struct texture** pTexture;
SDL_Texture** pTextures;
TTF_Font** pFonts;
bool loadMedia();
/*SDL_Surface* getSurface(int mediaNumber);*/
SDL_Texture* getTexture(int mediaNumber);
TTF_Font* getFont(int fontNumber);
void unloadMedia();

#endif /* MEDIALOADER_H_ */
