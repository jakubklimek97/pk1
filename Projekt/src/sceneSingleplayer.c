/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include "sceneSingleplayer.h"
#include "sceneScoreboard.h"
#include "minmax.h"
#include "defines.h"
#include "sceneGlobals.h"
#include "game.h"
#include <string.h>
#include <stdlib.h>

static int btnCount = 12;
static int inputBoxCount = 0;

static bool wasClicked = false;
static int lastPlayerMove = -1;
static int lastBotMove = -1;
static int score;
struct texture *text;
static bool hasEnded;
static uint32_t ticks;
static bool msg; /*czy wiadomosc oczekuje na wyswietlenie*/

/*Ponizsze trzy funkcje wlaczaja/wylaczaja przyciski*/
static void enableUndoButton();
static void disableUndoButton();
static void disableLoadButton();
/*Funkcja przygotowywuje wiadomosc do wyswietlenia. Parametry:
 * text - tekstura, na ktorej wiadomosc ma byc wypisana
 * msg - ciag znakow reprezentujacy wiadomosc
 * */
static void printMessage(struct texture *text,const char* msg){
	SDL_Color color = {255,0,0,255};
	createFromText(text, FONT_OPENSANS_BOLD,msg,color);
	ticks = SDL_GetTicks();
}
/*Funkcje przyciskow*/
static void saveBtnClicked(){
	/*odczyt pliku do zmiennej*/
	FILE* saveFile = fopen("save.dat","wb");
	if(saveFile != NULL){
		fwrite((void*)board,sizeof(char),3,saveFile);
		fwrite((void*)(board+1),sizeof(char),3,saveFile);
		fwrite((void*)(board+2),sizeof(char),3,saveFile);
		fwrite((void*)&lastPlayerMove,sizeof(int),1,saveFile);
		fwrite((void*)&lastBotMove,sizeof(int),1,saveFile);
		fwrite((void*)&score,sizeof(int),1,saveFile);
		fclose(saveFile);
		msg = true; /*Pojawi sie wiadomosc*/
		printMessage(text,"Progress has been saved.");
	}
}

static void undoBtnClicked(){
	/*ustaw tekstury szachownicy na takie jak w poprzednim ruchu*/
	changeButtonTexture(pBtns[lastBotMove], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_DEFAULT);
	changeButtonTexture(pBtns[lastBotMove], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_MOUSEOVER);
	changeButtonTexture(pBtns[lastPlayerMove], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_DEFAULT);
	changeButtonTexture(pBtns[lastPlayerMove], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_MOUSEOVER);
	/*cofnij zmiany na planszy*/
	board[lastBotMove/3][lastBotMove%3] = '_';
	board[lastPlayerMove/3][lastPlayerMove%3] = '_';
	/*dalsze cofanie niedozwolone*/
	lastPlayerMove = -1;
	lastBotMove = -1;
	score -= 40/difficulty(0);/*odejmij punkty za cofniecie*/
	disableUndoButton();/*wylaczenie przycisku do cofania*/
}
static void refreshBoardTextures(){
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
}
static void loadBtnClicked(){
	FILE* loadedFile = fopen("save.dat","rb");
	if(loadedFile == NULL){
		/*jezeli plik nie istnieje, wypisz odpowiednia wiadomosc*/
		msg = true;
		printMessage(text,"Save file doesn't exist.");
	}
	else{
		/*odczyt pliku*/
		fread((void*)board,sizeof(char),3,loadedFile);
		fread((void*)(board+1),sizeof(char),3,loadedFile);
		fread((void*)(board+2),sizeof(char),3,loadedFile);
		fread((void*)&lastPlayerMove,sizeof(int),1,loadedFile);
		fread((void*)&lastBotMove,sizeof(int),1,loadedFile);
		if(fread((void*)&score,sizeof(int),1,loadedFile) == 0){
			/*jezeli nie udalo sie wszystkiego odczytac wypisz odpowiednia wiadomosc*/
			msg = true;
			printMessage(text,"Error occured during loading.");
		}
		/*ustaw tekstury planszy na odpowiadajace ustawieniu planszy*/
		refreshBoardTextures();
		/*jezeli zapis nie zawiera poprzedniego ruchu, zablokuj przycisk do cofania*/
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
static void disableSaveButton(){
	pBtns[9]->pFunction = NULL;
}
static void disableLoadButton(){
	pBtns[11]->pFunction = NULL;
}
static void disableUndoButton(){
	pBtns[10]->pFunction = NULL;
	changeButtonTexture(pBtns[10], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_UNACTIVE);
	changeButtonTexture(pBtns[10], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_UNACTIVE);
}
static void enableUndoButton(){
	pBtns[10]->pFunction = &undoBtnClicked;
	changeButtonTexture(pBtns[10], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_ACTIVE);
	changeButtonTexture(pBtns[10], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_MOUSEOVER);
}
static void disableAllFunctionButtons(){
	disableUndoButton();
	disableSaveButton();
	disableLoadButton();
}

static void makeAIMove(){
	/*sprawdz, czy w tej chwili bot "ma ochote" wykonac ruch bedacy w interesie gracza*/
	int generated = SDL_GetTicks()%5;
	if(generated > difficulty(0)){
		setupBot(X); /*"Bot udaje gracza */
	}
	struct minMax_Move nextMove = findBestMove(board); /*znajdz najlepszy dostepny ruch i wykonaj go (analogicznie do gracza)*/
	board[nextMove.row][nextMove.col] = generated > difficulty(0) ? opponent:player;
	changeButtonTexture(pBtns[nextMove.row*3+nextMove.col], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_DEFAULT);
	changeButtonTexture(pBtns[nextMove.row*3+nextMove.col], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_MOUSEOVER);
	lastBotMove = nextMove.row*3+nextMove.col; /*ostatni ruch bota*/
	if(generated > difficulty(0)){
		setupBot(O); /*"Bot znowu jest botem */
	}
}
static void handleQuad(int processedButton){/*obsluga wcisnietego przycisku*/
	if(board[processedButton/3][processedButton%3] == '_'){ /*jezeli pole jest puste*/
		board[processedButton/3][processedButton%3] = opponent;/*wstaw tam znak gracza, i zmien tekstury pola*/
		changeButtonTexture(pBtns[processedButton], BUTTON_DEFAULT, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_DEFAULT);
		changeButtonTexture(pBtns[processedButton], BUTTON_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_MOUSEOVER);
		lastPlayerMove = processedButton;/*ustawienie tego pola jako ostatni ruch gracza*/
		enum whoWon result = lookForWinner(board);/*sprawdz, czy gracz dzieki temu wygral*/
		if(result == 0){/*jezeli nie ma zwyciezcy*/
			if(isEmptyPlace(board)){/*i istnieje wolne miejsce*/
				makeAIMove();
				enableUndoButton();/*skoro mamy ostatnie ruchy, mozemy je cofnac*/
				result = lookForWinner(board);/*sprawdz, czy dalej nie ma zwyciezcy*/
			}else{
				/*jezeli nie ma miejsca, jest remis. Pzygotuj wiadomosc do wypisania i ustaw flage hasEnded na true*/
				hasEnded = true;
				printMessage(text,"Draw. Press Enter to exit.");
				addScore(0);
				disableAllFunctionButtons();/*koniec gry, nie ma mowy o cofaniu, zapisywaniu badz wczytywaniu*/

			}
		}

		if(result == PLAYER){/*jezeli wygral bot*/
			hasEnded = true;
			printMessage(text,"You lost. Press Enter to exit.");
			addScore(0);
			disableAllFunctionButtons();
		}
		if(result == OPPONENT){/*jezeli wygral gracz*/
			hasEnded = true;
			char textCstr[64] = { '\0' };
			sprintf(textCstr,"You won. Your score: %d. Press Enter to exit.",score);
			printMessage(text,textCstr);
			addScore(score);/*dodaj wynik do tablicy wynikow*/
			disableAllFunctionButtons();
		}
	}
}
/*Funkcje przyciskow*/

#define IMG_BACKGROUND IMG_SCENE_SINGLEPLAYER_BG_STATIC
#define CURRENT_SCENE SINGLEPLAYER



static void init(){/*omowione w sceneMenu.c*/
	quit = malloc(sizeof(bool));
	*quit = false;
	text = malloc(sizeof(struct texture));
	text->lTexture = NULL;
	hasEnded = false;/*gra sie nie skonczyla*/

	if(btnCount){
		pBtns = malloc(btnCount*sizeof(struct button*));
	}
	if(inputBoxCount){
		pInputBoxes = malloc(inputBoxCount*sizeof(struct inputBox*));
	}
	/* pierwsze 9 przyciskow definiuje plansze do gry */
	int index;
	for(index = 0; index < 9; ++index){
		pBtns[index] = createButton(25+150*(index%3),120+150*(index/3),IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_DEFAULT,
			IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_MOUSEOVER,IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_DEFAULT,&boardClicked);
	}
	pBtns[9] = createButton(489, 119,IMG_SCENE_SINGLEPLAYER_BTN_SAVE_DEFAULT,
			IMG_SCENE_SINGLEPLAYER_BTN_SAVE_MOUSEOVER, IMG_SCENE_SINGLEPLAYER_BTN_SAVE_DEFAULT,&saveBtnClicked);
	pBtns[10] = createButton(489,274,IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_UNACTIVE,
			IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_UNACTIVE, IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_UNACTIVE, NULL);
	pBtns[11] = createButton(489,429, IMG_SCENE_SINGLEPLAYER_BTN_LOAD_DEFAULT,
			IMG_SCENE_SINGLEPLAYER_BTN_LOAD_MOUSEOVER,IMG_SCENE_SINGLEPLAYER_BTN_LOAD_DEFAULT,&loadBtnClicked);
	memset(board,'_',sizeof(char)*9);/*wyzeruj plansze*/
	setupBot(O); /*bot gra kolkiem*/
	score = 1000;/*poczatkowy wynik*/
	lastBotMove = lastPlayerMove = -1; /*Nie bylo wczesniej ruchu*/
	msg = false;/*nie ma wiadomosci do wypisania*/
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
	destroyTexture(text);/*usun teksture, ktora przechowywala wiadomosc*/
}


static void handleEvents(SDL_Event *e){/*Funkcja omowiona w sceneMenu.c*/

	if(!wasInitiated){
		init();
	}
	if(e->type == SDL_QUIT){
		*quit = true;
		return;
	}
	if(hasEnded && e->type == SDL_KEYDOWN && (e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_RETURN2)){
		selectScene(SCOREBOARD);/*jezeli gra sie zakonczyla a uzytkownik potwierdzil komunikat na ekranie, przejdz do tablicy wynikow*/
	}
	int processedButton;
	for(processedButton = 0;selectedScene == CURRENT_SCENE && processedButton < btnCount; ++processedButton){
		handeEvent(pBtns[processedButton], e);
		if(wasClicked == true){/*jezeli aktualnie przetwarzany przycisk zostal klikniety, wykonaj odpowiednia akcje*/
			wasClicked = false;
			handleQuad(processedButton);
		}
	}
}
static void renderScene()/*Funkcja omowiona  w sceneMenu.c*/
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
		for(processed = 0; processed < btnCount; ++processed){
			renderButton(pBtns[processed]);
		}
		for(processed = 0; processed < inputBoxCount; ++processed){
					inputBoxRender(pInputBoxes[processed]);
		}
		if(hasEnded == true){
			renderTexture(text,(SCREEN_WIDTH-text->width)/2,(SCREEN_HEIGHT-text->height)/2);
		}
		else if(msg){/*jezeli gra dalej trwa i istnieje wiadomosc do pokazania*/
			renderTexture(text,(SCREEN_WIDTH-text->width)/2,(SCREEN_HEIGHT-text->height)/2);
			if(SDL_GetTicks() - ticks > 2000){
				msg = false;/*jezeli wiadomosc istnieje ponad 2 sekundy, nie pokazuj jej wiecej*/
			}
		}
		SDL_RenderPresent(getRenderer());
}


struct scene singleplayerScene = {&handleEvents, &renderScene, &init, &unInit};

