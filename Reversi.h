class Reversi{
	private:
		vector<vector<char>> board;
		int white_score;
		int black_score;
	public:
		void display_board();
		void clear_board();
		void update_score();
		vector<int,char> get_open_spaces();
		char get_number_of_letter(int number);
}