/*
 * sceneMenu.h
 *
 *  Created on: 21 maj 2018
 *      Author: klimek
 */

#ifndef SCENESCOREBOARD_H_
#define SCENESCOREBOARD_H_
#include "scene.h"
#include "sceneList.h"
#include "mediaLoader.h"
#include "mediaList.h"
#include "button.h"
#include "inputBox.h"
#include <SDL2/SDL.h>
#include "bool.h"

extern struct scene scoreboardScene;
void addScore(int score);

#endif /* SCENESCOREBOARD_H_ */
