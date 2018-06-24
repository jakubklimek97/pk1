
#ifndef SCENELIST_H_
#define SCENELIST_H_

struct scene;
enum sceneList{
	MAIN_MENU,
	TEST,
	SINGLEPLAYER,
	MULTIPLAYER_ENGAGING,
	QUIT
};
extern struct scene* sceneList[];

#endif
