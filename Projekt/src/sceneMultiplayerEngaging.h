/*
 * sceneMultiplayerEngaging.h
 *
 *  Created on: 22 maj 2018
 *      Author: klimek
 */

#ifndef SCENEMULTIPLAYER_ENGAGING_H_
#define SCENEMULTIPLAYER_ENGAGING_H_
#include "scene.h"
#include "sceneList.h"
#include "mediaLoader.h"
#include "mediaList.h"
#include "button.h"
#include "inputBox.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "bool.h"

extern struct scene multiplayerEngagingScene;
enum multis {
	SERVER,
	CLIENT,
} multiplayerState;
enum messageType {
	/*0-8 - recently assigned tile */
	ABORT_CONNECTION = 9,
	CHECK_CONNECTION = 10
};
TCPsocket* getSocket();
void closeSocket();
enum multis getClientState();
bool sendMessage(enum messageType msg);
SDLNet_SocketSet* getSocketSet();
bool isActiveSocket();

#endif /* SCENEMULTIPLAYER_ENGAGING_H_ */
