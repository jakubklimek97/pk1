/*
 * utils.c
 *
 *  Created on: 27 cze 2018
 *      Author: klimek
 */


#include "utils.h"
#include <string.h>
/*Funkcja sprawdza parametry i reaguje na nie. Parametry:
 * argc - ilosc parametrow
 * argv - tablica lancuchow znakowych (tablica parametrow)
 *  */
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
