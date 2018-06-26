/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include "sceneMenu.h"
#include "sceneGlobals.h"
#include "game.h"
#include <string.h>

/*ilosc przyciskow i pol tekstowych*/
static int btnCount = 3;
static int inputBoxCount = 0;
/*Funkcje przyciskow*/
static void quitBtnClicked(){
	selectScene(QUIT);
}
static void singleBtnClicked(){
	selectScene(SINGLEPLAYER);
}
static void multiBtnClicked(){
	selectScene(MULTIPLAYER_ENGAGING);
}
/*koniec funkcji przyciskow*/
/*tlo sceny, aktualna scena */
#define IMG_BACKGROUND IMG_SCENE_MENU_BG_STATIC
#define CURRENT_SCENE MAIN_MENU

static void init(){
	quit = malloc(sizeof(bool));
	*quit = false;
	/*alokacja pamieci na przyciski i pola tekstowe*/
	if(btnCount){
		pBtns = malloc(btnCount*sizeof(struct button*));
	}
	if(inputBoxCount){
		pInputBoxes = malloc(inputBoxCount*sizeof(struct inputBox*));
	}
	/*tworzenie przyciskow */
	pBtns[0] = createButton(250,125,IMG_SCENE_MENU_BTN_SINGLEGAME_DEFAULT, IMG_SCENE_MENU_BTN_SINGLEGAME_MOUSEOVER,IMG_SCENE_MENU_BTN_SINGLEGAME_MOUSEOVER, &singleBtnClicked);
	pBtns[1] = createButton(250,280,IMG_SCENE_MENU_BTN_MULTIPLAYER_DEFAULT, IMG_SCENE_MENU_BTN_MULTIPLAYER_MOUSEOVER,IMG_SCENE_MENU_BTN_MULTIPLAYER_MOUSEOVER, &multiBtnClicked);
	pBtns[2] = createButton(250,435,IMG_SCENE_MENU_BTN_QUITGAME_DEFAULT, IMG_SCENE_MENU_BTN_QUITGAME_MOUSEOVER,IMG_SCENE_MENU_BTN_QUITGAME_MOUSEOVER, &quitBtnClicked);
	/*to juz wszystko */
	wasInitiated = true;
}
static void unInit(){
	/*trzeba inicjalizowac na nowo */
	wasInitiated = false;
	free(quit); /*zwolnienie pamieci*/
	int i;
	for(i=0; i < btnCount; ++i){
		destroyButton(pBtns[i]); /*usuwanie przyciskow */
	}
	for(i=0; i < inputBoxCount; ++i){
			destroyInputBox(pInputBoxes[i]); /*usuwanie pol tekstowych */
		}
	/*usuwanie tablic */
	free(pBtns);
	free(pInputBoxes);
}


static void handleEvents(SDL_Event *e){
	/*jezeli scena nie zostala zainicjalizowana, inicjalizuj */
	if(!wasInitiated){
		init();
	}
	/*jezeli okno ma zostac zamkniete, ustaw odpowiednia zmienna i nie przetwarzaj eventow */
	if(e->type == SDL_QUIT){
		*quit = true;
		return;
	}
	/*sprawdz obecny event na kazdym z przyciskow, o ile scena nie ulegnie zmianie
	 *  (moze nastapic zmiana sceny przez nacisniecie przycisku) */
	int processedButton;
	for(processedButton = 0;selectedScene == CURRENT_SCENE && processedButton < btnCount; ++processedButton){
		handeEvent(pBtns[processedButton], e);
	}
}
static void renderScene()
{
	/*jezeli uzytkownik chce wyjsc, zmien scene na finalna i nie renderuj */
	if(*quit == true){
		selectScene(QUIT);
		return;
	}
	/*jezeli tekstura tla nie jest obecna, pobierz ja */
	if(currTex == NULL){
		currTex = getTexture(IMG_BACKGROUND);
	}
		SDL_RenderClear(getRenderer()); /*wyczysc okno*/
		SDL_RenderCopy(getRenderer(),currTex,NULL,NULL); /*wyrenderuj tlo*/
		/*wyrenderuj przyciski*/
		int processed;
		for(processed = 0; processed < btnCount; ++processed){
			renderButton(pBtns[processed]);
		}
		/*wyrenderuj pola tekstowe*/
		for(processed = 0; processed < inputBoxCount; ++processed){
					inputBoxRender(pInputBoxes[processed]);
		}
		/*pokaz na ekranie*/
		SDL_RenderPresent(getRenderer());
}

/*obiekt sceny*/
struct scene menuScene = {&handleEvents, &renderScene, &init, &unInit};

