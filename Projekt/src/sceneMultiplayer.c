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
#include "sceneGlobals.h"
#include "game.h"
#include <string.h>
#include <stdlib.h>

static int btnCount = 9;
static int inputBoxCount = 0;

static bool wasClicked = false; /*czy plansza zostala nacisnieta (w danym momencie)*/
static enum multis state;/*czyj ruch (klient czy serwer)*/
struct texture *text; /*tekstura do wyswietlania komunikatow*/
static bool hasEnded; /*czy konczymy mecz*/

#define IMG_BACKGROUND IMG_SCENE_SINGLEPLAYER_BG_STATIC
#define CURRENT_SCENE MULTIPLAYER

/*funkcje przyciskow*/
static void boardClicked(){
	wasClicked = true;
}
/*koniec funkcji przyciskow*/

/*Funkcja konczy gre i wypisuje wiadomosc na ekranie. Parametry:
 * message - ciag znakow zawierajacy wiadomosc
 */
static void printEndMessage(const char* message){
	hasEnded = true; /*Koniec gry */
	SDL_Color color = {255,0,0,255}; /*Kolor napisu */
	createFromText(text, FONT_OPENSANS_BOLD,message,color); /*Utworzenie tekstury*/
}
/*Funkcja sprawdza, czy nastapil koniec potyczki */
static void checkForEnd(){
	/*poszukaj zwyciezcy*/
	enum whoWon result = lookForWinner(board);
			if(result == 0){ /*jezeli takiego nie ma*/
				if(!isEmptyPlace(board)){ /*jezeli na planszy nie ma wolnego miejsca, nastapil remis*/
					closeSocket(); /*zamknij polaczenie*/
					printEndMessage("Draw. Press Enter to exit."); /*Zakoncz gre i wypisz komunikat*/
				}
			}
			/*jezeli wygrala druga strona*/
			if(result == PLAYER){
				closeSocket();
				printEndMessage("You lost. Press Enter to exit.");
			}
			/*jezeli wygral gracz*/
			if(result == OPPONENT){
				closeSocket();
				printEndMessage("You won. Press Enter to exit.");
			}
}
/*obsluga przycisku po nacisnieciu go*/
static void handleQuad(int processedButton){
	if(board[processedButton/3][processedButton%3] == '_'){ /*jezeli odpowiadajace przyciskowi pole na planszy jest wolne*/
		board[processedButton/3][processedButton%3] = opponent; /*zajmij pole*/
		/*zmien teksture przycisku na teksture gracza (zwykla i po nacisnieciu przycisku)
		 * tekstura wybierana jest automatycznie na podstawie tego, czy jestesmy klientem czy serwerem*/
		changeButtonTexture(pBtns[processedButton], BUTTON_DEFAULT, (getClientState() == SERVER ? IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_DEFAULT:IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_DEFAULT));
		changeButtonTexture(pBtns[processedButton], BUTTON_MOUSEOVER, (getClientState() == SERVER ? IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_MOUSEOVER:IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_MOUSEOVER));
		sendMessage(processedButton); /*wyslij wiadomosc o wykonanym ruchu do drugiej strony*/
		state = state == SERVER? CLIENT : SERVER; /*teraz kolej na drugiego gracza*/
		checkForEnd(); /*sprawdz czy gra zostala zakonczona */
	}
}




static void init(){ /*funkcja omowiona w sceneMenu.c */
	quit = malloc(sizeof(bool));
	*quit = false;
	text = malloc(sizeof(struct texture));
	text->lTexture = NULL; /*tekstury (jeszcze) nie ma */
	hasEnded = false; /*gra trwa */
	state = SERVER; /*pierwszy ruch nalezy do serwera */

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
	memset(board,'_',sizeof(char)*9); /*plansza do gry jest na razie pusta */
	setupBot(getClientState() == SERVER ? O : X); /*wybor symbolu gracza w zaleznosci czy jest serwerem czy klientem */
	wasInitiated = true;
}
static void unInit(){/*funkcja omowiona w sceneMenu.c */
	wasInitiated = false;
	closeSocket(); /*zamknij polaczenie */
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
	destroyTexture(text); /*usun teksture */
}
/*Funkcja obsluguje odebrana paczke danych*/
static void handleIncomingData(char data){
	switch(data){
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:{ /*wiadomosci 0-8 oznaczaja klikniete numery pol */
				board[data/3][data%3] = player; /*analogicznie do klikniecia gracza, ale tym razem przeciwnik */
				changeButtonTexture(pBtns[(int)data], BUTTON_DEFAULT, (getClientState() == SERVER ? IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_DEFAULT:IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_DEFAULT));
				changeButtonTexture(pBtns[(int)data], BUTTON_MOUSEOVER, (getClientState() == SERVER ? IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_MOUSEOVER :IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_MOUSEOVER));
				state = state == SERVER? CLIENT : SERVER;
				checkForEnd();
				break;
			}
			default: break;
			}
}
/*Funkcja obsluguje polaczenie ze zdalnym graczem (przetwarza informacje otrzymane od niego)*/
static void handleRemotePlayer(){
	if(isActiveSocket()){ /*jezeli w sockecie znajduja sie jakies informacje */
		char data;
		/*jezeli przeczytano cala informacje (w przeciwnym wypadku moglo dojsc
		 * do problemow z druga strona albo druga strona zdecydowala sie zakonczyc gre)*/
		if(SDLNet_TCP_Recv(*getSocket(),&data,sizeof(char)) == sizeof(char)){
			handleIncomingData(data);
		}
		else{
			/*polaczenie jest bledne albo druga strona sie rozlaczyla */
			closeSocket(); /*zamknij polaczenie */
			printEndMessage("Other side disconnected. Press Enter to exit."); /*Zakoncz gre i wyswietl wiadomosc*/
		}
	}
}
static void handleEvents(SDL_Event *e){/*funkcja omowiona w sceneMenu.c */
	if(!wasInitiated){
		init();
	}
	if(e->type == SDL_QUIT){
		*quit = true;
		return;
	}
	/*jezeli gra sie nie zakonczyla i teraz jest tura zdalnego uzytkownika */
	if(hasEnded == false && getClientState() != state){
		handleRemotePlayer();
	}
	if(hasEnded && e->type == SDL_KEYDOWN && (e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym == SDLK_RETURN2)){
		selectScene(MAIN_MENU); /*jezeli gra sie zakonczyla i uzytkownik wykonal polecenie na ekranie*/
	}
	int processedButton;
	for(processedButton = 0;selectedScene == CURRENT_SCENE && processedButton < btnCount; ++processedButton){
		handeEvent(pBtns[processedButton], e);
		if(wasClicked == true){/*jezeli aktualnie obslugiwany przycisk "wykonal" swoja funkcje*/
			wasClicked = false;/*wracamy do stanu domyslnego*/
			if(getClientState() == state)handleQuad(processedButton);/*przycisk zostal nacisniety, wiec wykonaj odpowiednia akcje*/
		}
	}
}
static void renderScene()/*funkcja omowiona w sceneMenu.c */
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
		if(hasEnded == true){/*jezeli gra sie zakonczyla, wyswietl komunikat na ekranie*/
			renderTexture(text,(SCREEN_WIDTH-text->width)/2,(SCREEN_HEIGHT-text->height)/2);
		}
		SDL_RenderPresent(getRenderer());
}

struct scene multiplayerScene = {&handleEvents, &renderScene, &init, &unInit};

