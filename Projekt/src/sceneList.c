/*
 * sceneList.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */

#include "sceneList.h"
#include "scene.h"
#include "sceneMultiplayerEngaging.h"
#include "sceneMultiplayer.h"
#include "sceneSingleplayer.h"
#include "sceneScoreboard.h"
#include "sceneMenu.h"

struct scene* sceneList[] = {
		&menuScene,
		&singleplayerScene,
		&multiplayerEngagingScene,
		&scoreboardScene,
		&multiplayerScene
};
