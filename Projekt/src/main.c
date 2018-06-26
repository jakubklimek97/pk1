/*
 * main.c
 *
 *  Created on: 26 cze 2018
 *      Author: klimek
 */
#include "bool.h"
#include "game.h"
#include <string.h>
int checkParameters(int argc, char** argv){
	if(argc > 1){
		char difficultyS[] = "-d";
		char restore[] = "-r";
		bool isDifficulty = false;
		int index;
		for(index = 1; index < argc; ++index){
				if(strcmp(restore,argv[index]) == 0){
					if(isDifficulty == true) isDifficulty = false;
					restoreDefaults();
					printf("Save and score files have been removed\n");
				}
				else if(strcmp(difficultyS,argv[index]) == 0 && index < (argc-1)){
					isDifficulty = true;
				}
				else if(isDifficulty == true){
					isDifficulty = false;
					int difficultyI = atoi(argv[index]);
					if(difficultyI >0 && difficultyI < 5){
						difficulty(difficultyI);
					}
				}else{
					printf("Wrong parameters. Use with:\n-r - to restore default settings\n-d [difficulty from 1 to 4] - to set single player difficulty\n");
					return -1;
				}
		}
	}
	return 0;
}
int main(int argc, char** argv){
	return checkParameters(argc,argv) == 0 ? run() : -1;
}

