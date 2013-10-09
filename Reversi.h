class Reversi{
	public:
		void clear_board();
		void update_score();
		vector<int, char> get_open_spaces();
		
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
		vector<vector<char>> board;
		vector<int, char> available_moves;
		stack<int, char> previous_moves;
		void display_state();
		int white_score;
		int black_score;
		char current_player;				//holds the players color whose turn it is
		
		// PRIVATE UTILITY FUNCTIONS
		char get_number_of_letter(int number);
		int get_letter_of_number(char c);
		vector<int, char> get_available_moves();
		bool stepping_loop(int x_step, int y_step, int x, int y, char self, char opp);
}