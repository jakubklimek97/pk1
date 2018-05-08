/*
 * main.c
 *
 *  Created on: 30 kwi 2018
 *      Author: klimek
 */
#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "bool.h"
#include "defines.h"
#include "mediaLoader.h"
#include "mediaList.h"

SDL_Window* gWindow = NULL;
/*SDL_Surface* gScreenSurface = NULL;*/
SDL_Surface* gHelloWorld = NULL;
SDL_Renderer* gRenderer = NULL;

bool inicjalizacja(){
	bool czySukces = true;
	if(SDL_Init(SDL_INIT_VIDEO)){
		printf("SDL nie mogl zostac zinicjalizowany: %s\n", SDL_GetError());
		czySukces = false;
	}
	else {
		gWindow = SDL_CreateWindow("tutorial",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL){
			printf("SDL nie mogl utworzyc okna: %s\n", SDL_GetError());
			czySukces = false;
		}else{
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init((imgFlags)) & imgFlags)){
				printf("SDL_image nie mogl zostac zinicjalizowany: %s\n", IMG_GetError());
				czySukces = false;
			}
			/*gScreenSurface = SDL_GetWindowSurface(gWindow);*/
			gRenderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
			if(gRenderer == NULL){
				printf("Renderer nie mogl zostac utworzony: %s\n", SDL_GetError());
				czySukces = false;
			}
			else{
				SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
			}
		}
	}

	return czySukces;
}
void zamknij(){
	unloadMedia();
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}

int main(){
	if(inicjalizacja() == false){
		perror("Nie udalo sie zinicjalizowac.\n");
	}
	else{
		if(!loadMedia()){
			perror("Nie udalo sie zaladowac mediow.\n");
		}
		else{
			bool quit = false;
			SDL_Event e;
			/*SDL_Surface* curr = getSurface(IMG_HELLOWORD);*/
			SDL_Texture* currTex = getTexture(IMG_HELLOWORD);

			SDL_Rect stretchRect;
			stretchRect.x = 200;
			stretchRect.y = 200;
			stretchRect.w = 100;
			stretchRect.h = 70;
			/*SDL_UpdateWindowSurface(gWindow);*/
			while(!quit){
				while(SDL_PollEvent(&e) != 0){
					if(e.type == SDL_QUIT){
						quit = true;
					}
					if(e.type == SDL_KEYDOWN){
						switch(e.key.keysym.sym){
						case SDLK_UP:
							/*curr = getSurface(IMG_UP);*/
							currTex = getTexture(IMG_UP);
							break;
						case SDLK_DOWN:
							/*curr = getSurface(IMG_DOWN);*/
							currTex = getTexture(IMG_DOWN);
							break;
						case SDLK_LEFT:
							/*curr = getSurface(IMG_LEFT);*/
							currTex = getTexture(IMG_LEFT);
							break;
						case SDLK_RIGHT:
							/*curr = getSurface(IMG_RIGHT);*/
							currTex = getTexture(IMG_RIGHT);
							break;
						default:
							/*curr = getSurface(IMG_HELLOWORD);*/
							currTex = getTexture(IMG_HELLOWORD);
							break;
						}
					}
				}
				SDL_RenderClear(gRenderer);
				SDL_RenderCopy(gRenderer,currTex,NULL,NULL);
				SDL_RenderCopy(gRenderer,currTex,NULL,&stretchRect);
				SDL_RenderPresent(gRenderer);
				/*(SDL_BlitScaled(curr,NULL,gScreenSurface,&stretchRect);
				SDL_UpdateWindowSurface(gWindow);*/
			}

			zamknij();
		}
	}
	return 0;
}
