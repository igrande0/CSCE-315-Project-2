#include "AI.h"
#include "Reversi.h"

/* I believe this is how it works.
It does something like chose the maximum value that will give your opponent a minimum value
*/


//AI public functions

string AI::get_move(Reversi game, int difficulty){
	while(true){
		break;
	}
}

//AI Private functions

Position AI::max_move(Reversi game, char player_color, int depth){
	/*Position position;
	int highest_value = INT_MIN;
	for(unsigned int i=0; i<available_moves.size(); i++){
		int temp_value = min_move(board_if_move_is_applied);
		if(temp_value > highest_value){
			position.row = available_moves[i].row;
			position.column = available_moves[i].column;
			highest_value = temp_value;
		}
	}
	return position;*/
}

Position AI::min_move(Reversi game, char player_color, int depth){
	/*Position position;
	int lowest_value = INT_MAX;
	for(unsigned int i=0; i<available_moves.size(); i++){
		int temp_value = max_move(board_if_move_is_applied);
		if(temp_value > highest_value){
		position.row = available_moves[i].row;
		position.column = available_moves[i].column;
		lowest_value = temp_value;
		}
	}
	return position;*/
}


//Picks the best available move based on weighted tile values
string AI::get_educated_move(Reversi game){
	vector<Position> available_moves = game.get_possible_moves();
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
	vector<Position> available_moves = game.get_possible_moves();
	vector<string> available_move_strings(available_moves.size());
	int best_move_index;
	int best_move_gain = 0;
	char current_player = game.get_current_player();
	
	for(unsigned int i=0; i < available_moves.size(); i++){
		int current_move_gain;
		Reversi game_after_move = game;

		string move_string = "" + game.get_letter_of_number(available_moves[i].column + 1);
		move_string += to_string(available_moves[i].row + 1);
		available_move_strings[i] = move_string;

		game_after_move.make_move(move_string);

		if(current_player == 'w')
			current_move_gain = game_after_move.get_white_score() - game.get_white_score();
		else
			current_move_gain = game_after_move.get_black_score() - game.get_black_score();
		
		if(current_move_gain > best_move_gain){
			best_move_gain = current_move_gain;
			best_move_index = i;
		}
	}
	
	return available_move_strings[best_move_index];
}

