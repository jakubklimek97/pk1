/*
 * mediaList.h
 *
 *  Created on: 30 kwi 2018
 *      Author: klimek
 */

#ifndef MEDIALIST_H_
#define MEDIALIST_H_
#define NULL_TEXTURE MEDIA_COUNT
/*nazwy tekstur */
enum mediaList{
		IMG_HELLOWORD,
		IMG_UP,
		IMG_DOWN,
		IMG_LEFT,
		IMG_RIGHT,
		IMG_MENU_BG,
		IMG_MENU_BTN_DEFAULT,
		IMG_MENU_BTN_MOUSEOVER,
		IMG_SCENE_MENU_BG_STATIC,
		IMG_SCENE_MENU_BTN_SINGLEGAME_DEFAULT,
		IMG_SCENE_MENU_BTN_SINGLEGAME_MOUSEOVER,
		IMG_SCENE_MENU_BTN_MULTIPLAYER_DEFAULT,
		IMG_SCENE_MENU_BTN_MULTIPLAYER_MOUSEOVER,
		IMG_SCENE_MENU_BTN_QUITGAME_DEFAULT,
		IMG_SCENE_MENU_BTN_QUITGAME_MOUSEOVER,
		/*multiplayer scenes */
		IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CLIENT_DEFAULT_MOUSEOVER,
		IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_SERVER_DEFAULT_MOUSEOVER,
		IMG_SCENE_MULTIPLAYER_ENGAGING_BTN_CONNECTING_DEFAULT_MOUSEOVER,
		IMG_SCENE_MULTIPLAYER_ENGAGING_INPUT_BG,
		IMG_SCENE_MULTIPLAYER_ENGAGING_BG_STATIC,
		/*SinglePlayer Scene*/
		IMG_SCENE_SINGLEPLAYER_BG_STATIC,
		IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_DEFAULT,
		IMG_SCENE_SINGLEPLAYER_BTN_BOARD_BLANK_MOUSEOVER,
		IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_DEFAULT,
		IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CIRCLE_MOUSEOVER,
		IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_DEFAULT,
		IMG_SCENE_SINGLEPLAYER_BTN_BOARD_CROSS_MOUSEOVER,
		IMG_SCENE_SINGLEPLAYER_BTN_SAVE_DEFAULT,
		IMG_SCENE_SINGLEPLAYER_BTN_SAVE_MOUSEOVER,
		IMG_SCENE_SINGLEPLAYER_BTN_LOAD_DEFAULT,
		IMG_SCENE_SINGLEPLAYER_BTN_LOAD_MOUSEOVER,
		IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_ACTIVE,
		IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_MOUSEOVER,
		IMG_SCENE_SINGLEPLAYER_BTN_UNDOMOVE_UNACTIVE,
		/*scoreboard scene*/
		IMG_SCENE_SCOREBOARD_BG_STATIC,
		IMG_SCENE_SCOREBOARD_INPUTBOX_BG_STATIC,

		MEDIA_COUNT
};
/*nazwy czcionek */
enum fontList{
		FONT_OPENSANS_BOLD,
		FONT_COUNT
};
/*tutaj przechowywane sa lokalizacje wyzej podanych tekstur/czcionek */
extern char* mediaLocations[];

#endif /* MEDIALIST_H_ */
