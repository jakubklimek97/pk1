/*
 * minmax.h
 *
 *  Created on: 25 maj 2018
 *      Author: klimek
 */

#ifndef MINMAX_H_
#define MINMAX_H_
#include "bool.h"
enum whoWon{
	OPPONENT = -10,
	NONE = 0,
	PLAYER = 10
};
enum signs {
	X = 'x',
	O = 'o'
};

struct minMax_Move{
	int row, col;
};
char player;
char opponent;
char prevBoard[3][3];
char board[3][3];
bool isEmptyPlace(char board[3][3]);
enum whoWon lookForWinner(char board[3][3]);
int minMax(char board[3][3], int depth, bool isMaximalising);
struct minMax_Move findBestMove(char board[3][3]);
void setupBot(enum signs sign);
#endif /* MINMAX_H_ */
