#include "Reversi.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
//#include <stdiolib.h>

string Reversi::get_state_string(){
	string STRING;
	STRING +="_ _ _ _ _ _ _ _\n";							//displays board and current score
	for(unsigned int i=0; i<8; i++){
		STRING += i + "|";
		for(unsigned int j=0; j<8; j++){
			if(board[i][j] == 'w')
				STRING += "O|";
			else if(board[i][j] == 'b')
				STRING += "@|";
			else
				STRING += "_|";
		}
		STRING += "\n";
	}
	STRING += "a b c d e f g h\n";
	STRING += "White Score: ";
	STRING += white_score;
	STRING += "    Black Score: ";
	STRING += black_score + "\n";

	return STRING;
}

void Reversi::clear_board(){							//sets board to starting state of game
	for(unsigned int i=0; i<8; i++)
		for(unsigned int j=0; j<8; j++)
			board[i][j] = 'o';
	board[3][3] = 'w';
	board[3][4] = 'b';
	board[4][3] = 'b';
	board[4][4] = 'w';
	update_score();									//updates score
}


void Reversi::update_score(){
	int white_count, black_count = 0;
	for(unsigned int i=0; i<8; i++)
		for(unsigned int j=0; j<8; j++){
			if(board[i][j] == 'w')
				white_count++;
			else if(board[i][j] == 'b')
				black_count++;
		}			
	white_score = white_count;
	black_score = black_count;
}

bool Reversi::make_move(string move){
	//check size of move
	if(sizeof(move) != 2)
		return false;
	//split move into correct data types
	stringstream s;
	s.str(move);
	int x = -1;
	s >> x;
	char c = 'x';
	x << c;
	// check if valid move
	Position current_move;
	current_move.row = x;
	current_move.column = get_number_of_letter(c);

	bool possible_move_check = false;
	for(unsigned int i=0; i< available_moves.size(); i++)
		if(available_moves[i].row == current_move.row && available_moves[i].column == current_move.column)
			possible_move_check = true;

	if(!possible_move_check)
		return false;

	//check all directions
	//if valid in a direction flip all appropriate tiles
	vector<Position> all_positions;
	vector<Position> temp_positions;
	int x_step = 0;
	int y_step = -1;
	temp_positions = get_tiles(current_move, x_step, y_step);		//check above
	for(unsigned int i=0; i<temp_positions.size(); i++)
		all_positions.push_back(temp_positions[i]);
	y_step = 1;
	temp_positions = get_tiles(current_move, x_step, y_step);		//check below
	for(unsigned int i=0; i<temp_positions.size(); i++)
		all_positions.push_back(temp_positions[i]);
	y_step = 0;
	x_step = 1;
	temp_positions = get_tiles(current_move, x_step, y_step);		//check right
	for(unsigned int i=0; i<temp_positions.size(); i++)
		all_positions.push_back(temp_positions[i]);
	x_step = -1;
	temp_positions = get_tiles(current_move, x_step, y_step);		//check left
	for(unsigned int i=0; i<temp_positions.size(); i++)
		all_positions.push_back(temp_positions[i]);
	y_step = -1;
	temp_positions = get_tiles(current_move, x_step, y_step);		//check top left
	for(unsigned int i=0; i<temp_positions.size(); i++)
		all_positions.push_back(temp_positions[i]);
	x_step = 1;
	temp_positions = get_tiles(current_move, x_step, y_step);		//check top right
	for(unsigned int i=0; i<temp_positions.size(); i++)
		all_positions.push_back(temp_positions[i]);
	y_step = 1;
	temp_positions = get_tiles(current_move, x_step, y_step);		//check bottom right
	for(unsigned int i=0; i<temp_positions.size(); i++)
		all_positions.push_back(temp_positions[i]);
	x_step = -1;
	temp_positions = get_tiles(current_move, x_step, y_step);		//check bottom left
	for(unsigned int i=0; i<temp_positions.size(); i++)
		all_positions.push_back(temp_positions[i]);
	for(unsigned int i=0; i<all_positions.size(); i++)
		board[all_positions[i].row][all_positions[i].column] = current_player;
	update_score();
	if(current_player == 'w')
		current_player = 'b';
	else
		current_player = 'w';
	available_moves = get_available_moves();
}
vector<Position> Reversi::get_tiles(Position start_position, int x_step, int y_step){
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

	vector<Position> position_switches;
	bool self_check = false;
	while(current_position.row < 8 && current_position.column < 8 && board[current_position.row][current_position.column] != 'o' && current_position.row >= 0 && current_position.column >= 0){
		if(board[current_position.row][current_position.column] == opp)
			position_switches.push_back(current_position);
		else if(board[current_position.row][current_position.column] == current_player){
			self_check = true;
			break;
		}
		current_position.row+=x_step;
		current_position.column+=y_step;
	}
	if(!self_check)
		position_switches.clear();
	return position_switches;
}

bool Reversi::make_random_move(){
	int random_index = rand() % (available_moves.size()-1);
	string s = get_letter_of_number(available_moves[random_index].column) + to_string(available_moves[random_index].row);
	make_move(s);
	//turn moves[random_index] into a string
	//make_move(string moves[random_index])
}


bool Reversi::undo(){
	State last_state;

	if(previous_states.size() < 2)
		return false;

	// remove other player's move
	previous_states.pop();

	// revert to state before last move
	last_state = previous_states.top();
	board = last_state.board;
	available_moves = last_state.available_moves;
	white_score = last_state.white_score;
	black_score = last_state.black_score;
	current_player = last_state.current_player;
}

vector<Position> Reversi::get_available_moves(){
	char opponent;
	if(current_player == 'w')				//determines oppenents color by given argument
		opponent = 'b';
	else if(current_player == 'b')
		opponent = 'w';
	vector<Position> temp_vec;
	//needs to be finished
	vector<Position> open_spaces = get_open_spaces();				//need to check every direction for every space
	for(unsigned int i=0; i<open_spaces.size(); i++){
		//check if board[i][j] is a possible move
		int temp_x = i;		//set to x coord
		int temp_y;// = j;		//set to y coord
		int x_step = 0;
		int y_step = -1;
		bool check_push_back = false;

		//check above
		if(stepping_loop(x_step, y_step, temp_x, temp_y, opponent)){		
			temp_vec.push_back(open_spaces[i]);
			check_push_back = true;
		}

		//check below
		y_step = 1;
		if(stepping_loop(x_step, y_step, temp_x, temp_y, opponent) && !check_push_back){		
			temp_vec.push_back(open_spaces[i]);
			check_push_back = true;
		}

		//check right
		y_step = 0;
		x_step = 1;
		if(stepping_loop(x_step, y_step, temp_x, temp_y, opponent) && !check_push_back){		
			temp_vec.push_back(open_spaces[i]);
			check_push_back = true;
		}

		//check left
		x_step = -1;
		if(stepping_loop(x_step, y_step, temp_x, temp_y, opponent) && !check_push_back){		
			temp_vec.push_back(open_spaces[i]);
			check_push_back = true;
		}
		y_step = -1;

		//check top left
		if(stepping_loop(x_step, y_step, temp_x, temp_y, opponent) && !check_push_back){		
			temp_vec.push_back(open_spaces[i]);
			check_push_back = true;
		}

		//check top right
		x_step = 1;
		if(stepping_loop(x_step, y_step, temp_x, temp_y, opponent) && !check_push_back){		
			temp_vec.push_back(open_spaces[i]);
			check_push_back = true;
		}

		//check bottom right
		y_step = 1;
		if(stepping_loop(x_step, y_step, temp_x, temp_y, opponent) && !check_push_back){		
			temp_vec.push_back(open_spaces[i]);
			check_push_back = true;
		}

		//check bottom left
		x_step = -1;
		if(stepping_loop(x_step, y_step, temp_x, temp_y, opponent) && !check_push_back){		
			temp_vec.push_back(open_spaces[i]);
			check_push_back = true;
		}
	}
	return temp_vec;
}

bool Reversi::stepping_loop(int x_step, int y_step, int x, int y, char opp){
	x+=x_step;
	y+=y_step;
	bool mid_check=false;
	while(x < 8 && y < 8 && board[x][y] != 'o' && x >= 0 && y >= 0){
		if(board[x][y] == opp)
			mid_check=true;
		else if(board[x][y] == current_player && mid_check == true)
			return true;
		x+=x_step;
		y+=y_step;
	}
	return false;
}

bool Reversi::is_move_valid(Position move){
	for(unsigned int i=0; i<available_moves.size(); i++)
		if(move.row == available_moves[i].row && move.column == available_moves[i].column)
			return true;
	return false;
}

bool Reversi::is_game_over(){
	vector<Position> open_spaces = get_open_spaces();
	if(open_spaces.size() == 0)
		return true;
	else
		return false;
}

vector<Position> Reversi::get_open_spaces(){
	vector<Position> open_spaces;		//creates temp vector to hold open spaces
	for(unsigned int i=0; i<8; i++)
		for(unsigned int j=0; j<8; j++)
			if(board[i][j] == 'o')									//if the space is clear then push the integer and char of space
				open_spaces.push_back({i+1,j+1});		
	return open_spaces;												//return temp vector containing all open spaces
}

char Reversi::get_letter_of_number(int number){
	switch (number){
		case 1:
			return 'a';
			break;
		case 2:
			return 'b';
			break;
		case 3:
			return 'c';
			break;
		case 4:
			return 'd';
			break;
		case 5:
			return 'e';
			break;
		case 6:
			return 'f';
			break;
		case 7:
			return 'g';
			break;
		case 8:
			return 'h';
			break;
		default:
			return 'x';
			break;
	}
}

int Reversi::get_number_of_letter(char c){
	switch (c){
		case 'a':
			return 0;
			break;
		case 'b':
			return 1;
			break;
		case 'c':
			return 2;
			break;
		case 'd':
			return 3;
			break;
		case 'e':
			return 4;
			break;
		case 'f':
			return 5;
			break;
		case 'g':
			return 6;
			break;
		case 'h':
			return 7;
			break;
		default:
			return -1;
			break;
	}
}