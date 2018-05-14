/*
 * scene.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */


#include "scene.h"
#include "sceneList.h"
void selectScene(enum sceneList scene){
	currScene = sceneList[scene];
	selectedScene = scene;
}
