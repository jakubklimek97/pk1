/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include "sceneScoreboard.h"
#include "stdlib.h"
#include "defines.h"
#include "sceneGlobals.h"
#include "game.h"
#include <string.h>

static int btnCount = 1;
static int inputBoxCount = 1;

static struct texture** textBoxes = NULL;
static int pendingScore = 0;
static bool newScore;
static int* scores = NULL;
static char** names = NULL;
/*button functions*/

/*end of button functions*/

#define IMG_BACKGROUND IMG_SCENE_SCOREBOARD_BG_STATIC
#define CURRENT_SCENE SCOREBOARD
/*Funkcja usuwajaca wszystkie wyniki z pamieci*/
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
/*Funkcja ladujaca do pamieci wyniki z pliku*/
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
		deleteScores(); /*najpierw usun obecnie przechowywane wyniki*/
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
/*Funkcja sprawdza, czy wynik trafilby na tablice wynikow. Parametry:
 * score - sprawdzany wynik
 * Zwraca -1 w przypadku gdy nie trafi, w przeciwnym wypadku pozycje
 */
int tryScore(int score){
	int pos = 0;
	for(pos = 2; pos >= 0 && score > scores[pos];--pos);
	return (pos == 2) ? -1 : ++pos;
}

/*Funkcja podobna do tryScore(...). Ta funkcja umieszcza wynik w tabeli wynikow. Parametry:
 * score - sprawdzany wynik
 * Zwraca -1 w przypadku gdy nie trafi, w przeciwnym wypadku pozycje*/
int placeScore(int score){
	int pos = 0;
	for(pos = 2; pos >= 0 && score > scores[pos];--pos);
	switch(pos){
	case -1:{
		/*przesuwanie wynikow w tabeli*/
		scores[2] = scores[1];
		memcpy(names[2], names[1],sizeof(char)*33);
		scores[1] = scores[0];
		memcpy(names[1], names[0],sizeof(char)*33);
		break;
	}
	case 0:{
		/*przesuwanie wynikow w tabeli*/
		scores[2] = scores[1];
		memcpy(names[2], names[1],sizeof(char)*33);
		break;
	}
	case 2: return -1; /*wynik nie trafil do tabeli*/
	}
	scores[++pos] = score;
	return pos; /*zajeta pozycja */
}
/*Funkcja zapisuje wyniki do pliku i usuwa je z pamieci*/
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
/*Funkcja przygotowuje tekstury zawierajace tablice wynikow*/
void prepareScoresTexture(){
	char scoretex[40] = {'\0'};/*tymczasowy lancuch znakow*/
	sprintf(scoretex,"%d - Your score",pendingScore);
	SDL_Color kolor = {255,255,255,255};
	if(textBoxes == NULL){
		/*jezeli nie zostalo to zrobione wczesniej, alokacja tekstur*/
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
	/*Utworzenie tekstur - wynik gracza + 3 najlepsze wyniki */
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
static void init(){ /*Omowione w pliku sceneMenu.c*/
	quit = malloc(sizeof(bool));
	*quit = false;
	loadScores(); /*zaladuj wyniki*/
	prepareScoresTexture();/*przygotuj tekstury*/
	if(btnCount){
		pBtns = malloc(btnCount*sizeof(struct button*));
	}
	pBtns[0] = createButton(150,175,IMG_SCENE_SCOREBOARD_INPUTBOX_BG_STATIC,IMG_SCENE_SCOREBOARD_INPUTBOX_BG_STATIC,IMG_SCENE_SCOREBOARD_INPUTBOX_BG_STATIC,NULL);
	if(inputBoxCount){
		pInputBoxes = malloc(inputBoxCount*sizeof(struct inputBox*));
	}
	newScore = (pendingScore == 0 || tryScore(pendingScore) == -1) ? false : true; /*czy potrzeba okienko do wpisywania nazwy uzytkownika */
	if(newScore == true){ /*jezeli potrzeba okienka*/
		SDL_StartTextInput();
	}
	pInputBoxes[0] = createInputBox(15,170,220,true,FONT_OPENSANS_BOLD,255,255,255,false,0);

	wasInitiated = true;
}
static void unInit(){/*omowione w sceneMenu.c*/
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
	/*zwolnij tekstury zawierajace wyniki */
	destroyTexture(textBoxes[0]);
	destroyTexture(textBoxes[1]);
	destroyTexture(textBoxes[2]);
	destroyTexture(textBoxes[3]);
	destroyTexture(textBoxes[4]);
	free(textBoxes);
	textBoxes = NULL;
	saveScores();/*zapisz wyniki do pliku*/
}


static void handleEvents(SDL_Event *e){

	if(!wasInitiated){
		init();
	}
	if(e->type == SDL_QUIT){
		*quit = true;
		return;
	}

	if(newScore){/*jezeli uzytkownik ma wpisac swoja nazwe*/
		if(e->type == SDL_KEYDOWN){
			if(e->key.keysym.sym == SDLK_BACKSPACE){
				inputBoxPopCharacter(pInputBoxes[0]);/*jezeli klika backspace, usuwamy znak z pola tekstowego*/
			}
			else if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym ==SDLK_RETURN2 ) {/*jezeli uzytkownik zaakceptowal swoja nazwe*/
				SDL_StopTextInput();
				int pos = placeScore(pendingScore);/*umiesc wynik w tablicy */
				if(pos >= 0){
					/*umiesc nazwe uzytkownika w polu odpowiadajacym wynikowi*/
					memset(names[pos],'\0',sizeof(char)*33);
					memcpy(names[pos],pInputBoxes[0]->textField,sizeof(char)*pInputBoxes[0]->textFieldSize);
					prepareScoresTexture();/*odswiez tekstury z wynikami*/
				}
				newScore = false;/*juz nie potrzeba okienka do wpisywania*/
			}

		}
		else if(e->type == SDL_TEXTINPUT){
			/*jezeli uzytkownik wpisuje, dodaj wpisany znak do pola tekstowego*/
			inputBoxAppendChar(pInputBoxes[0],e->text.text[0]);
		}
	}
	else{
		if(e->type == SDL_KEYDOWN){
			 if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym ==SDLK_RETURN2 ){
				 /*jezeli uzytkownik zdecydowal sie opuscic tablice wynikow, wroc do glownego menu*/
				 selectScene(MAIN_MENU);
			 }
		}
	}
	int processedButton;
	for(processedButton = 0;selectedScene == CURRENT_SCENE && processedButton < btnCount; ++processedButton){
		handeEvent(pBtns[processedButton], e);
	}
}
static void renderScene()/*omowione w sceneMenu.c*/
{
	if(*quit == true){
		selectScene(QUIT);
		return;
	}
	if(currTex == NULL){
		currTex = getTexture(IMG_BACKGROUND);
	}
		SDL_RenderClear(getRenderer());
		SDL_RenderCopy(getRenderer(),currTex,NULL,NULL);
		int processed;
		for(processed = 0; processed < 4; ++processed){
			renderTexture(textBoxes[processed],240,220+processed*60);
		}
		if(newScore){/*jezeli trzeba wyswietlic okienko do wpisywania nazwy*/
			for(processed = 0; processed < btnCount; ++processed){
				renderButton(pBtns[processed]);/*"okienko"*/
			}
			renderTexture(textBoxes[4],(SCREEN_WIDTH-textBoxes[4]->width)/2,180);/*komunikat do uzytkownika*/
			for(processed = 0; processed < inputBoxCount; ++processed){
				inputBoxRender(pInputBoxes[processed]);
			}

		}
		SDL_RenderPresent(getRenderer());
}


struct scene scoreboardScene = {&handleEvents, &renderScene, &init, &unInit};

