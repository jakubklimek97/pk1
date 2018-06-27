/*
 * main.c
 *
 *  Created on: 26 cze 2018
 *      Author: klimek
 */
#include "utils.h"
int main(int argc, char** argv){
	return checkParameters(argc,argv) == 0 ? run() : -1;
}

