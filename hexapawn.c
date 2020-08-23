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

	show_window(WIN_MAIN_MENU);

	int c;
	
	while ((c = getch()) != 'x') {
		switch (c) {
			case 'm':
				show_window(WIN_MAIN_MENU);
				break;
			case 'i':
				show_window(WIN_INSTRUCTIONS);
				break;
			case 'g':
				show_window(WIN_GAME);
				break;
			case 'a':
				show_window(WIN_AI);
				break;
			case 'h':
				show_window(WIN_HISTORY);
				break;
			case 's':
				show_window(WIN_SAVE);
				break;
			case 'l':
				show_window(WIN_LOAD);
				break;
		}
	}

	// End ncurses
	endwin();

	return 0;
}
