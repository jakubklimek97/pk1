/*
 * minmax.c
 *
 *  Created on: 25 maj 2018
 *      Author: klimek
 */

#include "minmax.h"

bool isEmptyPlace(char board[3][3]){
	int row, column;
	for(row = 0; row < 3; ++row){
		for(column = 0; column < 3; ++column){
			if(board[row][column] == '_')
				return true; /*jezeli jakies pole jest puste, zwroc prawde */
		}
	}
	return false;
}
void setupBot(enum signs sign){
	if(sign == X){
		player = X;
		opponent = O;
	}
	else{
		player = O;
		opponent = X;
	}
}
int max(int a, int b){
	return a>b? a:b;
}
int min(int a, int b){
	return a<b ? a : b;
}
enum whoWon lookForWinner(char board[3][3]){
	int row, column;
	for (row = 0; row<3; ++row)/*wiersze*/
	{
	        if (board[row][0]==board[row][1] &&
	            board[row][1]==board[row][2])
	        {
	            if (board[row][0]==player)
	                return PLAYER;
	            else if (board[row][0]==opponent)
	                return OPPONENT;
	        }
	    }
	    for (column = 0; column<3; ++column) /*kolumny*/
	    {
	        if (board[0][column]==board[1][column] &&
	            board[1][column]==board[2][column])
	        {
	            if (board[0][column]==player)
	                return PLAYER;

	            else if (board[0][column]==opponent)
	                return OPPONENT;
	        }
	    }
	    if (board[0][0]==board[1][1] && board[1][1]==board[2][2])/*Przekatne*/
	    {
	        if (board[0][0]==player)
	            return PLAYER;
	        else if (board[0][0]==opponent)
	            return OPPONENT;
	    }
	    if (board[0][2]==board[1][1] && board[1][1]==board[2][0]) /*Przekatne*/
	    {
	        if (board[0][2]==player)
	            return PLAYER;
	        else if (board[0][2]==opponent)
	            return OPPONENT;
	    }
	    return 0;
}
int minMax(char board[3][3], int depth, bool isMaximalising){
		enum whoWon score = lookForWinner(board);
        int i,j;
	    if (score == PLAYER)
	        return score;
	    if (score == OPPONENT)
	        return score;
	    if (isEmptyPlace(board) == false)
	        return 0;
	    if (isMaximalising)
	    {/*Szukam maksymalnej wartosci*/
	        int best = -1000;
	        for (i = 0; i<3; i++) /*Dla kazdego pola planszy*/
	        {
	            for (j = 0; j<3; j++)
	            {
	                if (board[i][j]=='_')/*W pustym miejscu wstawiam znak bota*/
	                {
	                    board[i][j] = player;
	                    /*Wywoluje funkcje na "nowej" planszy, porownuje wynik z poprzednim najwiekszym.
	                     * Jezeli jest wiekszy, przyjmuje jego wartosc */
	                    best = max( best, minMax(board, depth+1, !isMaximalising) );
	                    board[i][j] = '_';
	                }
	            }
	        }
	        return best;
	    }
	    else
	    {/*analogicznie, teraz szukam najmniejszego */
	        int best = 1000;
	        for (i = 0; i<3; i++)
	        {
	            for (j = 0; j<3; j++)
	            {
	                if (board[i][j]=='_')
	                {
	                    board[i][j] = opponent;
	                    best = min(best,
	                           minMax(board, depth+1, !isMaximalising));
	                    board[i][j] = '_';
	                }
	            }
	        }
	        return best;
	    }
}
struct minMax_Move findBestMove(char board[3][3]){
		int bestVal = -1000;
	    struct minMax_Move bestMove;
	    bestMove.row = -1;
	    bestMove.col = -1;
        int i,j;
	    for (i = 0; i<3; i++)
	    {
	        for (j = 0; j<3; j++)
	        {
	            if (board[i][j]=='_')
	            {
	                board[i][j] = player;
	                /*Obliczam wartosc ruchu takiego ruchu*/
	                int moveVal = minMax(board, 0, false);
	                board[i][j] = '_';
	                /*Jezeli ruch ma wieksza wartosc niz poprzednio najlepszy,
	                 * od teraz ten ruch bedzie najlepszy
	                 */
	                if (moveVal > bestVal)
	                {
	                    bestMove.row = i;
	                    bestMove.col = j;
	                    bestVal = moveVal;
	                }
	            }
	        }
	    }
	    return bestMove;
}
