/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include <SDL2/SDL.h>
#include <string.h>
#include "scene.h"
#include "sceneList.h"
#include "bool.h"
#include "mediaLoader.h"
#include "mediaList.h"
#include "button.h"


extern SDL_Renderer* gRenderer;
static  SDL_Texture* currTex = NULL;
static bool quit = false;
static bool wasInitiated = false;
static struct button** pBtns = NULL;

static int btnCount = 2;
/*button functions*/
static void quitBtnClicked(){
	quit = true;
}
/*end of button functions*/

static void init(){
	pBtns = malloc(btnCount*sizeof(struct button*));
	pBtns[0] = createButton(300,100,IMG_MENU_BTN_DEFAULT, IMG_MENU_BTN_MOUSEOVER,IMG_MENU_BTN_MOUSEOVER, NULL);
	pBtns[1] = createButton(300,300,IMG_MENU_BTN_DEFAULT, IMG_MENU_BTN_MOUSEOVER,IMG_MENU_BTN_MOUSEOVER, &quitBtnClicked);
}



static void handleEvents(SDL_Event *e){

	if(!wasInitiated){
		init();
	}
	if(e->type == SDL_QUIT){
		quit = true;
		return;
	}
	/*if(e->type == SDL_KEYDOWN){
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
					break;
				default:
					currTex = getTexture(IMG_HELLOWORD);
					break;
		}*/
	int processedButton;
	for(processedButton = 0; processedButton < btnCount; ++processedButton){
		handeEvent(pBtns[processedButton], e);
	}
}
static void renderScene()
{
	if(quit == true){
		selectScene(QUIT);
	}
	if(currTex == NULL){
		currTex = getTexture(IMG_MENU_BG);
	}
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer,currTex,NULL,NULL);
		int processedButton;
		for(processedButton = 0; processedButton < btnCount; ++processedButton){
			renderButton(pBtns[processedButton]);
		}
		SDL_RenderPresent(gRenderer);
}


struct scene menuScene = {&handleEvents, &renderScene};

