/*
 * mediaLoader.cpp
 *
 *  Created on: 30 kwi 2018
 *      Author: klimek
 */

#include "mediaLoader.h"
#include "mediaList.h"
/*xtern SDL_Surface* gScreenSurface;*/
extern SDL_Renderer* gRenderer;
bool loadMedia(){
	/*pSurfaces = NULL;*/
	pTextures = NULL;
	pTexture = NULL;
	bool success = true;
	if(MEDIA_COUNT > 0){
		/*pSurfaces = malloc(MEDIA_COUNT*sizeof(SDL_Surface*));*/
		pTextures = malloc(MEDIA_COUNT*sizeof(SDL_Texture*));
		pTexture = malloc(MEDIA_COUNT*sizeof(struct texture*));
		SDL_Surface* pTmpSur;
		int currMedia = 0;
		while(currMedia < MEDIA_COUNT){
			pTmpSur = IMG_Load(mediaLocations[currMedia]);
			if(pTmpSur == NULL){
				printf("Nie mozna zaladowac obrazu: %s %s\n", mediaLocations[currMedia],IMG_GetError());
				success = false;
			}
			/*pSurfaces[currMedia] = SDL_ConvertSurface(pTmpSur,gScreenSurface->format,0);
			if(pSurfaces[currMedia] == NULL){
				printf("Nie mozna zoptymalizowac obrazu: %s %s\n", mediaLocations[currMedia], SDL_GetError());
				success = false;
			}*/
			else {
				pTextures[currMedia] = SDL_CreateTextureFromSurface(gRenderer,pTmpSur);
				pTexture[currMedia] = (struct texture*)malloc(sizeof(struct texture));
				pTexture[currMedia]->height = pTmpSur->h;
				pTexture[currMedia]->width = pTmpSur->w;
				pTexture[currMedia]->lTexture = SDL_CreateTextureFromSurface(gRenderer,pTmpSur);
				if(pTexture[currMedia]->lTexture == NULL){
					printf("Nie mozna zaladowac tekstury: %s Blad: %s\n", mediaLocations[currMedia],SDL_GetError());
					success = false;
				}

			}

			SDL_FreeSurface(pTmpSur);
			++currMedia;
		}
	}
	return success;
}
/*SDL_Surface* getSurface(int mediaNumber){
	return pSurfaces[mediaNumber];
}*/
SDL_Texture* getTexture(int mediaNumber){
	return pTexture[mediaNumber]->lTexture;
}
void unloadMedia(){
	int currMedia = 0;
	while(currMedia < MEDIA_COUNT){
		/*SDL_FreeSurface(pSurfaces[currMedia]);*/
		SDL_DestroyTexture(pTextures[currMedia]);
		SDL_DestroyTexture(pTexture[currMedia]->lTexture);
		free(pTexture[currMedia]);
		currMedia++;
	}
	/*free(pSurfaces);*/
	free(pTextures);
	free(pTexture);
}
