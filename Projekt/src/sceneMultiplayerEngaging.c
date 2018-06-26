/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include "sceneMultiplayerEngaging.h"
#include "defines.h"
#include "sceneGlobals.h"
#include "game.h"
#include <string.h>

static int btnCount = 2;
static int inputBoxCount = 1;

static IPaddress* ip = NULL;
static TCPsocket* socket = NULL;
static TCPsocket* serverSocket = NULL;
static SDLNet_SocketSet* socketSet = NULL;
static int startServer();
static int connectToServer(char* ipAddr);
static void stopServer();
static bool clientConnected();

#define IMG_BACKGROUND IMG_SCENE_MULTIPLAYER_ENGAGING_BG_STATIC
#define CURRENT_SCENE MULTIPLAYER_ENGAGING

/*Funkcje przyciskow*/

static void clientServerBtnClicked(){
	if(multiplayerState == SERVER){ /*jezeli gra byla serwerem*/
		stopServer(); /*zakoncz dzialanie serwera*/
		inputBoxToggleVisibility(pInputBoxes[0]);/*pokaz pole do wpisywania ip*/
		multiplayerState = CLIENT;/*od teraz jestem klientem*/
		/*zmiana przycisku*/
		changeButtonTexture(pBtns[0],BUTTON_DEFAULT,IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CLIENT_DEFAULT_MOUSEOVER);
		changeButtonTexture(pBtns[0],BUTTON_MOUSEOVER,IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CLIENT_DEFAULT_MOUSEOVER);
		SDL_StartTextInput();/*od teraz mozna wpisywac znaki do pola tekstowego*/
	}
	else{/*jezeli gra byla klientem*/
		if(!startServer()){/*jezeli udalo sie wystartowac serwer(port nie byl zajety)*/
			SDL_StopTextInput();/*juz nie mozna wpisywac znakow*/
			inputBoxClearTextField(pInputBoxes[0]);/*wyczysc zawartosc pola ip*/
			inputBoxToggleVisibility(pInputBoxes[0]);/*ukryj pole do wpisywania ip*/
			multiplayerState = SERVER;/*od teraz jestem serwerem*/
			/*zmiana przycisku*/
			changeButtonTexture(pBtns[0],BUTTON_DEFAULT,IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_SERVER_DEFAULT_MOUSEOVER);
			changeButtonTexture(pBtns[0],BUTTON_MOUSEOVER,IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_SERVER_DEFAULT_MOUSEOVER);
		}
	}

}

/*Funkcje przyciskow*/

static void init(){/*Funkcja opisana w sceneMenu.c*/
	quit = malloc(sizeof(bool));
	*quit = false;
	if(btnCount){
		pBtns = malloc(btnCount*sizeof(struct button*));
	}
	if(inputBoxCount){
		pInputBoxes = malloc(inputBoxCount*sizeof(struct inputBox*));
	}
	/*place buttons and input fields here */
	pBtns[0] = createButton(250,125,IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CLIENT_DEFAULT_MOUSEOVER,
			IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CLIENT_DEFAULT_MOUSEOVER,
			IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CLIENT_DEFAULT_MOUSEOVER,&clientServerBtnClicked);
	pBtns[1] = createButton((SCREEN_WIDTH-300)/2, (SCREEN_HEIGHT-150)/2,IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CONNECTING_DEFAULT_MOUSEOVER,
			IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CONNECTING_DEFAULT_MOUSEOVER,IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CONNECTING_DEFAULT_MOUSEOVER,NULL);
	toggleButtonVisibility(pBtns[1]); /*ten przycisk zawiera informacje ze polaczenie jest nawiazywane, wiec na razie jest ukrywany*/
	pInputBoxes[0] = createInputBox(15,292,290,true,FONT_OPENSANS_BOLD,255,255,255,true,IMG_SCENE_MULTIPLAYER_ENGAGING_INPUT_BG);


	/*end of button and input fields placement*/
	multiplayerState = CLIENT; /*zaczynam jako klient*/
	SDL_StartTextInput();
	wasInitiated = true;

}
static void unInit(){/*Funkcja opisana w sceneMenu.c*/
	wasInitiated = false;
	free(quit);
	int i;
	for(i=0; i < btnCount; ++i){
		destroyButton(pBtns[i]);
	}
	for(i=0; i < inputBoxCount; ++i){
			destroyInputBox(pInputBoxes[i]);
		}
	free(pBtns);
	free(pInputBoxes);
	if(serverSocket != NULL){ /*jezeli serwer nasluchiwal, to musi zamknac nasluchujacy socket*/
		SDLNet_TCP_Close(*serverSocket);
		free(serverSocket);
		serverSocket = NULL;
	}

}


static void handleEvents(SDL_Event *e){/*Funkcja opisana w sceneMenu.c*/

	if(!wasInitiated){
		init();
	}
	if(e->type == SDL_QUIT){
		*quit = true;
		return;
	}
	if(multiplayerState == CLIENT){ /*jezeli klient*/
		if(e->type == SDL_KEYDOWN){
			if(e->key.keysym.sym == SDLK_BACKSPACE){ /*jezeli nacisnieto backspace, usun znak z pola tekstowego*/
				inputBoxPopCharacter(pInputBoxes[0]);
			}
			/*jezeli nacisnieto enter, nalezy wykonac probe polaczenia*/
			else if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym ==SDLK_RETURN2 ) {
				/*zablokuj mozliwosc zmiany w serwer, pokaz komunikat o zmianie, ukryj okno do wpisywania*/
				pBtns[0]->pFunction = NULL;
				toggleButtonVisibility(pBtns[1]);
				inputBoxToggleVisibility(pInputBoxes[0]);
				currScene->renderScene();/*wyrenderuj poczynione zmiany*/
				/*jezeli udalo sie polaczyc, zablokuj mozliwosc wpisywania znakow i zmien scene*/
				if(!connectToServer(pInputBoxes[0]->textField)){
					SDL_StopTextInput();
					selectScene(MULTIPLAYER);
				}
				else{/*w przeciwnym wypadku wracamy do punktu wyjscia*/
					toggleButtonVisibility(pBtns[1]);
					inputBoxToggleVisibility(pInputBoxes[0]);
					pBtns[0]->pFunction = &clientServerBtnClicked;
					SDL_StartTextInput();
				}
			}
		}
		/*nie usuwamy znaku, nie probujemy wykonac polaczenia,
		 * wiec jezeli uzytkownik wpisuje znak, dodajemy go do pola tekstowego
		 */
		else if(e->type == SDL_TEXTINPUT){
			inputBoxAppendChar(pInputBoxes[0],e->text.text[0]);
		}
	}
	else{/*jezeli jestesmy serwerem*/
		if(serverSocket == NULL){
			/*taka sytuacja nie ma prawa nastapic,
			 * ale w przypadku ingerencji w pamiec nie mozna dopuscic
			 * do sprawdzenia aktywnosci socketu*/
		}
		else if(clientConnected()){/*jezeli klient sie polaczyl, zmien scene*/
			selectScene(MULTIPLAYER);
		}
	}
	int processedButton;
	for(processedButton = 0;selectedScene == CURRENT_SCENE && processedButton < btnCount; ++processedButton){
		handeEvent(pBtns[processedButton], e);
	}
}
static void renderScene()/*Funkcja opisana w sceneMenu.c*/
{
	if(*quit == true){
		selectScene(QUIT);
		return;
	}
	if(currTex == NULL){
		currTex = getTexture(IMG_BACKGROUND);
	}
		SDL_RenderClear(getRenderer());
		SDL_RenderCopy(getRenderer(),currTex,NULL,NULL);
		int processed;
		for(processed = 0; processed < btnCount; ++processed){
			renderButton(pBtns[processed]);
		}
		for(processed = 0; processed < inputBoxCount; ++processed){
					inputBoxRender(pInputBoxes[processed]);
		}
		SDL_RenderPresent(getRenderer());
}


struct scene multiplayerEngagingScene = {&handleEvents, &renderScene, &init, &unInit};

/*Funkcja uruchamia serwer*/
static int startServer(){
	if(ip == NULL){
		ip = malloc(sizeof(IPaddress));
	}
	if(SDLNet_ResolveHost(ip,NULL,63334) == -1){/*Tworzy adres ip dla serwera. Jezeli sie nie powiodlo to wycofaj zmiany*/
		free(ip);
		ip = NULL;
		return -1;
	}
	if(serverSocket== NULL){
		serverSocket = malloc(sizeof(TCPsocket));
	}
	*serverSocket =SDLNet_TCP_Open(ip);/*Tworzy socket do nasluchiwania. Jezeli sie nie powiodlo to wycofaj zmiany*/
	if(!(*serverSocket)){
		free(serverSocket);
		free(ip);
		serverSocket = NULL;
		ip = NULL;
		return -1;
	}
	/*udalo sie uruchomic serwer*/
	return 0;
}

/*Funkcja wylacza serwer*/
static void stopServer(){
	if(serverSocket != NULL){/*jezeli istnieje socket, na ktorym serwer nasluchuje*/
		/*zamknij socket i uwolnij zarezerwowana na serwer pamiec*/
		SDLNet_TCP_Close(*serverSocket);
		free(serverSocket);
		serverSocket = NULL;
		free(ip);
		ip = NULL;
	}
}

/*Funkcja obsluguje polaczenie klienta.
 * Jezeli takie nastopilo, zwraca true, a w przeciwnym wypadku false
 */
static bool clientConnected(){
	TCPsocket sock = SDLNet_TCP_Accept(*serverSocket); /*odbierz polaczenie z socketu*/
	if(sock == NULL) return false; /*jezeli polaczenia nie bylo zakoncz*/
	if(socket == NULL){
		socket = malloc(sizeof(TCPsocket));
	}
	*socket = sock; /*przypisz odebrane polaczenie do bardziej dostepnej zmiennej*/
	if(socketSet == NULL)
		socketSet = malloc(sizeof(SDLNet_SocketSet));
	*socketSet = SDLNet_AllocSocketSet(5);/*Utworz zbior socketow i dodaj do niego nowo utworzony socket*/
	SDLNet_TCP_AddSocket(*socketSet,*socket);
	stopServer();/*znaleziono klienta, serwer nasluchujacy juz nie jest potrzebny*/
	return true;
}

/*Funkcja laczy sie z serwerem, zwraca -1 w przypadku niepowodzenia i 0 w przeciwnym wypadku. Parametry:
 * ipAddr - ciag znakow bedacy adresem IP serwera
 */
static int connectToServer(char* ipAddr){
	if(ip == NULL){
		ip = malloc(sizeof(IPaddress));
	}
	if(SDLNet_ResolveHost(ip,ipAddr,63334) == -1){ /*proba utworzenia adresu IP z podanego ciagu znakow*/
		/*jezeli sie nie udalo, cofnij zmiany*/
		free(ip);
		ip = NULL;
		return -1;
	}
	if(socket == NULL){
		socket = malloc(sizeof(TCPsocket));
	}
	*socket = SDLNet_TCP_Open(ip); /*Utworz socket i nawiaz polaczenie z serwerem*/
	if(!(*socket)){
		/*jezeli sie nie udalo, cofnij zmiany*/
		free(socket);
		free(ip);
		socket = NULL;
		ip = NULL;
		return -1;
	}
	/*utworzenie zbioru socketow do obslugi polaczenia*/
	if(socketSet == NULL)
		socketSet = malloc(sizeof(SDLNet_SocketSet));
	*socketSet = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(*socketSet,*socket);
	return 0;
}
TCPsocket* getSocket(){
	return socket;
}
enum multis getClientState(){
	return multiplayerState;
}
void closeSocket(){
	if(socket != NULL){ /*jezeli socket istnieje*/
		SDLNet_FreeSocketSet(*socketSet); /*usun zbior socketow*/
		free(socketSet);
		socketSet = NULL;
		SDLNet_TCP_Close(*socket);/*zamknij socket*/
		free(socket);
		socket = NULL;
	}
}
bool sendMessage(enum messageType msg){
	char packet = msg;
	int result = SDLNet_TCP_Send(*socket,&packet,sizeof(char));/*wyslanie wiadomosci*/
	if(result < sizeof(char)){
		/*jezeli nie udalo sie wyslac calej wiadomosci*/
		return false;
	}
	return true;
}
SDLNet_SocketSet* getSocketSet(){
	return socketSet;
}
bool isActiveSocket(){
	if(SDLNet_CheckSockets(*socketSet,0)>0){ /*jezeli w grupie istnieje aktywny socket*/
		if(SDLNet_SocketReady(*socket)){/*jezeli konkretnie ten socket jest aktywny*/
			return true;
		}
	}
	return false;
}
