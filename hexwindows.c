//
// hexwindows.c
// Hexapawn
//
// Functions for managing the windows in Hexapawn
//

#include "hexapawn.h"
#include "hexstrings.h"

static struct hexwindow hexwindows[] = {
	// Main menu
	{WIN_MAIN_MENU, WT_MAIN, NULL, NULL, STR_MENU_SUBTITLE},
	// Instructions
	{WIN_INSTRUCTIONS, WT_MAIN, NULL, NULL, STR_INSTRUCTIONS_SUBTITLE},
	// Game
	{WIN_GAME, WT_MAIN, NULL, NULL, STR_GAME_SUBTITLE},
	// AI
	{WIN_AI, WT_MAIN, NULL, NULL, STR_AI_SUBTITLE},
	// History
	{WIN_HISTORY, WT_MAIN, NULL, NULL, STR_HIST_SUBTITLE},
	// Save
	{WIN_SAVE, WT_MAIN, NULL, NULL, STR_SAVE_SUBTITLE},
	// Load
	{WIN_LOAD, WT_MAIN, NULL, NULL, STR_LOAD_SUBTITLE}
};

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

// Create a main window
void create_main_window(WINDOW ** window, PANEL ** panel, const char * subtitle) {
	// Create the window and panel
	create_basic_window(window, panel, WIN_MAIN_HEIGHT, WIN_MAIN_WIDTH, WIN_MAIN_Y, WIN_MAIN_X);

	// Add a title and a divider between title and content area
	mvwprintw(*window, TITLE_Y, TITLE_X, "%s%s", STR_MAIN_TITLE, subtitle);
	mvwaddch(*window, DIVIDER_Y, DIVIDER_X, ACS_LTEE);
	mvwhline(*window, DIVIDER_Y, DIVIDER_X + 1, ACS_HLINE, WIN_MAIN_WIDTH - BORDER_WIDTH * 2);
	mvwaddch(*window, DIVIDER_Y, DIVIDER_X + WIN_MAIN_WIDTH - 1, ACS_RTEE);
}

// Create all the permanant windows
void initialise_windows(void) {
	for (int i = 0; i < ARRAY_SIZE(hexwindows); i++) {
		// struct hexwindow ws = hexwindows[i];
		switch (hexwindows[i].w_cat) {
			case WT_MAIN:
				create_main_window(&hexwindows[i].w_ptr, &hexwindows[i].p_ptr, hexwindows[i].subtitle);
				break;
		}
	}
}

// Return a pointer to the struct that matches the uid or NULL if no match is found
struct hexwindow * get_hexwindow(enum window_id win_id) {
	for (int i = 0; i < ARRAY_SIZE(hexwindows); i++) {
		if (hexwindows[i].uid == win_id) {
			return &hexwindows[i];
		}
	}
	return NULL;
}

// Show the specified window
void show_window(enum window_id win_id) {
	struct hexwindow * hex_win = get_hexwindow(win_id);

	// Update the window
	wrefresh(hex_win->w_ptr);

	// Show the panel
	top_panel(hex_win->p_ptr);
	update_panels();
	doupdate();
}
