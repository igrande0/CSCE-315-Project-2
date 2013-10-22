#include "AI.h"
#include "Reversi.h"

#include <cfloat>
#include <algorithm>
#include <iostream>
#include <climits>

/*------------------------------------------------------------------------------------*/
/* DEFINES and HELPER FUNCTIONS */
/*------------------------------------------------------------------------------------*/

#define CAPTURED_CORNER_WEIGHT 99
#define POTENTIAL_CORNER_WEIGHT 33

bool less_than(const Position& left, const Position& right) {
	return (left.row*10 + left.column) < (right.row*10 + right.column);
}

bool equals(const Position& left, const Position& right) {
	return left.row == right.row && left.column == right.column;
}

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

		if(current_value > best_value) {
			best_value = current_value;
			best_move = available_moves[i];
		}

		if(current_value > alpha)
			alpha = current_value;

		if(alpha > beta) {
			break;
		}
	}

	return {best_move, best_value};
}

//Picks the best available move based on weighted tile values
string AI::get_educated_move(Reversi game){
	vector<Position> available_moves = game.get_available_move_positions();
	int value = INT_MIN;
	Position best_move;
	for(unsigned int i=0; i < available_moves.size(); i++)
		if(value_table[available_moves[i].row][available_moves[i].column] > value){
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

	Reversi new_game = game;
	new_game.toggle_player();
	new_game.update_state();

	vector<Position> available_moves_max = game.get_available_move_positions();
	vector<Position> available_moves_min = new_game.get_available_move_positions();

	// potential corners
	for(int i = 0; i < available_moves_max.size(); i++ )
		if(is_corner(available_moves_max[i]))
			max_potential++;

	for(int i = 0; i < available_moves_min.size(); i++ )
		if(is_corner(available_moves_min[i]))
			min_potential++;

	// captured corners	
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

	// overall corner score
	max_player_corner = max_captured*CAPTURED_CORNER_WEIGHT + max_potential*POTENTIAL_CORNER_WEIGHT;
	min_player_corner = min_captured*CAPTURED_CORNER_WEIGHT + min_potential*POTENTIAL_CORNER_WEIGHT;

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
	int max_unstable_count;
	int min_unstable_count;
	int max_stable_count;
	int min_stable_count;

	Reversi new_game = game;
	new_game.toggle_player();
	new_game.update_state();

	// find all player pieces
	vector<Position> all_max_pieces = get_player_pieces(game);
	vector<Position> all_min_pieces = get_player_pieces(new_game);
	
	// find unstable player pieces
	vector<Position> min_unstable = get_unstable_tiles(game);
	vector<Position> max_unstable = get_unstable_tiles(new_game);
	
	max_unstable_count = max_unstable.size();
	min_unstable_count = min_unstable.size();

	// sort all position vectors
	sort(all_max_pieces.begin(), all_max_pieces.end(), less_than);
	sort(all_min_pieces.begin(), all_min_pieces.end(), less_than);
	sort(max_unstable.begin(), max_unstable.end(), less_than);
	sort(min_unstable.begin(), min_unstable.end(), less_than);

	// get pieces of unknown stability
	vector<Position> unknown_max_pieces(all_max_pieces.size());
	vector<Position> unknown_min_pieces(all_min_pieces.size());

	vector<Position>::iterator end;
	
	end = set_difference(all_max_pieces.begin(), all_max_pieces.end(),
		max_unstable.begin(), max_unstable.end(), unknown_max_pieces.begin(), less_than);
	unknown_max_pieces.resize(end - unknown_max_pieces.begin());

	end = set_difference(all_min_pieces.begin(), all_min_pieces.end(),
		min_unstable.begin(), min_unstable.end(), unknown_min_pieces.begin(), less_than);
	unknown_min_pieces.resize(end - unknown_min_pieces.begin());

	// determine stable pieces
	max_stable_count = get_num_stable_tiles(game, unknown_max_pieces);
	min_stable_count = get_num_stable_tiles(new_game, unknown_min_pieces);

	max_player_stability = max_stable_count - max_unstable_count;
	min_player_stability = min_stable_count - min_unstable_count;

	if(max_player_stability + min_player_stability == 0)
		return 0;
	else
		return 100*(max_player_stability - min_player_stability)/(max_player_stability + min_player_stability);
}

vector<Position> AI::get_player_pieces(Reversi game) {
	vector<vector<char>> current_board = game.get_board();
	Position p;
	vector<Position> player_pieces;

	// find all of the current player's pieces
	for(unsigned int i=0; i<8; i++){
		for(unsigned int j=0; j<8; j++){
			if(current_board[i][j] == game.get_current_player()){
				p.row = i;
				p.column = j;
				player_pieces.push_back(p);
			}	
		}
	}
	
	return player_pieces;
}

int AI::get_num_stable_tiles(Reversi game, vector<Position> player_pieces) {
	int num_stable = 0;

	for(unsigned int i=0; i< player_pieces.size(); i++){
		bool is_stable = false;

		// if tile is corner then it is always stable
		if(is_corner(player_pieces[i]))			
			is_stable = true;

		// if on top or bottom edge we only need to check to see if the entire edge is filled
		else if(player_pieces[i].row == 0 || player_pieces[i].row == 7){			
			//check right
			if(is_stable)
				is_stable = check_direction(player_pieces[i], game, 1, 0);		

			//check left
			if(is_stable)
				is_stable = check_direction(player_pieces[i], game, -1, 0);		
		}

		// if on right or left we only need to check if the entire edge is filled
		else if(player_pieces[i].column == 0 || player_pieces[i].column == 7){		
			//check above
			is_stable = check_direction(player_pieces[i], game, 0, -1);		

			//check below
			if(is_stable)
				is_stable = check_direction(player_pieces[i], game, 0, 1);		
		}
		else{
			//check above
			is_stable = check_direction(player_pieces[i], game, 0, -1);

			//check below
			if(is_stable)
				is_stable = check_direction(player_pieces[i], game, 0, 1);

			//check right
			if(is_stable)
				is_stable = check_direction(player_pieces[i], game, 1, 0);

			//check left
			if(is_stable)
				is_stable = check_direction(player_pieces[i], game, -1, 0);

			//check top left
			if(is_stable)
				is_stable = check_direction(player_pieces[i], game, -1, -1);

			//check top right
			if(is_stable)
				is_stable = check_direction(player_pieces[i], game, 1, -1);
			
			//check bottom right
			if(is_stable)
				is_stable = check_direction(player_pieces[i], game, 1, 1);

			//check bottom left
			if(is_stable)
				is_stable = check_direction(player_pieces[i], game, -1, 1);	
		}
		if(is_stable)
			++num_stable;
	}
	return num_stable;
}

bool AI::is_corner(Position p){
	return (p.row == 0 || p.row == 7) && (p.column == 0 || p.column == 7);
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

vector<Position> AI::get_unstable_tiles(Reversi game) {
	vector<Position> available_moves = game.get_available_move_positions();
	vector<Position> unstable_tiles;

	for(unsigned int i = 0; i < available_moves.size(); ++i) {
		vector<Position> temp_positions;

		//check above
		temp_positions = get_move_tiles(available_moves[i], 0, -1, game);
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		//check below
		temp_positions = get_move_tiles(available_moves[i], 0, 1, game);
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		//check right
		temp_positions = get_move_tiles(available_moves[i], 1, 0, game);
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		//check left
		temp_positions = get_move_tiles(available_moves[i], -1, 0, game);
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		//check top left
		temp_positions = get_move_tiles(available_moves[i], -1, -1, game);
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		//check top right
		temp_positions = get_move_tiles(available_moves[i], 1, -1, game);
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		//check bottom right
		temp_positions = get_move_tiles(available_moves[i], 1, 1, game);
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());

		//check bottom left
		temp_positions = get_move_tiles(available_moves[i], -1, 1, game);
		unstable_tiles.insert(unstable_tiles.end(), temp_positions.begin(), temp_positions.end());
	}

	// remove duplicates
	sort(unstable_tiles.begin(), unstable_tiles.end(), less_than);
	auto last = unique(unstable_tiles.begin(), unstable_tiles.end(), equals);
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
