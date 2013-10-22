#include <iostream>
#include <unistd.h>

#include "AI.h"
#include "Reversi.h"

using namespace std;

int main(){
	cout << "Entered main\n";
	AI ai;
	cout << "Initialized AI\n";
	Reversi game;
	game.set_current_player('w');
	cout << "Easy move string: " << ai.get_move(game, AI::EASY) << "\nMedium move string: " << ai.get_move(game, AI::MEDIUM) << "\nHard move string: " << ai.get_move(game, AI::HARD) << '\n';
	cout << "Beginning game play!\n";
	while(!game.is_game_over()) {
		cout << ";----------MAKE HARD MOVE----------\n";
		game.make_move(ai.get_move(game,AI::HARD));
		cout << "MOVE:" << game.get_previous_move() << '\n';
		cout << game.get_state_string();
		sleep(1);
		if(!game.is_game_over()){
			cout << ";----------MAKE EASY MOVE----------\n";
			game.make_move(ai.get_move(game,AI::EASY));
			cout << "MOVE:" << game.get_previous_move() << '\n';
			cout << game.get_state_string();
			sleep(1);
		}
	}
	return 0;
}