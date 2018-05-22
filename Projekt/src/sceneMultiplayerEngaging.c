/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include "sceneMultiplayerEngaging.h"
#include <string.h>

extern SDL_Renderer* gRenderer;
static  SDL_Texture* currTex = NULL;
static bool* quit;
static bool wasInitiated = false;
static struct button** pBtns = NULL;
static struct inputBox** pInputBoxes = NULL;

static int btnCount = 1;
static int inputBoxCount = 1;
/*button functions*/
static void clientServerBtnClicked(){
	inputBoxToggleVisibility(pInputBoxes[0]);
}

/*end of button functions*/

#define IMG_BACKGROUND IMG_SCENE_MULTIPLAYER_ENGAGING_BG_STATIC
#define CURRENT_SCENE MULTIPLAYER_ENGAGING

static void init(){
	quit = malloc(sizeof(bool));
	*quit = false;
	if(btnCount){
		pBtns = malloc(btnCount*sizeof(struct button*));
	}
	if(inputBoxCount){
		pInputBoxes = malloc(inputBoxCount*sizeof(struct inputBox*));
	}
	/*place buttons and input fields here */
	pBtns[0] = createButton(250,125,IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CLIENT_DEFAULT_MOUSEOVER,
			IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CLIENT_DEFAULT_MOUSEOVER,
			IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CLIENT_DEFAULT_MOUSEOVER,&clientServerBtnClicked);
	pInputBoxes[0] = createInputBox(15,292,290,false,FONT_OPENSANS_BOLD,255,255,255,true,IMG_SCENE_MULTIPLAYER_ENGAGING_INPUT_BG);

	/*end of button and input fields placement*/

	wasInitiated = true;
}
static void unInit(){
	wasInitiated = false;
	free(quit);
	int i;
	for(i=0; i < btnCount; ++i){
		destroyButton(pBtns[i]);
	}
	for(i=0; i < inputBoxCount; ++i){
			destroyInputBox(pInputBoxes[i]);
		}
	free(pBtns);
	free(pInputBoxes);
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
	for(processedButton = 0;selectedScene == CURRENT_SCENE && processedButton < btnCount; ++processedButton){
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
		int processed;
		for(processed = 0; processed < btnCount; ++processed){
			renderButton(pBtns[processed]);
		}
		for(processed = 0; processed < inputBoxCount; ++processed){
					inputBoxRender(pInputBoxes[processed]);
		}
		SDL_RenderPresent(gRenderer);
}


struct scene multiplayerEngagingScene = {&handleEvents, &renderScene, &init, &unInit};

