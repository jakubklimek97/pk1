/*
 * sceneList.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */

#include "sceneList.h"
#include "scene.h"
/*extern for every scene */
struct scene testScene;
/*end extern*/

struct scene* sceneList = {
		&testScene
};
