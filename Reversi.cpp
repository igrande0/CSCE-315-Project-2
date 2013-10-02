#include "Reversi.h"
#include <iostream>

void Reversi::display_board(){
	cout<<"_ _ _ _ _ _ _ _"<<endl;
	for(unsigned int i=0; i<8; i++){
		cout<<(i+1)<<"|";
		for(unsigned int j=0; j<8; j++){
			if(board[i][j] == 'w')
				cout<<"O|";
			else if(board[i][j] == 'b')
				cout<<"@|";
			else
				cout<<"_|";
		}
		cout<<endl;
	}
			
}

void Reversi::clear_board(){
	for(unsigned int i=0; i<8; ++)
		for(unsigned int j=0; j<8 j++)
			board[i][j] = 'c';
	board[3][3] = 'w';
	board[3][4] = 'b';
	board[4][3] = 'b';
	board[4][4] = 'w';
}

void Reversi::update_score(){
	int white_count, black_count = 0;
	for(unsigned int i=0; i<8; ++)
		for(unsigned int j=0; j<8 j++){
			if(board[i][j] == 'w')
				white_count++;
			else if(board[i][j] == 'b')
				black_count++;
		}
	white_score = white_count;
	black_score = black_count;
}

vector<int, char> Reversi::get_avalible_moves(char side){
	char opponent;
	if(side == 'w')				//determines oppenents color by given argument
		opponent = 'b';
	else if(side == 'b')
		opponent = 'w';
	
}

vector<int, char> Reversi::get_open_spaces(){
	vector<int, char> open_spaces;		//creates temp vector to hold open spaces
	for(unsigned int i=0; i<8; ++)
		for(unsigned int j=0; j<8 j++)
			if(board[i][j] == 'c')									//if the space is clear then push the integer and char of space
				open_spaces.push_back((i+1),get_letter_of_number(j+1));		
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
			return -1;
			break;
	}
}