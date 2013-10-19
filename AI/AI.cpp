#include "AI.h"
#include "Reversi.h"

#include <climits>
#include <algorithm>

/* I believe this is how it works.
It does something like chose the maximum value that will give your opponent a minimum value
*/


//AI public functions

string AI::get_move(Reversi game, Difficulty d){
	string move;

	switch(d){
	case EASY:
		move = get_greedy_move(game);
		break;
	case MEDIUM:
		move = get_educated_move(game);
		break;
	case HARD:
		move = nega_max(game, 7, INT_MIN, INT_MAX, 1).move;
		break;
	case RANDOM:
		break;
	}

	return move;
}

//AI Private functions

/* Reference: http://en.wikipedia.org/wiki/Negamax
 * NegaMax is a variation of minimax that uses less recursion
 * For the maximizing player, color is 1; for the minimizing player, color is -1
 */
AI::NegaReturn AI::nega_max(Reversi game, int depth, int alpha, int beta, int color) {
	// reached end of tree; return heuristic value of node
	if(depth == 0 || game.is_game_over())
		return {"", evaluate(game)};

	int best_value = INT_MIN;
	string best_move;
	vector<string> available_moves = game.get_available_move_strings();

	for(unsigned int i = 0; i < available_moves.size(); ++i) {
		int current_value;
		Reversi new_game = game;
		new_game.make_move(available_moves[i]);
		
		// consider whether next player is opponent or current player
		if(new_game.get_current_player() != game.get_current_player())
			current_value = -(nega_max(new_game, depth-1, -alpha, -beta, -color)).value;
		else
			current_value = (nega_max(new_game, depth-1, alpha, beta, color)).value;

		if(current_value > best_value) {
			best_value = current_value;
			best_move = available_moves[i];
		}

		if(current_value > alpha)
			alpha = current_value;

		if(alpha > beta)
			break;
	}

	return {best_move, best_value};
}

/* The evaluation function for a current game state
 * Returns the current state's heuristic value
 * -> If the game is over, this needs to be higher than any other scenario
	  but still equally relative to other end game scenarios.
	  We don't need to worry about who the maximizing player is, because that is dealt with in nega_max().
	  EXAMPLE: (current player's score - opponent's score) * (some very large number)
 * -> If the game isn't over, this should be an intelligent algorithm that includes multiple factors such as:
		- Tile Parity
		- Mobility
		- Corners Captured
		- Stability
	  Possible references for heuristic function: 
	    - http://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/
	    - http://home.datacomm.ch/t_wolf/tw/misc/reversi/html/index.html
		- Google
 */
int AI::evaluate(Reversi game) {

}

//Picks the best available move based on weighted tile values
string AI::get_educated_move(Reversi game){
	vector<Position> available_moves = game.get_available_move_positions();
	int value = 0;
	Position best_move;
	for(unsigned int i=0; i < available_moves.size(); i++)
		if(value_table[available_moves[i].row][available_moves[i].column] >value){
			value = value_table[available_moves[i].row][available_moves[i].column];
			best_move.row = available_moves[i].row;
			best_move.column = available_moves[i].column;
		}
	string move = "" + game.get_letter_of_number(best_move.column + 1);
	move += to_string(best_move.row + 1);
	return move;
}


//Greedy Move
//This may eventually be unnecessary. We might instead call minimax with depth 0
string AI::get_greedy_move(Reversi game){
	vector<string> available_move_strings = game.get_available_move_strings();
	int best_move_index;
	int best_move_gain = 0;
	string best_move;
	char current_player = game.get_current_player();
	
	for(unsigned int i=0; i < available_move_strings.size(); i++){
		int current_move_gain;
		Reversi game_after_move = game;
		game_after_move.make_move(available_move_strings[i]);

		if(current_player == 'w')
			current_move_gain = game_after_move.get_white_score() - game.get_white_score();
		else
			current_move_gain = game_after_move.get_black_score() - game.get_black_score();
		
		if(current_move_gain > best_move_gain){
			best_move_gain = current_move_gain;
			best_move = available_move_strings[i];
		}
	}
	
	return best_move;
}

