/*
 * main.c
 *
 *  Created on: 30 kwi 2018
 *      Author: klimek
 */
#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL_net.h>

#include "bool.h"
#include "game.h"
#include "defines.h"

#include "mediaLoader.h"
#include "mediaList.h"

#include "sceneList.h"
#include "scene.h"

SDL_Window** setWindow(SDL_Window* pGWindow){
	static SDL_Window* pointer;
	if(pointer == NULL){
		pointer = pGWindow;
	}
	return &pointer;
}
SDL_Window* getWindow(){
	static SDL_Window** pointer;
	if(pointer == NULL) pointer = setWindow(NULL);
	return *pointer;
}
SDL_Renderer** setRenderer(SDL_Renderer* pRenderer){
	static SDL_Renderer* pointer;
	if(pointer == NULL){
		pointer = pRenderer;
	}
	return &pointer;
}
SDL_Renderer* getRenderer(){
	static SDL_Renderer** p;
	if(p == NULL) p = setRenderer(NULL);
	return *p;
}
/*Funkcja inicjalizujaca biblioteke SDL*/
bool inicjalizacja(){
	bool czySukces = true;
	if(SDL_Init(SDL_INIT_VIDEO)){
		printf("SDL nie mogl zostac zinicjalizowany: %s\n", SDL_GetError());
		czySukces = false;
	}
	else {
		setWindow(SDL_CreateWindow("tutorial",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
		if(getWindow() == NULL){
			printf("SDL nie mogl utworzyc okna: %s\n", SDL_GetError());
			czySukces = false;
		}else{
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init((imgFlags)) & imgFlags)){
				printf("SDL_image nie mogl zostac zinicjalizowany: %s\n", IMG_GetError());
				czySukces = false;
			}
			else{
				if(TTF_Init() == -1){
					printf("SDL_ttf nie mogl zostac zinicjalizowany: %s\n", TTF_GetError());
					czySukces = false;
				}
				else{
					if(SDLNet_Init() == -1){
						printf("SDL_net nie mogl zostac zinicjalizowany: %s\n", TTF_GetError());
						czySukces = false;
					}
					else {
						setRenderer(SDL_CreateRenderer(getWindow(),-1,SDL_RENDERER_ACCELERATED));
						if(getRenderer() == NULL){
							printf("Renderer nie mogl zostac utworzony: %s\n", SDL_GetError());
							czySukces = false;
						}
						else{
						SDL_SetRenderDrawColor(getRenderer(),0xFF,0xFF,0xFF,0xFF);
						}
					}
				}
			}
		}
	}

	return czySukces;
}
void zamknij(){
	/*Najpierw usuwamy media, po czym wychodzimy z SDLa (odwrotna kolejnosc niz przy inicjalizacji)*/
	unloadMedia();
	SDL_DestroyRenderer(getRenderer());
	SDL_DestroyWindow(getWindow());
	SDLNet_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int run(){

	if(inicjalizacja() == false){
		perror("Nie udalo sie zinicjalizowac.\n");
	}
	else{
		if(!loadMedia()){
			perror("Nie udalo sie zaladowac mediow.\n");
		}
		else{
			/*zaczynamy na scenie z menu glownym*/
			selectScene(MAIN_MENU);
			SDL_Event e;
			/*tak dlugo, jak wybrana scena jest rozna od QUIT, obsluguj eventy i renderuj scene*/
			while(selectedScene != QUIT){
				while(SDL_PollEvent(&e) != 0){
					if(selectedScene == QUIT) break;
					currScene->handleEvents(&e);
				}
				if(selectedScene== QUIT) break;
				currScene->renderScene();
			}
			/*Petla gry zakonczona, zwalniamy zasoby*/
			zamknij();

		}
	}
	return 0;
}
void restoreDefaults(){
	FILE* saveFile = fopen("save.dat","rb");
	if(saveFile != NULL){
		fclose(saveFile);
		remove("save.dat");/*usun plik(o ile istnieje)*/
	}
	saveFile = fopen("scores.dat", "rb");
	if(saveFile != NULL){
		fclose(saveFile);
		remove("scores.dat");
	}
}
int difficulty(int difficulty){
	static int dif;
	if(dif == 0) dif = 3;
	if(difficulty > 0 && difficulty < 5) dif = difficulty;
	return dif;
}
