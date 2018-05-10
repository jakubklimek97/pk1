/*
 * scene.h
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */

#ifndef SCENE_H_
#define SCENE_H_
#include <SDL2/SDL.h>
#include "sceneList.h"



struct scene{
	void (*handleEvents)(SDL_Event *e);
	void (*renderScene)();
};
enum sceneList selectedScene;
struct scene* currScene;
void selectScene(enum sceneList scene);



#endif /* SCENE_H_ */