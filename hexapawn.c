/**
 * @file hexapawn.c
 * @brief A C version of Martin Gardner's Hexapawn game and matchbox AI
 * @author Laurence Scotford
 */

#include "hexapawn.h"

int main(void) {
	// Switch on debug mode - REMOVE THIS FOR PRODUCTION
	set_debug_mode(true);

	// Setup data structure
	initialise_game_data();

	// Start ncurses
	initialise_curses();

	// Build the game windows
	initialise_windows();

	// Build the menus and forms
	initialise_menus();
	initialise_forms();

	// Start the main menu
	main_menu_controller();

	// End ncurses
	destroy_forms();
	destroy_menus();
	end_curses();

	// Close game data
	close_game_data();

	return 0;
}
