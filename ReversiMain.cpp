#include "Reversi.h"

#include <iostream>
#include <unistd.h>

using namespace std;

void random_move_loop(Reversi game) {
	while(!game.is_game_over()) {
		cout << "----------MAKE RANDOM MOVE----------";
		game.make_random_move();
		cout << "MOVE:" << game.get_previous_move();
		cout << game.get_state_string();
		sleep(2);
	}
}

int main() {
	Reversi game;

	cout << "----------SET PLAYER: WHITE----------";
	game.set_current_player('w');
	sleep(2);

	cout << "----------MAKE MOVE: d3----------";
	game.make_move("d3");
	cout << game.get_state_string();
	sleep(2);

	random_move_loop(game);

	cout << "----------UNDO----------";
	game.undo();
	sleep(2);

	random_move_loop(game);

	cout << "GAME OVER!";
	cout << game.get_state_string();
	sleep(2);
}