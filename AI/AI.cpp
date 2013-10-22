#include "AI.h"
#include "Reversi.h"

#include <cfloat>
#include <algorithm>
#include <iostream>

/*------------------------------------------------------------------------------------*/
/* DEFINES -- Most Heiuristic Values */
/*------------------------------------------------------------------------------------*/

#define CAPTURED_CORNER_WEIGHT 99
#define POTENTIAL_CORNER_WEIGHT 33
#define UNLIKELY_CORNER_WEIGHT 1

/*------------------------------------------------------------------------------------*/
/* PUBLIC FUNCTION */
/*------------------------------------------------------------------------------------*/

string AI::get_move(Reversi game, Difficulty d){
	string move;

	switch(d){
	case EASY:
		move = get_educated_move(game);
		break;
	case MEDIUM:
		move = get_greedy_move(game);
		break;
	case HARD:
		move = nega_max(game, 3, -DBL_MAX, DBL_MAX, 1).move;
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
		return {"", color*evaluate(game)};

	double best_value = -DBL_MAX;

	string best_move;
	vector<string> available_moves = game.get_available_move_strings();

	for(unsigned int i = 0; i < available_moves.size(); ++i) {
		double current_value;
		Reversi new_game = game;
		new_game.make_move(available_moves[i]);
		
		// consider whether next player is opponent or current player
		if(new_game.get_current_player() != game.get_current_player())
			current_value = -(nega_max(new_game, depth-1, -beta, -alpha, -color)).value;
		else
			current_value = (nega_max(new_game, depth-1, alpha, beta, color)).value;
		/*
		if(depth == 2) {
				cout << "current value is: " << current_value << ", " << best_move << endl;
				cout << "best value is: " << best_value << endl;
		}
		*/

		if(current_value > best_value) {
			best_value = current_value;
			best_move = available_moves[i];
			/*if(depth == 2)
				cout << "replaced best value." << endl;*/
		}

		if(current_value > alpha)
			alpha = current_value;

		/*if(depth == 2)
				cout << "alpha: " << alpha << ", beta: " << beta << endl;*/
		if(alpha > beta) {
			break;
		}
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
	string move;
	move += game.get_letter_of_number(best_move.column + 1);
	move += to_string(best_move.row + 1);
	return move;
}


string AI::get_greedy_move(Reversi game){
	vector<string> available_move_strings = game.get_available_move_strings();
	/*for(int i=0; i<available_move_strings.size(); i++){
		std::cout << available_move_strings[i] << '\n';
	}*/
	int best_move_index;
	int best_move_gain = 0;
	int current_move_gain;
	string best_move;
	char current_player = game.get_current_player();
	
	for(unsigned int i=0; i < available_move_strings.size(); i++){
		Reversi game_after_move = game;
		game_after_move.make_move(available_move_strings[i]);

		if(current_player == 'w')
			current_move_gain = game_after_move.get_white_score() - game.get_white_score();
		else
			current_move_gain = game_after_move.get_black_score() - game.get_black_score();
		//std::cout << "Current move: " << available_move_strings[i] << " Gain: " << current_move_gain << '\n';
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

	if(max_player_mobility + min_player_mobility == 0)
		return 0;
	else
		return 100*(max_player_mobility - min_player_mobility)/(max_player_mobility + min_player_mobility);
}

/* Returns a value that is weighted by corners captured, potential corners, and unlikely corners
 * for now, don't know weights (the reference does not list any weight values)
 * a guess:
 *   captured = HIGHEST NUMBER; potential = LOWER THAN CAPTURED; unlikely = 0?
 * 
 * corners captured: self-explanatory
 * potential corners: a corner that could be caught in the next move
 * unlikely corners: corners that cannot be captured in the near future
 */
double AI::corners(Reversi game) {
	int max_player_corner = 0;
	int min_player_corner = 0;
	int max_captured = 0;
	int min_captured = 0;
	int max_potential = 0;
	int min_potential = 0;
	int max_unlikely = 0;
	int min_unlikely = 0;
	vector<Position> open_space_max = game.get_open_spaces();
	vector<Position> available_moves_max = game.get_available_move_positions();
	// potential corners
	for(int i = 0; i < available_moves_max.size(); i++ ){
		if(available_moves_max[i].row == (0 || 7) ){
			if(available_moves_max[i].column == (0 || 7))
				max_potential++;
		}
	}
	// unlikely corners
	int max_open = 0;
	for(int k = 0; k < open_space_max.size(); k++ ){
		if(open_space_max[k].row == (0 || 7) ){
			if(open_space_max[k].column == (0 || 7))
				max_open++;
		}
	}
	max_unlikely = max_open - max_potential;

	//opponent 
	Reversi new_game = game;
	new_game.toggle_player();
	new_game.update_state();
	vector<Position> open_space_min = game.get_open_spaces();
	vector<Position> available_moves_min = new_game.get_available_move_positions();
	// potential corners
	for(int i = 0; i < available_moves_min.size(); i++ ){
		if(available_moves_min[i].row == (0 || 7) ){
			if(available_moves_min[i].column == (0 || 7))
				min_potential++;
		}
	}
	// unlikely corners
	int min_open = 0;
	for(int k = 0; k < open_space_min.size(); k++ ){
		if(open_space_min[k].row == (0 || 7) ){
			if(open_space_min[k].column == (0 || 7))
				min_open++;
		}
	}
	min_unlikely = min_open - min_potential; 
	
	vector<vector<char>> board = game.get_board();
	char max_player = game.get_current_player();
	char min_player = new_game.get_current_player();

	if(board[0][0] == max_player)
		max_captured++;
	if(board[0][7] == max_player)
		max_captured++;
	if(board[7][0] == max_player)
		max_captured++;
	if(board[7][7] == max_player)
		max_captured++;

	if(board[0][0] == min_player)
		min_captured++;
	if(board[0][7] == min_player)
		min_captured++;
	if(board[7][0] == min_player)
		min_captured++;
	if(board[7][7] == min_player)
		min_captured++;

	//cout << "max_captured= " << max_captured << " max_potential= " << max_potential << " max_unlikely= " << max_unlikely << '\n';
	//cout << "min_captured= " << min_captured << " min_potential= " << min_potential << " min_unlikely= " << min_unlikely << '\n';
	max_player_corner = max_captured*CAPTURED_CORNER_WEIGHT + max_potential*POTENTIAL_CORNER_WEIGHT + max_unlikely*UNLIKELY_CORNER_WEIGHT;
	min_player_corner = min_captured*CAPTURED_CORNER_WEIGHT + min_potential*POTENTIAL_CORNER_WEIGHT + min_unlikely*UNLIKELY_CORNER_WEIGHT;

	if(max_player_corner + min_player_corner == 0)
		return 0;
	else
		return 100*(max_player_corner - min_player_corner)/(max_player_corner + min_player_corner);
}

/* Returns the overall stability of a current state.
 * tile weights: stable = 1, semi-stable = 0, unstable = -1
 *
 * stable tile: coins which cannot be flanked at any piont of time in the game from the given state
 * unstable tile: coins that could be flanked in the very next move
 * semi-stable tile: coins that could potentially be flanked in the future, but not in the next move
 */
double AI::stability(Reversi game) {
	int max_player_stability = 0;
	int min_player_stability = 0;
	int max_player_unstable;
	int min_player_unstable;
	int max_player_stable;
	int min_player_stable;
	Reversi new_game = game;
	new_game.toggle_player();
	new_game.update_state();
	
	vector<Position> min_unstable = get_unstable_tiles(game);
	vector<Position> max_unstable = get_unstable_tiles(new_game);

	max_player_unstable = max_unstable.size();
	min_player_unstable = min_unstable.size();

	min_player_stable = get_num_stable_tiles(game);
	max_player_stable = get_num_stable_tiles(new_game);

	max_player_stability = max_player_stable - max_player_unstable;
	min_player_stability = min_player_stable - min_player_unstable;

	if(max_player_stability + min_player_stability == 0)
		return 0;
	else
		return 100*(max_player_stability - min_player_stability)/(max_player_stability + min_player_stability);
}

int AI::get_num_stable_tiles(Reversi game) {
	vector<vector<char>> current_board = game.get_board();
	Position p;
	vector<Position> closed_spaces;

	double number = 0;

	for(unsigned int i=0; i<8; i++){
		for(unsigned int j=0; j<8; j++){
			if(current_board[i][j] == game.get_current_player()){
				p.row = i;
				p.column = j;
				closed_spaces.push_back(p);
			}	
		}
	}
	bool check = false;
	for(unsigned int i=0; i<closed_spaces.size(); i++){
		bool check = false;
		int x_step, y_step;

		if(is_corner(closed_spaces[i]))			//if tile is corner then it is always stable
			check == true;
		else if(closed_spaces[i].row == 0 || closed_spaces[i].row == 7){			//if on top or bottom edge we only need to check to see if the entire edge is filled
			x_step = 0;
			y_step = -1;
			check = check_direction(closed_spaces[i], game, x_step, y_step);		//check above

			y_step = 1;
			if(check)
				check = check_direction(closed_spaces[i], game, x_step, y_step);		//check below
		}
		else if(closed_spaces[i].column == 0 || closed_spaces[i].column == 7){		//if on right or left we only need to check if the entire edge is filled
			y_step = 0;
			x_step = 1;
			if(check)
				check = check_direction(closed_spaces[i], game, x_step, y_step);		//check right

			x_step = -1;
			if(check)
				check = check_direction(closed_spaces[i], game, x_step, y_step);		//check left
		}
		else{
			x_step = 0;
			y_step = -1;
			check = check_direction(closed_spaces[i], game, x_step, y_step);		//check above

			y_step = 1;
			if(check)
				check = check_direction(closed_spaces[i], game, x_step, y_step);		//check below

			y_step = 0;
			x_step = 1;
			if(check)
				check = check_direction(closed_spaces[i], game, x_step, y_step);		//check right

			x_step = -1;
			if(check)
				check = check_direction(closed_spaces[i], game, x_step, y_step);		//check left

			y_step = -1;
			if(check)
				check = check_direction(closed_spaces[i], game, x_step, y_step);		//check top left

			x_step = 1;
			if(check)
				check = check_direction(closed_spaces[i], game, x_step, y_step);		//check top right

			y_step = 1;
			if(check)
				check = check_direction(closed_spaces[i], game, x_step, y_step);		//check bottom right

			x_step = -1;
			if(check)
				check = check_direction(closed_spaces[i], game, x_step, y_step);		//check bottom left
		}
		if(check)
			number = number + 1;
	}
	return number;
}

bool AI::is_corner(Position p){
	if((p.row == 0 || p.row == 7) && (p.column == 0 || p.column == 7))
		return true;
	else
		return false;
}

bool AI::check_direction(Position current_position, Reversi game, int x_step, int y_step){
	vector<vector<char>> board = game.get_board();
	bool check = true;
	int x = current_position.row + x_step;
	int y = current_position.column + y_step;

	while(x >= 0 && x <= 7 && y >= 0 && y <= 7){
		if(board[x][y] == 'o'){
			check = false;
			break;
		}
		x += x_step;
		y += y_step;
	}
	return check;
}

bool sort_positions(const Position& left, const Position& right) {
	return (left.row*10 + left.column) < (right.row*10 + right.column);
}

bool equal_positions(const Position& left, const Position& right) {
	return left.row == right.row && left.column == right.column;
}

vector<Position> AI::get_unstable_tiles(Reversi game) {
	vector<Position> available_moves = game.get_available_move_positions();
	vector<Position> unstable_tiles;

	for(unsigned int i = 0; i < available_moves.size(); ++i) {
		vector<Position> temp_positions;

		temp_positions = get_move_tiles(available_moves[i], 0, -1, game);		//check above
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		temp_positions = get_move_tiles(available_moves[i], 0, 1, game);		//check below
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		temp_positions = get_move_tiles(available_moves[i], 1, 0, game);		//check right
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		temp_positions = get_move_tiles(available_moves[i], -1, 0, game);		//check left
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		temp_positions = get_move_tiles(available_moves[i], -1, -1, game);		//check top left
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		temp_positions = get_move_tiles(available_moves[i], 1, -1, game);		//check top right
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		temp_positions = get_move_tiles(available_moves[i], 1, 1, game);		//check bottom right
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		temp_positions = get_move_tiles(available_moves[i], -1, 1, game);		//check bottom left
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());
	}

	// REMOVE DUPLICATES
	sort(unstable_tiles.begin(), unstable_tiles.end(), sort_positions);
	auto last = unique(unstable_tiles.begin(), unstable_tiles.end(), equal_positions);
	unstable_tiles.erase(last, unstable_tiles.end());

	return unstable_tiles;
}

vector<Position> AI::get_move_tiles(Position start, int x_step, int y_step, Reversi game) {
	char current_player = game.get_current_player();
	char opp;
	vector<vector<char>> board = game.get_board();
	if(current_player == 'w')
		opp = 'b';
	else
		opp = 'w';
	int x = start.row;
	int y = start.column;
	
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
