#include "Reversi.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <time.h>
//#include <stdiolib.h>

//Constructor
Reversi::Reversi() {
        for(unsigned int i = 0; i < 8; ++i)
                board.push_back({'o','o','o','o','o','o','o','o'});

        board[3][3] = 'w';
        board[3][4] = 'b';
        board[4][3] = 'b';
        board[4][4] = 'w';
        //cout << "Created Board!\n";
        //cout << "Got Moves!\n";
        update_score();
        //cout << "Updated State!\n";
}

//Public functions

bool Reversi::make_move(string move){
        //check size of move
        //cout << "Making move!\n";
        //cout << "size of move:" <<  sizeof(move) << endl;
        if(move.size() != 2)
                return false;
        //check if user has chosen side
        //cout << "Move sized correctly!\n";
        //cout << "current player:" << current_player << endl;
        if(current_player == 'n')
                return false;

        //split move into correct data types
        //cout << "Player has been set!\n";
        //cout << "Move string: " << move << endl;

        Position current_move;
        if(isalpha(move[0])) {
                current_move.column = get_number_of_letter(move[0]);
                current_move.row = move[1]-'0'-1;
        }
        else{
                current_move.row = get_number_of_letter(move[0]);
                current_move.column = move[1]-'0'-1;
        }
        //cout << "row: " << current_move.row << endl;
        //cout << "column: " << current_move.column << endl;

        // check if valid move
        bool possible_move_check = false;
        for(unsigned int i=0; i< available_moves.size(); i++)
                if(available_moves[i].row == current_move.row && available_moves[i].column == current_move.column)
                        possible_move_check = true;

        if(!possible_move_check)
                return false;
        //cout << "Move is valid!\n";

        // save previous state
        // only need to support 10 undos (20 total saved states)
        if(previous_states.size() >= 20)
                previous_states.pop_back();
        
        previous_states.push_front({board, available_moves, available_move_strings,
                                                                white_score, black_score, current_player});
        previous_move = move;

        //check all directions
        //if valid in a direction flip all appropriate tiles
        vector<Position> all_positions;
        vector<Position> temp_positions;
        all_positions.push_back(current_move);
        int x_step = 0;
        int y_step = -1;
        temp_positions = get_tiles(current_move, x_step, y_step);                //check above
        for(unsigned int i=0; i<temp_positions.size(); i++)
                all_positions.push_back(temp_positions[i]);
        y_step = 1;
        temp_positions = get_tiles(current_move, x_step, y_step);                //check below
        for(unsigned int i=0; i<temp_positions.size(); i++)
                all_positions.push_back(temp_positions[i]);
        y_step = 0;
        x_step = 1;
        temp_positions = get_tiles(current_move, x_step, y_step);                //check right
        for(unsigned int i=0; i<temp_positions.size(); i++)
                all_positions.push_back(temp_positions[i]);
        x_step = -1;
        temp_positions = get_tiles(current_move, x_step, y_step);                //check left
        for(unsigned int i=0; i<temp_positions.size(); i++)
                all_positions.push_back(temp_positions[i]);
        y_step = -1;
        temp_positions = get_tiles(current_move, x_step, y_step);                //check top left
        for(unsigned int i=0; i<temp_positions.size(); i++)
                all_positions.push_back(temp_positions[i]);
        x_step = 1;
        temp_positions = get_tiles(current_move, x_step, y_step);                //check top right
        for(unsigned int i=0; i<temp_positions.size(); i++)
                all_positions.push_back(temp_positions[i]);
        y_step = 1;
        temp_positions = get_tiles(current_move, x_step, y_step);                //check bottom right
        for(unsigned int i=0; i<temp_positions.size(); i++)
                all_positions.push_back(temp_positions[i]);
        x_step = -1;
        temp_positions = get_tiles(current_move, x_step, y_step);                //check bottom left
        for(unsigned int i=0; i<temp_positions.size(); i++)
                all_positions.push_back(temp_positions[i]);
        for(unsigned int i=0; i<all_positions.size(); i++)
                board[all_positions[i].row][all_positions[i].column] = current_player;

        toggle_player();
        update_state();
        return true;
}

bool Reversi::make_random_move(){
        if(available_moves.size() == 0)
                return false;

        srand(time(NULL));
        int random_index = rand() % available_moves.size();
        Position move = available_moves[random_index];

        string move_string = get_letter_of_number(move.column+1) + to_string(move.row + 1);

        return make_move(move_string);
}

bool Reversi::undo(){
        if(previous_states.size() < 2)
                return false;

        State last_state;

        // remove other player's move (or moves)
        while(previous_states.front().current_player != current_player)
                previous_states.pop_front();

        // revert to state before last move
        last_state = previous_states.front();
        board = last_state.board;
        available_moves = last_state.available_moves;
        available_move_strings = last_state.available_move_strings;
        white_score = last_state.white_score;
        black_score = last_state.black_score;
        current_player = last_state.current_player;

        // remove player's move
        previous_states.pop_front();
}

bool Reversi::is_game_over(){
        vector<Position> open_spaces = get_open_spaces();
        if(open_spaces.size() == 0)
			return true;
		else if(get_black_score() == 0 || get_white_score() == 0)
			return true;
        else
            return false;
}

void Reversi::clear_board(){                                                        //sets board to starting state of game
        for(unsigned int i=0; i<8; i++)
                for(unsigned int j=0; j<8; j++)
                        board[i][j] = 'o';
        board[3][3] = 'w';
        board[3][4] = 'b';
        board[4][3] = 'b';
        board[4][4] = 'w';
        update_score();                                                                        //updates score
}

bool Reversi::set_current_player(char player){
        if(current_player != 'n')
                return false;
        else if(player == 'b' || player == 'w') {
                current_player = player;
                update_state();
                return true;
        }
        else
                return false;
}

string Reversi::get_state_string(){
        string display_string;
        display_string +=";  _ _ _ _ _ _ _ _\n;";                                                        //displays board and current score
        for(unsigned int i=0; i<8; i++){
                display_string += to_string(i+1) + "|";
                for(unsigned int j=0; j<8; j++){
                        if(board[i][j] == 'w')
                                display_string += "O|";
                        else if(board[i][j] == 'b')
                                display_string += "@|";
                        else
                                display_string += "_|";
                }
                display_string += "\n;";
        }
        display_string += "  a b c d e f g h\n";
        display_string += ";White Score: ";
        display_string += to_string(white_score);
        display_string += "    Black Score: ";
        display_string += to_string(black_score) + "\n";

        return display_string;
}

//Private Member Functions

void Reversi::toggle_player() {
        if(current_player == 'b')
                current_player = 'w';
        else
                current_player = 'b';
}

vector<Position> Reversi::get_available_moves(){
        char opponent;
        if(current_player == 'w')                                //determines oppenents color by given argument
                opponent = 'b';
        else if(current_player == 'b')
                opponent = 'w';
        vector<Position> temp_vec;
        vector<Position> open_spaces = get_open_spaces();                                //need to check every direction for every space

        //cout << "Number of open spaces:" << open_spaces.size() << endl;
        for(unsigned int i=0; i<open_spaces.size(); i++){
                //check if board[i][j] is a possible move
                int temp_x = open_spaces[i].row;                //set to x coord
                int temp_y = open_spaces[i].column;                //set to y coord
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
                if(!check_push_back && stepping_loop(x_step, y_step, temp_x, temp_y, opponent)){                
                        temp_vec.push_back(open_spaces[i]);
                        check_push_back = true;
                }

                //check right
                y_step = 0;
                x_step = 1;
                if(!check_push_back && stepping_loop(x_step, y_step, temp_x, temp_y, opponent)){        
                        temp_vec.push_back(open_spaces[i]);
                        check_push_back = true;
                }

                //check left
                x_step = -1;
                if(!check_push_back && stepping_loop(x_step, y_step, temp_x, temp_y, opponent)){                
                        temp_vec.push_back(open_spaces[i]);
                        check_push_back = true;
                }

                //check top left
                y_step = -1;
                x_step = -1;
                if(!check_push_back && stepping_loop(x_step, y_step, temp_x, temp_y, opponent)){                
                        temp_vec.push_back(open_spaces[i]);
                        check_push_back = true;
                }

                //check top right
                x_step = 1;
                if(!check_push_back && stepping_loop(x_step, y_step, temp_x, temp_y, opponent)){        
                        temp_vec.push_back(open_spaces[i]);
                        check_push_back = true;
                }

                //check bottom right
                y_step = 1;
                if(!check_push_back && stepping_loop(x_step, y_step, temp_x, temp_y, opponent)){                
                        temp_vec.push_back(open_spaces[i]);
                        check_push_back = true;
                }

                //check bottom left
                x_step = -1;
                if(!check_push_back && stepping_loop(x_step, y_step, temp_x, temp_y, opponent)){        
                        temp_vec.push_back(open_spaces[i]);
                        check_push_back = true;
                }
        }
        return temp_vec;
}

vector<Position> Reversi::get_open_spaces(){
        vector<Position> open_spaces;                //creates temp vector to hold open spaces
        for(unsigned int i=0; i<8; i++)
                for(unsigned int j=0; j<8; j++)
                        if(board[i][j] == 'o'){                                                                        //if the space is clear then push the integer and char of space
                                //open_spaces.push_back({i+1,j+1});
                                Position p;
                                p.row = i;
                                p.column = j;
                                open_spaces.push_back(p);
                        }
        return open_spaces;                                                                                                //return temp vector containing all open spaces
}

bool Reversi::is_move_valid(Position move){
        for(unsigned int i=0; i<available_moves.size(); i++)
                if(move.row == available_moves[i].row && move.column == available_moves[i].column)
                        return true;
        return false;
}

bool Reversi::stepping_loop(int x_step, int y_step, int x, int y, char opp){
        x+=x_step;
        y+=y_step;
        bool mid_check=false;
        while(x < 8 && y < 8 && x >= 0 && y >= 0 && board[x][y] != 'o' ){
                if(board[x][y] == opp)
                        mid_check=true;
                else if(board[x][y] == current_player && mid_check == true)
                        return true;
                else if(board[x][y] == current_player && mid_check == false)
                        return false;
                x+=x_step;
                y+=y_step;
        }
        return false;
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

void Reversi::update_score(){
        int white_count(0), black_count(0);
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

void Reversi::update_state(){
        update_score();
        if(current_player != 'n') {
                available_moves = get_available_moves();

                if(available_moves.size() == 0 && !(is_game_over())) {
                        toggle_player();
                        available_moves = get_available_moves();
                }

                // update move strings
                available_move_strings.clear();
                for(unsigned int i = 0; i < available_moves.size(); ++i) {
                        string move_string;
                        move_string += get_letter_of_number(available_moves[i].column + 1);
                        move_string += to_string(available_moves[i].row + 1);
                        //cout << "move_string: " << move_string << " column: " << get_letter_of_number(available_moves[i].column + 1) << " \trow: " << available_moves[i].row +1 << '\n';
                        available_move_strings.push_back(move_string); //was avaliable_move_strings[i] segfaulted.
                }
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
