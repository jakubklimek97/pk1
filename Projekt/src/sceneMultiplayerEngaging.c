/*
 * sceneTest.c
 *
 *  Created on: 10 maj 2018
 *      Author: klimek
 */
#include "sceneMultiplayerEngaging.h"
#include "defines.h"
#include <string.h>


extern SDL_Renderer* gRenderer;
static  SDL_Texture* currTex = NULL;
static bool* quit;
static bool wasInitiated = false;
static struct button** pBtns = NULL;
static struct inputBox** pInputBoxes = NULL;

static int btnCount = 2;
static int inputBoxCount = 1;



static IPaddress* ip = NULL;
static TCPsocket* socket = NULL;
static TCPsocket* serverSocket = NULL;
static SDLNet_SocketSet* socketSet = NULL;
int startServer();
int connectToServer(char* ipAddr);
void stopServer();
bool clientConnected();
/*button functions*/

static void clientServerBtnClicked(){
	stopServer();
	inputBoxToggleVisibility(pInputBoxes[0]);
	multiplayerState = CLIENT;
	SDL_StartTextInput();
}

/*end of button functions*/

#define IMG_BACKGROUND IMG_SCENE_MULTIPLAYER_ENGAGING_BG_STATIC
#define CURRENT_SCENE MULTIPLAYER_ENGAGING

static void init(){
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
	pBtns[1] = createButton((SCREEN_WIDTH-300)/2, (SCREEN_HEIGHT-150)/2,IMG_SCENE_MENU_BTN_MULTIPLAYER_DEFAULT,
			IMG_SCENE_MENU_BTN_MULTIPLAYER_DEFAULT,IMG_SCENE_MENU_BTN_MULTIPLAYER_DEFAULT,NULL);
	toggleButtonVisibility(pBtns[1]);
	pInputBoxes[0] = createInputBox(15,292,290,false,FONT_OPENSANS_BOLD,255,255,255,true,IMG_SCENE_MULTIPLAYER_ENGAGING_INPUT_BG);


	/*end of button and input fields placement*/
	multiplayerState = SERVER;
	startServer();
	wasInitiated = true;

}
static void unInit(){
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
	if(serverSocket != NULL){
		SDLNet_TCP_Close(*serverSocket);
		free(serverSocket);
		serverSocket = NULL;
	}

}


static void handleEvents(SDL_Event *e){

	if(!wasInitiated){
		init();
	}
	if(e->type == SDL_QUIT){
		*quit = true;
		return;
	}
	if(multiplayerState == CLIENT){
		if(e->type == SDL_KEYDOWN){
			if(e->key.keysym.sym == SDLK_BACKSPACE){
				inputBoxPopCharacter(pInputBoxes[0]);
			}
			else if(e->key.keysym.sym == SDLK_RETURN || e->key.keysym.sym ==SDLK_RETURN2 ) {
				pBtns[0]->pFunction = NULL;
				toggleButtonVisibility(pBtns[1]);
				inputBoxToggleVisibility(pInputBoxes[0]);
				printf("Connecting... to %s\n",pInputBoxes[0]->textField);
				if(!connectToServer(pInputBoxes[0]->textField)){
					SDL_StopTextInput();
					selectScene(MULTIPLAYER);
				}
				else{
					printf("jestem tutaj\n");
					toggleButtonVisibility(pBtns[1]);
					inputBoxToggleVisibility(pInputBoxes[0]);
					pBtns[0]->pFunction = &clientServerBtnClicked;
					SDL_StartTextInput();
				}
			}
		}
		else if(e->type == SDL_TEXTINPUT){
			inputBoxAppendChar(pInputBoxes[0],e->text.text[0]);
		}
	}
	else{
		if(serverSocket == NULL){
			/* do sth */
		}
		else if(clientConnected()){
			selectScene(MULTIPLAYER);
		}
	}
	/*if(e->type == SDL_KEYDOWN){
	switch(e->key.keysym.sym){
				case SDLK_UP:
					currTex = getTexture(IMG_UP);
					break;
				case SDLK_DOWN:
					currTex = getTexture(IMG_DOWN);
					break;
				case SDLK_LEFT:
					currTex = getTexture(IMG_LEFT);
					break;
				case SDLK_RIGHT:
					currTex = getTexture(IMG_RIGHT);
					break;
				default:
					currTex = getTexture(IMG_HELLOWORD);
					break;
		}*/
	int processedButton;
	for(processedButton = 0;selectedScene == CURRENT_SCENE && processedButton < btnCount; ++processedButton){
		handeEvent(pBtns[processedButton], e);
	}
}
static void renderScene()
{
	if(*quit == true){
		selectScene(QUIT);
		return;
	}
	if(currTex == NULL){
		currTex = getTexture(IMG_BACKGROUND);
	}
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer,currTex,NULL,NULL);
		int processed;
		for(processed = 0; processed < btnCount; ++processed){
			renderButton(pBtns[processed]);
		}
		for(processed = 0; processed < inputBoxCount; ++processed){
					inputBoxRender(pInputBoxes[processed]);
		}
		SDL_RenderPresent(gRenderer);
}


struct scene multiplayerEngagingScene = {&handleEvents, &renderScene, &init, &unInit};

int startServer(){
	if(ip == NULL){
		ip = malloc(sizeof(IPaddress));
	}
	if(SDLNet_ResolveHost(ip,NULL,63334) == -1){
		printf("Problem przy serwerowaniu\n");
		free(ip);
		ip = NULL;
		return -1;
	}
	if(serverSocket== NULL){
		serverSocket = malloc(sizeof(TCPsocket));
	}
	*serverSocket =SDLNet_TCP_Open(ip);
	if(!(*serverSocket)){
		free(serverSocket);
		free(ip);
		serverSocket = NULL;
		ip = NULL;
		printf("Nie mozna utworzyc serw socketa\n");
		return -1;
	}
	return 0;
}
void stopServer(){
	if(serverSocket != NULL){
		SDLNet_TCP_Close(*serverSocket);
		free(serverSocket);
		serverSocket = NULL;
	}
}
bool clientConnected(){
	TCPsocket sock = SDLNet_TCP_Accept(*serverSocket);
	if(sock == NULL) return false;
	if(socket == NULL){
		socket = malloc(sizeof(TCPsocket));
	}
	*socket = sock;
	if(socketSet == NULL)
		socketSet = malloc(sizeof(SDLNet_SocketSet));
	*socketSet = SDLNet_AllocSocketSet(5);
	SDLNet_TCP_AddSocket(*socketSet,*socket);
	stopServer();
	return true;
}
int connectToServer(char* ipAddr){
	if(ip == NULL){
		ip = malloc(sizeof(IPaddress));
	}
	if(SDLNet_ResolveHost(ip,ipAddr,63334) == -1){
		free(ip);
		ip = NULL;
		printf("Niepoprawny adres ip\n");
		return -1;
	}
	if(socket == NULL){
		socket = malloc(sizeof(TCPsocket));
	}
	*socket = SDLNet_TCP_Open(ip);
	if(!(*socket)){
		printf("Wystapil blad przy polaczeniu\n");
		free(socket);
		free(ip);
		serverSocket = NULL;
		ip = NULL;
		return -1;
	}
	printf("Polaczono!\n");
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
	if(socket != NULL){
		SDLNet_FreeSocketSet(*socketSet);
		free(socketSet);
		socketSet = NULL;
		SDLNet_TCP_Close(*socket);
		free(socket);
		socket = NULL;
	}
}
bool sendMessage(enum messageType msg){
	char packet = msg;
	int result = SDLNet_TCP_Send(*socket,&packet,sizeof(char));
	if(result < sizeof(char)){
		printf("rozlaczamy\n");
		return false;
	}
	return true;
}
SDLNet_SocketSet* getSocketSet(){
	return socketSet;
}
bool isActiveSocket(){
	if(SDLNet_CheckSockets(*socketSet,0)>0){
		if(SDLNet_SocketReady(*socket)){
			return true;
		}
	}
	return false;
}
