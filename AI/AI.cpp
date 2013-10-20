#include "AI.h"
#include "Reversi.h"

#include <cfloat>
#include <algorithm>

/*------------------------------------------------------------------------------------*/
/* PUBLIC FUNCTION */
/*------------------------------------------------------------------------------------*/

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
		move = nega_max(game, 5, DBL_MIN, DBL_MAX, 1).move;
		break;
	case RANDOM:
		break;
	}

	return move;
}

/*------------------------------------------------------------------------------------*/
/* AI ALGORITHMS */
/*------------------------------------------------------------------------------------*/

/* Reference: http://en.wikipedia.org/wiki/Negamax
 * NegaMax is a variation of minimax that uses less recursion
 * For the maximizing player, color is 1; for the minimizing player, color is -1
 */
AI::NegaReturn AI::nega_max(Reversi game, int depth, double alpha, double beta, double color) {
	// reached end of tree; return heuristic value of node
	if(depth == 0 || game.is_game_over())
		return {"", evaluate(game)};

	double best_value = DBL_MIN;
	string best_move;
	vector<string> available_moves = game.get_available_move_strings();

	for(unsigned int i = 0; i < available_moves.size(); ++i) {
		double current_value;
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

/*------------------------------------------------------------------------------------*/
/* HEURISTIC FUNCTIONS */
/*------------------------------------------------------------------------------------*/

/* Returns the heuristic value of a given game state
 * References: http://goo.gl/UA2uXu
			   http://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/
 * Weights for components were taken from the research in reference.
 * We assume that the maximizing player is the current player; negative weights are
 * dealt with in the nega_max() function.
 * All functions return a value weighted between -100 and 100.
 */
double AI::evaluate(Reversi game) {
	return corners(game)*30 + mobility(game)*5 + stability(game)*25 + parity(game)*25;
}

/* Captures the difference in points between the maximizing and minimizing player.
 */
double AI::parity(Reversi game) {
	char current_player = game.get_current_player();
	int max_player_tiles;
	int min_player_tiles;

	if(current_player = 'w') {
		max_player_tiles = game.get_white_score();
		min_player_tiles = game.get_black_score();
	}
	else {
		max_player_tiles = game.get_black_score();
		min_player_tiles = game.get_white_score();
	}

	return 100*(max_player_tiles - min_player_tiles)/(max_player_tiles + min_player_tiles);
}

/* Shows how much the state minimizes the opponent's mobility and maximizes self mobility.
 ******************************POSSIBLY INCOMPLETE***************************************
 */
double AI::mobility(Reversi game) {
	int max_player_mobility;
	int min_player_mobility;
	Reversi new_game = game;

	max_player_mobility = game.get_available_move_strings().size();

	new_game.toggle_player();
	new_game.update_state();
	
	min_player_mobility = new_game.get_available_move_strings().size();

	// may also need to somehow consider potential mobility
	// calculated by counting the number of empty spaces next to at least one of the opponent's tile
	// the potential mobility and actual mobility will have to somehow be weighted

	return 100*(max_player_mobility - min_player_mobility)/(max_player_mobility + min_player_mobility);
}

/* Returns a value that is weighted by corners captured, potential corners, and unlikely corners
 * for now, don't know weights (the reference does not list any weight values)
 * a guess:
 *   captured = HIGHEST NUMBER; potential = LOWER THAN CAPTURED; unlikely = NEGATIVE
 * 
 * corners captured: self-explanatory
 * potential corners: a corner that could be caught in the next move
 * unlikely corners: corners that cannot be captured in the near future
  ***********************************INCOMPLETE********************************************
 */
double AI::corners(Reversi game) {
	int max_player_corner = 0;
	int min_player_corner = 0;

	// corners captured
	// potential corners
	// unlikely corners

	// max_player_corner = max_captured*weight + max_potential*weight + max_unlikely*weight
	// min_player_corner = min_captured*weight + min_potential*weight + min_unlikely*weight

	return 100*(max_player_corner - min_player_corner)/(max_player_corner + min_player_corner);
}

/* Returns the overall stability of a current state.
 * tile weights: stable = 1, semi-stable = 0, unstable = -1
 *
 * stable tile: coins which cannot be flanked at any piont of time in the game from the given state
 * unstable tile: coins that could be flanked in the very next move
 * semi-stable tile: coins that could potentially be flanked in the future, but not in the next move
 ****************************************INCOMPLETE********************************************
 */
double AI::stability(Reversi game) {
	int max_player_stability = 0;
	int min_player_stability = 0;

	// STABILITY CODE

	return 100*(max_player_stability - min_player_stability)/(max_player_stability + min_player_stability);
}
