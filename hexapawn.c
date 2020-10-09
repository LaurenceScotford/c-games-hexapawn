//
// hexapawn.c
// Hexapawn
//

#include "hexapawn.h"
#include "hexstrings.h"

int main(void) {
	// Start ncurses
	initialise_curses();

	// Build the game windows
	initialise_windows();

	// Build the menus
	initialise_menus();

	// Start the main menu
	main_menu_controller();

	// End ncurses
	destroy_menus();
	endwin();

	return 0;
}
