#include "AI.h"

/* I believe this is how it works.
It does something like chose the maximum value that will give your opponent a minimum value
*/

//Constructors
AI::AI(){ };

//AI public functions

string AI::make_move(Reversi game){
	while(true){
		break;
	}
}

//AI Private functions

Position AI::max_move(vector<vector<char_value>> BOARD){
  Position position;
  int highest_value = INT_MIN;
  for(unsigned int i=0; i<available_moves.size(); i++){
    int temp_value = min_move(board_if_move_is_applied);
    if(temp_value > highest_value){
      position.row = available_moves[i].row;
      position.column = available_moves[i].column;
      highest_value = temp_value;
    }
  }
  return position;
}

Position AI::min_move(vector<vector<char_value>> BOARD){
	Position position;
	int lowest_value = INT_MAX;
	for(unsigned int i=0; i<available_moves.size(); i++){
		int temp_value = max_move(board_if_move_is_applied);
		if(temp_value > highest_value){
		position.row = available_moves[i].row;
		position.column = available_moves[i].column;
		lowest_value = temp_value;
		}
	}
	return position;
}


//Picks the best available move based on weighted tile values
string AI::make_educated_move(){
	int value = 0;
	Position best_move;
	for(unsigned int i=0; i<available_moves.size(); i++)
		if(value_table[available_moves[i].row][available_moves[i].column] >value){
			value = value_table[available_moves[i].row][available_moves[i].column];
			best_move.row = available_moves[i].row;
			best_move.column = available_moves[i].column;
		}
	string move = get_letter_of_number(best_move.column + 1);
	move += to_string(best_move.row + 1);
	return move;
}


//Greedy Move
//Pretty much done - picks the available move that will give it the most points at current time
string AI::make_greedy_move(){
	Position current_best_move;
	int most_possible_tiles = 0;
	for(unsigned int i=0; i<available_moves.size(); i++){
		int tiles_for_this_move = 0;
		int x_step = 0;
		int y_step = -1;
		tiles_for_this_move += total_in_direction(current_move, x_step, y_step);		//check above

		y_step = 1;
		tiles_for_this_move += total_in_direction(current_move, x_step, y_step);		//check below

		y_step = 0;
		x_step = 1;
		tiles_for_this_move += total_in_direction(current_move, x_step, y_step);		//check right

		x_step = -1;
		tiles_for_this_move += total_in_direction(current_move, x_step, y_step);		//check left

		y_step = -1;
		tiles_for_this_move += total_in_direction(current_move, x_step, y_step);		//check top left

		x_step = 1;
		tiles_for_this_move += total_in_direction(current_move, x_step, y_step);		//check top right

		y_step = 1;
		tiles_for_this_move += total_in_direction(current_move, x_step, y_step);		//check bottom right

		x_step = -1;
		tiles_for_this_move += total_in_direction(current_move, x_step, y_step);		//check bottom left

		if(tiles_for_this_move > most_possible_tiles){
			most_possible_tiles = tiles_for_this_move;
			current_best_position.row = available_moves[i].row;
			current_best_position.column = available_moves[i].column;
		}
	}
	string move_string = get_letter_of_number(current_best_position.column + 1);
	move_string += to_string(current_best_position.row + 1);
	
	return move_string;
}

int AI::total_in_direction(Position start_position, int x_step, int y_step){
	char opp;
	if(current_player == 'w')
		opp = 'b';
	else
		opp = 'w';
	int x = start_position.row;
	int y = start_position.column;
	
	Position current_position;
	current_position.row = x + x_step;
	current_position.column = y + y_step;
	
	int total_number_of_tiles = 0;
	bool self_check = false;
	while(current_position.row < 8 && current_position.column < 8  && current_position.row >= 0 && current_position.column >= 0 && board[current_position.row][current_position.column] != 'o'){
		if(board[current_position.row][current_position.column] == opp)
			total_number_of_tiles++;
		else if(board[current_position.row][current_position.column] == current_player){
			self_check = true;
			break;
		}
		current_position.row+=x_step;
		current_position.column+=y_step;
	}
	if(!self_check)
		total_number_of_tiles = 0;
	return total_number_of_tiles;
}

