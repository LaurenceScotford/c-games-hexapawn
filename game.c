//
// game.c
// Hexapawn
//
// Manages the game in Hexapawn
//

#include "hexapawn.h"
#include "hexstrings.h"

void new_game_controller() {
	play_game(GAME_MODE_NEW);
}

void resume_game_controller() {
	play_game(GAME_MODE_RESUME);
}

void play_game(enum game_mode mode) {
	show_window(WIN_GAME);
	while(wgetch(get_hexwindow(WIN_GAME)->w_ptr) != 'x');
}
