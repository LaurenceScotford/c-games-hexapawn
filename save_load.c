//
// save_load.c
// Hexapawn
//
// Manages the interface for saving and loading of game state for Hexapawn
//

#include "hexapawn.h"

void save_controller() {
	show_window(WIN_SAVE);
	while(wgetch(get_hexwindow(WIN_SAVE)->w_ptr) != 'x');
}

void load_controller() {
	show_window(WIN_LOAD);
	while(wgetch(get_hexwindow(WIN_LOAD)->w_ptr) != 'x');
}
