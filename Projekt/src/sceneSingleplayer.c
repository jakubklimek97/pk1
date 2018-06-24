/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include "sceneSingleplayer.h"
#include "minmax.h"
#include <string.h>


extern SDL_Renderer* gRenderer;
static  SDL_Texture* currTex = NULL;
static bool* quit;
static bool wasInitiated = false;
static struct button** pBtns = NULL;
static struct inputBox** pInputBoxes = NULL;

static int btnCount = 9;
static int inputBoxCount = 0;
static bool wasClicked = false;
/*button functions*/
static void quitBtnClicked(){
	selectScene(QUIT);
}
static void singleBtnClicked(){
	changeButtonTexture(pBtns[0], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_SAVE_DEFAULT);
	changeButtonTexture(pBtns[0], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_SAVE_MOUSEOVER);
}
static void multiBtnClicked(){
	selectScene(MULTIPLAYER_ENGAGING);
}
static void boardClicked(){
	wasClicked = true;
}
void handleQuad(int processedButton){
	if(board[processedButton/3][processedButton%3] == '_'){
		board[processedButton/3][processedButton%3] = player;
		changeButtonTexture(pBtns[processedButton], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_DEFAULT);
		changeButtonTexture(pBtns[processedButton], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_MOUSEOVER);
	}
}
/*end of button functions*/

#define IMG_BACKGROUND IMG_SCENE_SINGLEPLAYER_BG_STATIC
#define CURRENT_SCENE SINGLEPLAYER



static void init(){
	quit = malloc(sizeof(bool));
	*quit = false;
	if(btnCount){
		pBtns = malloc(btnCount*sizeof(struct button*));
	}
	if(inputBoxCount){
		pInputBoxes = malloc(inputBoxCount*sizeof(struct inputBox*));
	}
	/* game grid */
	int index;
	for(index = 0; index < 9; ++index){
		pBtns[index] = createButton(25+150*(index%3),120+150*(index/3),IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_DEFAULT,
			IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_MOUSEOVER,IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_DEFAULT,&boardClicked);
	}
	memset(board,'_',sizeof(char)*9);
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
		if(wasClicked == true){
			wasClicked = false;
			handleQuad(processedButton);
		}
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


struct scene singleplayerScene = {&handleEvents, &renderScene, &init, &unInit};

