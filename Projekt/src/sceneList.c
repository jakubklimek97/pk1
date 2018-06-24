/*
 * sceneList.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */

#include "sceneList.h"
#include "scene.h"
/*extern for every scene */
#include "sceneMultiplayerEngaging.h"
#include "sceneSingleplayer.h"
struct scene testScene;
struct scene menuScene;
/*end extern*/

struct scene* sceneList[] = {
		&menuScene,
		&testScene,
		&singleplayerScene,
		&multiplayerEngagingScene
};
