//
// save_load.c
// Hexapawn
//
// Manages the interface for saving and loading of game state for Hexapawn
//

#include "hexapawn.h"
#include "hexstrings.h"

void save_controller() {
	show_window(WIN_SAVE);
	while(getch() != 'x');
}

void load_controller() {
	show_window(WIN_LOAD);
	while(getch() != 'x');
}
