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

