#include <string>
#include <vector>
#include <stack>

using namespace std;

struct State {
	vector<vector<char> > board;
	vector<string> available_moves;
	stack<string> previous_moves;
	void display_state();
	int white_score;
	int black_score;
	char current_player;
};

struct Position {
	unsigned int row;
	unsigned int column;
};

class Reversi{
	public:
		void clear_board();
		void update_score();
		
		/* POSSIBLE FUNCTIONS */
		
		/*	play_game();
			LOOP
			-----SEND STATE THROUGH SOCKET-----
			send board string
			send moves
			send score
			-----GET USER'S MOVE-----
			send state
			read user's move 
			make_move(user's move)
			is_game_over()
			-----MAKE RANDOM MOVE-----
			send state
			make_random_move()
			is_game_over()
			
			END GAME
		*/
		void play_game(int file_descriptor);
		
		void make_random_move(char side);
		void make_move(string move);
		
		bool is_valid_move(string move);
		bool is_game_over();
		
		string get_state();
	private:
		// STATE VARIABLES
		vector<vector<char> > board;
		vector<Position> available_moves;
		stack<Position> previous_moves;
		void display_state();
		int white_score;
		int black_score;
		char current_player;				//holds the players color whose turn it is
		
		// PRIVATE UTILITY FUNCTIONS
		int get_number_of_letter(char c);
		char get_letter_of_number(int number);
		vector<Position> get_available_moves(char side);
		vector<Position> get_open_spaces();
		bool stepping_loop(int x_step, int y_step, int x, int y, char self, char opp);
};
