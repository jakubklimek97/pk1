/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include "sceneMenu.h"
#include <string.h>

extern SDL_Renderer* gRenderer;
static  SDL_Texture* currTex = NULL;
static bool* quit;
static bool wasInitiated = false;
static struct button** pBtns = NULL;

static int btnCount = 3;
/*button functions*/
static void quitBtnClicked(){
	selectScene(QUIT);
}
static void singleBtnClicked(){

}
static void multiBtnClicked(){

}
/*end of button functions*/

#define IMG_BACKGROUND IMG_SCENE_MENU_BG_STATIC

static void init(){
	quit = malloc(sizeof(bool));
	*quit = false;
	pBtns = malloc(btnCount*sizeof(struct button*));
	pBtns[0] = createButton(250,125,IMG_SCENE_MENU_BTN_SINGLEGAME_DEFAULT, IMG_SCENE_MENU_BTN_SINGLEGAME_MOUSEOVER,IMG_SCENE_MENU_BTN_SINGLEGAME_MOUSEOVER, &singleBtnClicked);
	pBtns[1] = createButton(250,280,IMG_SCENE_MENU_BTN_MULTIPLAYER_DEFAULT, IMG_SCENE_MENU_BTN_MULTIPLAYER_MOUSEOVER,IMG_SCENE_MENU_BTN_MULTIPLAYER_MOUSEOVER, &multiBtnClicked);
	pBtns[2] = createButton(250,435,IMG_SCENE_MENU_BTN_QUITGAME_DEFAULT, IMG_SCENE_MENU_BTN_QUITGAME_MOUSEOVER,IMG_SCENE_MENU_BTN_QUITGAME_MOUSEOVER, &quitBtnClicked);
	wasInitiated = true;
}
static void unInit(){
	wasInitiated = false;
	free(quit);
	int i;
	for(i=0; i < btnCount; ++i){
		destroyButton(pBtns[i]);
	}
	free(pBtns);
}


static void handleEvents(SDL_Event *e){

	if(!wasInitiated){
		init();
	}
	if(e->type == SDL_QUIT){
		*quit = true;
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
	for(processedButton = 0;selectedScene != QUIT && processedButton < btnCount; ++processedButton){
		handeEvent(pBtns[processedButton], e);
	}
}
static void renderScene()
{
	if(*quit == true){
		selectScene(QUIT);
		return;
	}
	if(currTex == NULL){
		currTex = getTexture(IMG_BACKGROUND);
	}
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer,currTex,NULL,NULL);
		int processedButton;
		for(processedButton = 0; processedButton < btnCount; ++processedButton){
			renderButton(pBtns[processedButton]);
		}
		SDL_RenderPresent(gRenderer);
}


struct scene menuScene = {&handleEvents, &renderScene, &init, &unInit};

