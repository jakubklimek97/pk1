/*
 * scene.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */


#include "scene.h"
#include "sceneList.h"
void selectScene(enum sceneList scene){
	if(currScene != NULL && currScene->destroyScene != NULL)
	{
		currScene->destroyScene();
	}

	currScene = sceneList[scene];
	selectedScene = scene;
	if(scene != QUIT){
		if(currScene->initScene != NULL){
				currScene->initScene();
		}
	}

}
