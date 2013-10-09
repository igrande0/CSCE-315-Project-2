#include <string>
#include <vector>
#include <stack>

using namespace std;


struct Position {
	unsigned int row;
	unsigned int column;
};

struct State {
	vector<vector<char> > board;
	vector<Position> available_moves;
	int white_score;
	int black_score;
	char current_player; 
};

class Reversi{
	public:
		void clear_board();
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
		void play_game(int file_descriptor);				//ignore for now
		
		bool make_random_move();							//done
		bool make_move(string move);						//needs to be completed
		
		bool undo();
                
		bool is_valid_move(Position move);					//done
		bool is_game_over();								//done
		
		string get_state_string();							//needs to be changed into a concatenated string					
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
		vector<Position> available_moves;
		int white_score;
		int black_score;
		char current_player;				//holds the players color whose turn it is
		stack<State> previous_states;
		
		// PRIVATE UTILITY FUNCTIONS
		int get_number_of_letter(char c);			//done
		char get_letter_of_number(int number);		//done
		vector<Position> get_available_moves(char side);		//done
		vector<Position> get_open_spaces();			//done
		bool stepping_loop(int x_step, int y_step, int x, int y, char self, char opp);		//done
		void update_score();															//done
};
