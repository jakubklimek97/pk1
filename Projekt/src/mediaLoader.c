/*
 * mediaLoader.cpp
 *
 *  Created on: 30 kwi 2018
 *      Author: klimek
 */

#include "mediaLoader.h"
#include "game.h"
#include "mediaList.h"

#define FONT_SIZE 28

bool loadMedia(){
	pTextureList = NULL;
	bool success = true;
	if(MEDIA_COUNT > 0){
		SDL_Surface* pTmpSur;/*Tymczasowa powierzchnia*/
		int currMedia = 0;
		while(currMedia < MEDIA_COUNT){
			/*zaladuj tymczasowa powierzchnie*/
			pTmpSur = IMG_Load(mediaLocations[currMedia]);
			if(pTmpSur == NULL){
				printf("Nie mozna zaladowac obrazu: %s %s\n", mediaLocations[currMedia],IMG_GetError());
				success = false;
			}
			else {
				/*Utworz element na teksture*/
				struct textureList* ptr = listAddTexture(pTextureList,currMedia,malloc(sizeof(struct texture)));
				ptr->pElement->height =pTmpSur->h;
				ptr->pElement->width = pTmpSur->w;
				ptr->pElement->lTexture = SDL_CreateTextureFromSurface(getRenderer(),pTmpSur);/*utworz teksture z powierzchni */
				if(ptr->pElement->lTexture == NULL){/*jezeli nie udalo sie*/
					printf("Nie mozna zaladowac tekstury: %s Blad: %s\n", mediaLocations[currMedia],SDL_GetError());
					success = false;
				}
				if(currMedia == 0) pTextureList = ptr;/*pierwsza tekstura bedzie glowa listy*/
			}
			SDL_FreeSurface(pTmpSur);/*to juz nie jest potrzebne*/
			++currMedia;
		}
		pFonts = malloc(FONT_COUNT*sizeof(TTF_Font*));
		while(currMedia-MEDIA_COUNT < FONT_COUNT){
			pFonts[currMedia-MEDIA_COUNT] = TTF_OpenFont(mediaLocations[currMedia],FONT_SIZE);/*Odczyt czcionki*/
			if(pFonts[currMedia-MEDIA_COUNT] == NULL){/*Jezeli nie udalo sie przypisac czcionki*/
				printf("Nie mozna zaladowac czcionki %s %s\n", mediaLocations[currMedia],TTF_GetError());
				success = false;
			}
			++currMedia;
		}
	}
	return success;/*Nie bylo bledow, wiec sukces*/
}

SDL_Texture* getTexture(int mediaNumber){
	return listGetTexture(pTextureList, mediaNumber)->lTexture;
}
struct texture* getTextureStr(int mediaNumber){
	return listGetTexture(pTextureList,mediaNumber);
}
TTF_Font* getFont(int fontNumber){
	return pFonts[fontNumber];
}
void unloadMedia(){
	int currMedia = MEDIA_COUNT;
	while(currMedia-MEDIA_COUNT < FONT_COUNT){
		TTF_CloseFont(pFonts[currMedia-MEDIA_COUNT]);/*Zwolnij czcionki*/
		currMedia++;
	}
	free(pFonts);/*Zwolnij miejsce na wskazniki na czcionki*/
	freeList(pTextureList);/*zwolnij liste tekstur*/
}


