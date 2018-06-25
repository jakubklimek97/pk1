/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include "sceneScoreboard.h"
#include <string.h>
#include "stdlib.h"
#include "defines.h"

extern SDL_Renderer* gRenderer;
static  SDL_Texture* currTex = NULL;
static bool* quit;
static bool wasInitiated = false;
static struct button** pBtns = NULL;
static struct inputBox** pInputBoxes = NULL;
static struct texture** textBoxes = NULL;
static int pendingScore = 0;
static bool newScore;
static int* scores = NULL;
static char** names = NULL;

static int btnCount = 1;
static int inputBoxCount = 1;

/*button functions*/

/*end of button functions*/

#define IMG_BACKGROUND IMG_SCENE_SCOREBOARD_BG_STATIC
#define CURRENT_SCENE SCOREBOARD

void deleteScores(){
	if(scores != NULL){
		free(scores);
		free(*names);
		free(*(names+1));
		free(*(names+2));
		free(names);
		scores = NULL;
	}
}
void loadScores(){
	FILE* scoreFile = fopen("scores.dat","rb");
	if(scoreFile == NULL){
		deleteScores();
		scores = malloc(sizeof(int)*3);
		memset(scores,0,sizeof(int)*3);
		names = malloc(sizeof(char*)*3);
		names[0] = malloc(sizeof(char)*33);
		names[1] = malloc(sizeof(char)*33);
		names[2] = malloc(sizeof(char)*33);
		memset(names[0],'\0',sizeof(char)*33);
		memset(names[1],'\0',sizeof(char)*33);
		memset(names[2],'\0',sizeof(char)*33);
	}
	else{
		deleteScores();
		scores = malloc(sizeof(int)*3);
		memset(scores,0,sizeof(int)*3);
		names = malloc(sizeof(char*)*3);
		names[0] = malloc(sizeof(char)*33);
		names[1] = malloc(sizeof(char)*33);
		names[2] = malloc(sizeof(char)*33);
		fread((void*)names[0],sizeof(char),33,scoreFile);
		fread((void*)names[1],sizeof(char),33,scoreFile);
		fread((void*)names[2],sizeof(char),33,scoreFile);
		fread((void*)scores, sizeof(int),3,scoreFile);
		fclose(scoreFile);

	}

}
int tryScore(int score){
	int pos = 0;
	for(pos = 2; pos >= 0 && score > scores[pos];--pos);
	return (pos == 2) ? -1 : ++pos;
}
int placeScore(int score){
	int pos = 0;
	for(pos = 2; pos >= 0 && score > scores[pos];--pos);
	switch(pos){
	case -1:{
		scores[2] = scores[1];
		memcpy(names[2], names[1],sizeof(char)*33);
		scores[1] = scores[0];
		memcpy(names[1], names[0],sizeof(char)*33);
		break;
	}
	case 0:{
		scores[2] = scores[1];
		memcpy(names[2], names[1],sizeof(char)*33);
		break;
	}
	case 2: return -1;
	}
	scores[++pos] = score;
	return pos; /*position where score was placed */
}
void saveScores(){
	if(scores == NULL){
		loadScores();
	}
	FILE* scoreFile = fopen("scores.dat","wb");
	if(scoreFile != NULL){
		fwrite((void*)names[0],sizeof(char),33,scoreFile);
		fwrite((void*)names[1],sizeof(char),33,scoreFile);
		fwrite((void*)names[2],sizeof(char),33,scoreFile);
		fwrite((void*)scores, sizeof(int),3,scoreFile);
		fclose(scoreFile);
	}
	deleteScores();
}
void addScore(int score){
	pendingScore = score;
}
void prepareScoresTexture(){
	char scoretex[40] = {'\0'};
	sprintf(scoretex,"%d - Your score",pendingScore);
	SDL_Color kolor = {255,255,255,255};
	if(textBoxes == NULL){
		textBoxes = malloc(sizeof(struct texture*)*5);
		textBoxes[0] = malloc(sizeof(struct texture));
		textBoxes[1] = malloc(sizeof(struct texture));
		textBoxes[2] = malloc(sizeof(struct texture));
		textBoxes[3] = malloc(sizeof(struct texture));
		textBoxes[4] = malloc(sizeof(struct texture));
		textBoxes[0]->lTexture = NULL;
		textBoxes[1]->lTexture = NULL;
		textBoxes[2]->lTexture = NULL;
		textBoxes[3]->lTexture = NULL;
		textBoxes[4]->lTexture = NULL;
	}

	createFromText(textBoxes[0],FONT_OPENSANS_BOLD,scoretex,kolor);
	memset(scoretex,'\0',40);
	sprintf(scoretex,"%d - %s",scores[0],names[0]);
	createFromText(textBoxes[1],FONT_OPENSANS_BOLD,scoretex,kolor);
	memset(scoretex,'\0',40);
	sprintf(scoretex,"%d - %s",scores[1],names[1]);
	createFromText(textBoxes[2],FONT_OPENSANS_BOLD,scoretex,kolor);
	memset(scoretex,'\0',40);
	sprintf(scoretex,"%d - %s",scores[2],names[2]);
	createFromText(textBoxes[3],FONT_OPENSANS_BOLD,scoretex,kolor);
	memset(scoretex,'\0',40);
	sprintf(scoretex,"Enter your name and press ENTER");
	createFromText(textBoxes[4],FONT_OPENSANS_BOLD,scoretex,kolor);
}
static void init(){
	quit = malloc(sizeof(bool));
	*quit = false;
	loadScores();
	prepareScoresTexture();
	if(btnCount){
		pBtns = malloc(btnCount*sizeof(struct button*));
	}
	pBtns[0] = createButton(150,175,IMG_SCENE_SCOREBOARD_INPUTBOX_BG_STATIC,IMG_SCENE_SCOREBOARD_INPUTBOX_BG_STATIC,IMG_SCENE_SCOREBOARD_INPUTBOX_BG_STATIC,NULL);
	if(inputBoxCount){
		pInputBoxes = malloc(inputBoxCount*sizeof(struct inputBox*));
	}
	newScore = (pendingScore == 0 || tryScore(pendingScore) == -1) ? false : true;
	if(newScore == true){
		SDL_StartTextInput();
	}
	pInputBoxes[0] = createInputBox(15,170,220,true,FONT_OPENSANS_BOLD,255,255,255,false,0);

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
	destroyTexture(textBoxes[0]);
	destroyTexture(textBoxes[1]);
	destroyTexture(textBoxes[2]);
	destroyTexture(textBoxes[3]);
	destroyTexture(textBoxes[4]);
	free(textBoxes);
	textBoxes = NULL;
	saveScores();
}


static void handleEvents(SDL_Event *e){

	if(!wasInitiated){
		init();
	}
	if(e->type == SDL_QUIT){
		*quit = true;
		return;
	}

	if(newScore){
		if(e->type == SDL_KEYDOWN){
			if(e->key.keysym.sym == SDLK_BACKSPACE){
				inputBoxPopCharacter(pInputBoxes[0]);
			}
			else if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym ==SDLK_RETURN2 ) {
				SDL_StopTextInput();
				int pos = placeScore(pendingScore);
				if(pos >= 0){
					memset(names[pos],'\0',sizeof(char)*33);
					memcpy(names[pos],pInputBoxes[0]->textField,sizeof(char)*pInputBoxes[0]->textFieldSize);
					prepareScoresTexture();
				}
				newScore = false;
			}

		}
		else if(e->type == SDL_TEXTINPUT){
			inputBoxAppendChar(pInputBoxes[0],e->text.text[0]);
		}
	}
	else{
		if(e->type == SDL_KEYDOWN){
			 if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym ==SDLK_RETURN2 ){
				 selectScene(MAIN_MENU);
			 }
		}
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
		for(processed = 0; processed < 4; ++processed){
			renderTexture(textBoxes[processed],240,220+processed*60);
		}
		if(newScore){
			for(processed = 0; processed < btnCount; ++processed){
				renderButton(pBtns[processed]);
			}
			renderTexture(textBoxes[4],(SCREEN_WIDTH-textBoxes[4]->width)/2,180);
			for(processed = 0; processed < inputBoxCount; ++processed){
				inputBoxRender(pInputBoxes[processed]);
			}

		}
		SDL_RenderPresent(gRenderer);
}


struct scene scoreboardScene = {&handleEvents, &renderScene, &init, &unInit};

