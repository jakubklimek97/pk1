/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include "sceneMultiplayer.h"
#include "sceneMultiplayerEngaging.h"
#include "minmax.h"
#include "defines.h"
#include <string.h>
#include <stdlib.h>


extern SDL_Renderer* gRenderer;
static  SDL_Texture* currTex = NULL;
static bool* quit;
static bool wasInitiated = false;
static struct button** pBtns = NULL;
static struct inputBox** pInputBoxes = NULL;

static int btnCount = 9;
static int inputBoxCount = 0;

static bool wasClicked = false;
static enum multis state;
struct texture *text;
static bool hasEnded;
/*button functions*/

static void boardClicked(){
	wasClicked = true;
}
static void checkForEnd(){
	enum whoWon result = lookForWinner(board);
			if(result == 0){
				if(!isEmptyPlace(board)){
					hasEnded = true;
					closeSocket();
					SDL_Color color = {255,0,0,255};
					createFromText(text, FONT_OPENSANS_BOLD,"Draw. Press Enter to exit.",color);
				}
			}

			if(result == PLAYER){
				hasEnded = true;
				closeSocket();
				SDL_Color color = {255,0,0,255};
				createFromText(text, FONT_OPENSANS_BOLD,"You lost. Press Enter to exit.",color);


			}
			if(result == OPPONENT){
				hasEnded = true;
				closeSocket();
				SDL_Color color = {255,0,0,255};
				char textCstr[64] = { '\0' };
				sprintf(textCstr,"You won. Press Enter to exit.");
				createFromText(text, FONT_OPENSANS_BOLD,textCstr,color);

			}
}
static void handleQuad(int processedButton){
	if(board[processedButton/3][processedButton%3] == '_'){
		board[processedButton/3][processedButton%3] = opponent;
		changeButtonTexture(pBtns[processedButton], BUTTON_DEFAULT, (getClientState() == SERVER ? IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_DEFAULT:IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_DEFAULT));
		changeButtonTexture(pBtns[processedButton], BUTTON_MOUSEOVER, (getClientState() == SERVER ? IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_MOUSEOVER:IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_MOUSEOVER));
		sendMessage(processedButton);
		state = state == SERVER? CLIENT : SERVER;
		checkForEnd();
	}
}
/*end of button functions*/

#define IMG_BACKGROUND IMG_SCENE_SINGLEPLAYER_BG_STATIC
#define CURRENT_SCENE MULTIPLAYER



static void init(){
	quit = malloc(sizeof(bool));
	*quit = false;
	text = malloc(sizeof(struct texture));
	text->lTexture = NULL;
	hasEnded = false;
	state = SERVER;

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
	setupBot(getClientState() == SERVER ? O : X);
	wasInitiated = true;
}
static void unInit(){
	wasInitiated = false;
	closeSocket();
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
	destroyTexture(text);
}


static void handleEvents(SDL_Event *e){

	if(!wasInitiated){
		init();
	}
	if(e->type == SDL_QUIT){
		*quit = true;
		return;
	}
	if(hasEnded == false && getClientState() != state){
		if(isActiveSocket()){
			char data;
			if(SDLNet_TCP_Recv(*getSocket(),&data,sizeof(char)) == sizeof(char)){
				switch(data){
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:{
					board[data/3][data%3] = player;
					changeButtonTexture(pBtns[(int)data], BUTTON_DEFAULT, (getClientState() == SERVER ? IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_DEFAULT:IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_DEFAULT));
					changeButtonTexture(pBtns[(int)data], BUTTON_MOUSEOVER, (getClientState() == SERVER ? IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_MOUSEOVER :IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_MOUSEOVER));
					state = state == SERVER? CLIENT : SERVER;
					checkForEnd();
					break;
				}
				case 9: printf("info ze koniec\n");break;
				case 10:
				default: break;
				}
			}
			else{
				hasEnded = true;
				closeSocket();
				SDL_Color color = {255,0,0,255};
				char textCstr[64] = { '\0' };
				sprintf(textCstr,"Other side disconnected. Press Enter to exit.");
				createFromText(text, FONT_OPENSANS_BOLD,textCstr,color);
			}
		}
	}
	if(hasEnded && e->type == SDL_KEYDOWN && (e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_RETURN2)){
		selectScene(MAIN_MENU);
	}
	int processedButton;
	for(processedButton = 0;selectedScene == CURRENT_SCENE && processedButton < btnCount; ++processedButton){
		handeEvent(pBtns[processedButton], e);
		if(wasClicked == true){
			wasClicked = false;
			if(getClientState() == state)handleQuad(processedButton);
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
		if(hasEnded == true){
			renderTexture(text,(SCREEN_WIDTH-text->width)/2,(SCREEN_HEIGHT-text->height)/2);
		}
		SDL_RenderPresent(gRenderer);
}


struct scene multiplayerScene = {&handleEvents, &renderScene, &init, &unInit};

