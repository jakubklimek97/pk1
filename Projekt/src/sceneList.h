
#ifndef SCENELIST_H_
#define SCENELIST_H_

struct scene;
/*nazwy scen*/
enum sceneList{
	MAIN_MENU,
	SINGLEPLAYER,
	MULTIPLAYER_ENGAGING,
	SCOREBOARD,
	MULTIPLAYER,
	QUIT
};
/*tutaj sa przechowywane adresy scen */
extern struct scene* sceneList[];

#endif
