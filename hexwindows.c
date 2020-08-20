//
// hexwindows.c
// Hexapawn
//
// Functions for managing the windows in Hexapawn
//

#include <ncurses.h>
#include <panel.h>
#include "hexwindows.h"

// Initialise ncurses
void initialise_curses(void)
{
	initscr();		// Start ncurses
	cbreak();		// Disable line buffer
	noecho();		// User input will not be echoed to screen
	keypad(stdscr, TRUE);	// Enable special keyboard characters
	curs_set(0);		// Switch off the cursor
	refresh();		// Draw stdscr (workaround for windows bug)
}

// Create a basic window with an associated panel and a border
void create_basic_window(WINDOW ** window, PANEL ** panel, int height, int width, int y, int x) {
	// Create the window
	*window = newwin(height, width, y, x);

	keypad(*window, TRUE);

	// Add the window to a panel
	*panel = new_panel(*window);

	// Display a border around the window
	box(*window, 0, 0);
}
