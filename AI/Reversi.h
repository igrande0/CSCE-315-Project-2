#ifndef REVERSI_H_
#define REVERSI_H_

#include <string>
#include <vector>
#include <list>

using namespace std;

struct Position {
	unsigned int row;
	unsigned int column;
};

struct State {
	vector<vector<char> > board;
	vector<Position> available_moves;
	vector<string> available_move_strings;
	int white_score;
	int black_score;
	char current_player;
};

class Reversi{		
public:
	Reversi();

	bool make_move(string move);
	bool make_random_move();
	bool undo();
                
	bool is_game_over();
	void clear_board();

	bool set_current_player(char player);
	void toggle_player();
	void update_state();

	vector<Position> get_available_move_positions() {return available_moves;}
	vector<string> get_available_move_strings() {return available_move_strings;}
	int get_white_score() {return white_score;}
	int get_black_score() {return black_score;}
	int get_num_moves() {return available_moves.size();}
	char get_current_player() {return current_player;}
	int get_number_of_letter(char c);
	char get_letter_of_number(int number);
	string get_previous_move() {return previous_move;}
	string get_state_string();	

private:
	// STATE VARIABLES
	/* internal - board[row][column]
	0|_|_|_|_|_|_|_|_|
	1|_|_|_|_|_|_|_|_|
	2|_|_|_|_|_|_|_|_|
	3|_|_|_|_|_|_|_|_|
	4|_|_|_|_|_|_|_|_|
	5|_|_|_|_|_|_|_|_|
	6|_|_|_|_|_|_|_|_|
	7|_|_|_|_|_|_|_|_|
	  0 1 2 3 4 5 6 7
	*/
	vector<vector<char> > board;
		
	int white_score;
	int black_score;
	char current_player = 'n';				//holds the players color whose turn it is
	string previous_move;
	list<State> previous_states;
	vector<Position> available_moves;
	vector<string> available_move_strings;
		
	// PRIVATE UTILITY FUNCTIONS
	void update_score();
	vector<Position> get_available_moves();
	vector<Position> get_open_spaces();
	bool is_move_valid(Position move);

	bool stepping_loop(int x_step, int y_step, int x, int y, char opp);
	vector<Position> get_tiles(Position start_position, int x_step, int y_step);
};

#endif
