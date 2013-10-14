#include "Reversi.h"

#include <iostream>
#include <unistd.h>

using namespace std;

void random_move_loop(Reversi game) {
	while(!game.is_game_over()) {
		cout << ";----------MAKE RANDOM MOVE----------\n";
		game.make_random_move();
		cout << "MOVE:" << game.get_previous_move() << '\n';
		cout << game.get_state_string();
		sleep(2);
	}
}

int main() {
	Reversi game;

	cout << ";----------SET PLAYER: WHITE----------\n";
	game.set_current_player('w');
	sleep(2);

	cout << ";----------MAKE MOVE: d2----------\n";
	game.make_move("d2");
	cout << game.get_state_string();
	sleep(2);

	random_move_loop(game);

	cout << ";----------UNDO----------\n";
	game.undo();
	sleep(2);

	random_move_loop(game);

	cout << ";GAME OVER!";
	cout << game.get_state_string();
	sleep(2);
}