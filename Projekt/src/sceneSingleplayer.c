/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include "sceneSingleplayer.h"
#include "minmax.h"
#include <string.h>
#include <stdlib.h>

extern SDL_Renderer* gRenderer;
static  SDL_Texture* currTex = NULL;
static bool* quit;
static bool wasInitiated = false;
static struct button** pBtns = NULL;
static struct inputBox** pInputBoxes = NULL;

static int btnCount = 11;
static int inputBoxCount = 0;

static int difficulty = 2;
static bool wasClicked = false;
static int lastPlayerMove = -1;
static int lastBotMove = -1;
/*button functions*/
static void saveBtnClicked(){
	FILE* saveFile = fopen("save.dat","wb");
	if(saveFile != NULL){
		fwrite((void*)board,sizeof(char),3,saveFile);
		fwrite((void*)(board+1),sizeof(char),3,saveFile);
		fwrite((void*)(board+2),sizeof(char),3,saveFile);
		fwrite((void*)&lastPlayerMove,sizeof(int),1,saveFile);
		fwrite((void*)&lastBotMove,sizeof(int),1,saveFile);
		fclose(saveFile);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Save completed","Progress has been saved.",NULL);
	}
}

static void undoBtnClicked(){
	pBtns[10]->pFunction = NULL;
	changeButtonTexture(pBtns[10], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_UNACTIVE);
	changeButtonTexture(pBtns[10], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_UNACTIVE);
	changeButtonTexture(pBtns[lastBotMove], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_DEFAULT);
	changeButtonTexture(pBtns[lastBotMove], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_MOUSEOVER);
	changeButtonTexture(pBtns[lastPlayerMove], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_DEFAULT);
	changeButtonTexture(pBtns[lastPlayerMove], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_MOUSEOVER);
	board[lastBotMove/3][lastBotMove%3] = '_';
	board[lastPlayerMove/3][lastPlayerMove%3] = '_';
	lastPlayerMove = -1;
	lastBotMove = -1;
}
static void loadBtnClicked(){
	FILE* loadedFile = fopen("save.dat","rb");
	if(loadedFile == NULL){
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Loading failed","Save file could not be opened.",NULL);
	}
	else{
		fread((void*)board,sizeof(char),3,loadedFile);
		fread((void*)(board+1),sizeof(char),3,loadedFile);
		fread((void*)(board+2),sizeof(char),3,loadedFile);
		fread((void*)&lastPlayerMove,sizeof(int),1,loadedFile);
		if(fread((void*)&lastBotMove,sizeof(int),1,loadedFile) == 0){
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Loading failed","Error occured during loading.",NULL);
		}
		int index;
		for(index = 0; index < 9; ++index){
			switch(board[index/3][index%3]){
			case '_': break;
			case 'x':{
				changeButtonTexture(pBtns[index], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_DEFAULT);
				changeButtonTexture(pBtns[index], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_MOUSEOVER);
				break;
			}
			case 'o':{
				changeButtonTexture(pBtns[index], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_DEFAULT);
				changeButtonTexture(pBtns[index], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_MOUSEOVER);
				break;
			}
			}

		}
		if(lastPlayerMove != -1){
			pBtns[10]->pFunction = &undoBtnClicked;
			changeButtonTexture(pBtns[10], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_ACTIVE);
			changeButtonTexture(pBtns[10], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_MOUSEOVER);
		}
	}
}
static void boardClicked(){
	wasClicked = true;
}
void handleQuad(int processedButton){
	if(board[processedButton/3][processedButton%3] == '_'){
		board[processedButton/3][processedButton%3] = opponent;
		changeButtonTexture(pBtns[processedButton], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_DEFAULT);
		changeButtonTexture(pBtns[processedButton], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_MOUSEOVER);
		lastPlayerMove = processedButton;
		enum whoWon result = lookForWinner(board);
		if(result == 0){
			if(isEmptyPlace(board)){
				int generated = SDL_GetTicks()%5;
				if(generated > difficulty){
					setupBot(X);
				}
				struct minMax_Move nextMove = findBestMove(board);
				board[nextMove.row][nextMove.col] = generated > difficulty ? opponent:player;
				changeButtonTexture(pBtns[nextMove.row*3+nextMove.col], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_DEFAULT);
				changeButtonTexture(pBtns[nextMove.row*3+nextMove.col], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_MOUSEOVER);
				lastBotMove = nextMove.row*3+nextMove.col;
				pBtns[10]->pFunction = &undoBtnClicked;
				changeButtonTexture(pBtns[10], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_ACTIVE);
				changeButtonTexture(pBtns[10], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_MOUSEOVER);
				if(generated > difficulty){
					setupBot(O);
				}
				result = lookForWinner(board);
			}else{
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Draw","No more tiles :/",NULL);
			}
		}

		if(result == PLAYER){
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Computer won","Think next time plz.",NULL);
		}
		if(result == OPPONENT){

			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Player won","Gratz.",NULL);
		}
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
	pBtns[9] = createButton(489, 119,IMG_SCENE_SINGLEPLAYER_BTN_SAVE_DEFAULT,
			IMG_SCENE_SINGLEPLAYER_BTN_SAVE_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_SAVE_DEFAULT,&saveBtnClicked);
	pBtns[10] = createButton(489,274,IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_UNACTIVE,
			IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_UNACTIVE, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_UNACTIVE, NULL);
	memset(board,'_',sizeof(char)*9);
	setupBot(O);
	difficulty = 4;
	srand(time(NULL));
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

