/**
 * @file hexapawn.c
 * @brief A C version of Martin Gardner's Hexapawn game and matchbox AI
 * @author Laurence Scotford
 */

#include "hexapawn.h"
#include "hexstrings.h"

int main(void) {
	// Setup data structure
	initialise_game_data();

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

	// Close game data
	close_game_data();

	return 0;
}
