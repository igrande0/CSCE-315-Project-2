#ifndef AI_H_
#define AI_H_

#include "Reversi.h"

#include <vector>

using namespace std;

//Artificial Intelligence

/* We have to at the minimum implement the MIN-MAX algorithm
http://en.wikipedia.org/wiki/Minimax */

/* Possible Tile Values
  ___ ___ ___ ___ ___ ___ ___ ___ 
1| 99| -8|  8|  6|  6|  8| -8| 99|
2| -8|-24| -4| -3| -3| -4|-24| -8|
3|  8| -4|  7|  4|  4|  7| -4|  8|
4|  6| -3|  4|  0|  0|  4| -3|  6|
5|  6| -3|  4|  0|  0|  4| -3|  6|
6|  8| -4|  7|  4|  4|  7| -4|  8|
7| -8|-24| -4| -3| -3| -4|-24| -8|
8| 99| -8|  8|  6|  6|  8| -8| 99|
   a   b   c   d   e   f   g   h
*/

struct Position;

/*struct char_value{
  char status;
  int value;
};
*/
class AI{
	//Private Data Members
	Reversi inital_game;
	int value_table[8][8]=
	{
		{ 99, -8, 8, 6, 6, 8, -8, 99},
		{ -8, -24, -4, -3, -3, -4, -24, -8},
		{ 8, -4, 7, 4, 4, 7, -4, 8},
		{ 6, -3, 4, 0, 0, 4, -3, 6},
		{ 6, -3, 4, 0, 0, 4, -3, 6},
		{ 8, -4, 7, 4, 4, 7, -4, 8},
		{ -8, -24, -4, -3, -3, -4, -24, -8},
		{ 99, -8, 8, 6, 6, 8, -8, 99}
	};  //Table containing tile values

	//Private Functions
	Position max_move(Reversi game, char player_color, int depth);
	Position min_move(Reversi game, char player_color, int depth);
	
	string get_minimax_move(Reversi game, int depth);
	string get_educated_move(Reversi game);
	string get_greedy_move(Reversi game);
public:
	//Constructor
	AI() {}
	
	//Public Member Functions
	string get_move(Reversi, int);
};

#endif
