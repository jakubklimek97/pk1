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
				return true;
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
	// Checking for Rows for X or O victory.
	    for (int row = 0; row<3; ++row)
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

	    // Checking for Columns for X or O victory.
	    for (int column = 0; column<3; ++column)
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

	    // Checking for Diagonals for X or O victory.
	    if (board[0][0]==board[1][1] && board[1][1]==board[2][2])
	    {
	        if (board[0][0]==player)
	            return PLAYER;
	        else if (board[0][0]==opponent)
	            return OPPONENT;
	    }

	    if (board[0][2]==board[1][1] && board[1][1]==board[2][0])
	    {
	        if (board[0][2]==player)
	            return PLAYER;
	        else if (board[0][2]==opponent)
	            return OPPONENT;
	    }

	    // Else if none of them have won then return 0
	    return 0;
}
int minMax(char board[3][3], int depth, bool isMaximalising){
		enum whoWon score = lookForWinner(board);

	    // If Maximizer has won the game return his/her
	    // evaluated score
	    if (score == PLAYER)
	        return score;

	    // If Minimizer has won the game return his/her
	    // evaluated score
	    if (score == OPPONENT)
	        return score;

	    // If there are no more moves and no winner then
	    // it is a tie
	    if (isEmptyPlace(board) == false)
	        return 0;

	    // If this maximizer's move
	    if (isMaximalising)
	    {
	        int best = -1000;

	        // Traverse all cells
	        for (int i = 0; i<3; i++)
	        {
	            for (int j = 0; j<3; j++)
	            {
	                // Check if cell is empty
	                if (board[i][j]=='_')
	                {
	                    // Make the move
	                    board[i][j] = player;

	                    // Call minimax recursively and choose
	                    // the maximum value
	                    best = max( best,
	                        minMax(board, depth+1, !isMaximalising) );

	                    // Undo the move
	                    board[i][j] = '_';
	                }
	            }
	        }
	        return best;
	    }

	    // If this minimizer's move
	    else
	    {
	        int best = 1000;

	        // Traverse all cells
	        for (int i = 0; i<3; i++)
	        {
	            for (int j = 0; j<3; j++)
	            {
	                // Check if cell is empty
	                if (board[i][j]=='_')
	                {
	                    // Make the move
	                    board[i][j] = opponent;

	                    // Call minimax recursively and choose
	                    // the minimum value
	                    best = min(best,
	                           minMax(board, depth+1, !isMaximalising));

	                    // Undo the move
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

	    // Traverse all cells, evalutae minimax function for
	    // all empty cells. And return the cell with optimal
	    // value.
	    for (int i = 0; i<3; i++)
	    {
	        for (int j = 0; j<3; j++)
	        {
	            // Check if celll is empty
	            if (board[i][j]=='_')
	            {
	                // Make the move
	                board[i][j] = player;

	                // compute evaluation function for this
	                // move.
	                int moveVal = minMax(board, 0, false);

	                // Undo the move
	                board[i][j] = '_';

	                // If the value of the current move is
	                // more than the best value, then update
	                // best/
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
