/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include <SDL2/SDL.h>
#include "scene.h"
#include "sceneList.h"
#include "bool.h"
#include "mediaLoader.h"
#include "mediaList.h"
#include <string.h>

extern SDL_Renderer* gRenderer;
SDL_Texture* currTex = NULL;
bool quit = false;
bool changeScene = false;

static void handleEvents(SDL_Event *e){
	if(e->type == SDL_QUIT){
		quit = true;
	}
	if(e->type == SDL_KEYDOWN){
	switch(e->key.keysym.sym){
				case SDLK_UP:
					currTex = getTexture(IMG_UP);
					break;
				case SDLK_DOWN:
					currTex = getTexture(IMG_DOWN);
					break;
				case SDLK_LEFT:
					currTex = getTexture(IMG_LEFT);
					break;
				case SDLK_RIGHT:
					currTex = getTexture(IMG_RIGHT);
					selectScene(MAIN_MENU);
					break;
				default:
					currTex = getTexture(IMG_HELLOWORD);
					break;
		}
}
}
static void renderScene()
{
	if(quit == true){
		selectScene(QUIT);
	}
	if(currTex == NULL){
		currTex = getTexture(IMG_HELLOWORD);
	}
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer,currTex,NULL,NULL);
		SDL_RenderPresent(gRenderer);
}


struct scene testScene = {&handleEvents, &renderScene, NULL, NULL};

