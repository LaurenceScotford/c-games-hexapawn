//
// menu.c
// Hexapawn
//
// Manages the main menu for the Hexapawn game
//

#include "hexapawn.h"
#include "hexstrings.h"

void (*funcPtrs[])(void) = {
	&instructions_controller,
	&new_game_controller,
	&resume_game_controller,
	&ai_controller,
	&history_controller,
	&save_controller,
	&load_controller,
	&reset_controller
};

void main_menu_controller() {
	int running = 1;
	show_window(WIN_MAIN_MENU);
	
	while (running > 0) {
		int choice = menu_navigation(MENU_MAIN);
		
		if (choice != MENU_EXIT) {
			(*funcPtrs[choice])();
			show_window(WIN_MAIN_MENU);
		} else {
			running = 0;
		}
	}
}

void reset_controller() {
	// TO DO: Add reset controller code
}
