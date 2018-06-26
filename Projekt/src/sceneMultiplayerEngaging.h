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
/*stan gry*/
enum multis {
	SERVER,
	CLIENT,
} multiplayerState;
/*tresc wiadomosci*/
enum messageType {
	/*0-8 - recently assigned tile */
	ABORT_CONNECTION = 9,
	CHECK_CONNECTION = 10
};
/*Funkcja zwraca adres socketu */
TCPsocket* getSocket();

/*Funkcja zamyka socket*/
void closeSocket();

/*Funkcja zwraca stan gry (klient/serwer)*/
enum multis getClientState();

/*Funkcja wysyla wiadomosc przez socket. Zwraca, czy operacja sie powiodla. Parametry:
 * msg - wiadomosc do wyslania
 */
bool sendMessage(enum messageType msg);

/*Funkcja zwraca adres zmiennej typu SDL_SocketSet*/
SDLNet_SocketSet* getSocketSet();

/*Funkcja sprawdza, czy socket mial w ostatnim czasie jakas aktywnosc*/
bool isActiveSocket();

#endif /* SCENEMULTIPLAYER_ENGAGING_H_ */
