#ifndef AI_H_
#define AI_H_

#include "Reversi.h"

#include <vector>
#include <set>

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

class AI{
public:
	// Public Enums
	enum Difficulty {
		EASY, MEDIUM, HARD, RANDOM
	};

	// Constructor
	AI() { }
	
	// Public Member Function
	string get_move(Reversi game, Difficulty d);
private:
	// Private Data Members
	struct NegaReturn{
		string move;
		double value;
	};
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

	// AI algorithms
	NegaReturn nega_max(Reversi game, int depth, double alpha, double beta, double color);
	string get_educated_move(Reversi game);
	string get_greedy_move(Reversi game);
	
	// Heuristic Functions
	double evaluate(Reversi game);
	double parity(Reversi game);
	double mobility(Reversi game);
	double corners(Reversi game);
	double stability(Reversi game);

	bool is_corner(Position p);
	bool check_direction(Position current_position, Reversi game, int x_step, int y_step);
	vector<Position> get_unstable_tiles(Reversi game);
	vector<Position> get_move_tiles(Position start, int x_step, int y_step, Reversi game);
};

#endif
