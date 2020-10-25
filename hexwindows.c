/**
 * @file hexwindows.c
 * @brief Functions for managing the windows in Hexapawn
 * @author Laurence Scotford
 */

#include "hexapawn.h"
#include "hexstrings.h"

static hexwindow_t hexwindows[] = {
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

static hexmenu_t hexmenus[] = {
	// Main menu
	{MENU_MAIN, WIN_MAIN_MENU, NULL, NULL, NULL, MAIN_MENU_Y, MAIN_MENU_X, MAIN_MENU_HEIGHT, MAIN_MENU_WIDTH, MAIN_MENU_ITEMS,
		{STR_INSTRUCTIONS_SUBTITLE, 
		STR_MENU_NEW, 
		STR_MENU_RESUME,
		STR_AI_SUBTITLE,
		STR_HIST_SUBTITLE,
		STR_SAVE_SUBTITLE,
		STR_LOAD_SUBTITLE,
		STR_MENU_RESET,
		STR_MENU_EXIT,
		(char *) NULL}}
};

/**
 * @brief Initalise ncurses and configure it for the game
 */
void initialise_curses(void)
{
	initscr();		// Start ncurses

	// Check that terminal window is of a sufficient size to play
	if (LINES < WIN_MAIN_HEIGHT || COLS < WIN_MAIN_WIDTH) {
		exit_with_error(STR_TERMINAL_TOO_SMALL);
	}

	// Check that terminal window supports colour
	if (has_colors() == FALSE) {
		exit_with_error(STR_NO_COLOUR);
	}

	cbreak();		// Disable line buffer
	noecho();		// User input will not be echoed to screen
	keypad(stdscr, TRUE);	// Enable special keyboard characters
	curs_set(0);		// Switch off the cursor
	start_color();	// Initialise colour mode
	init_pair(BLACK_GREEN_PAIR, COLOR_BLACK, COLOR_GREEN);
	init_pair(WHITE_GREEN_PAIR, COLOR_WHITE, COLOR_GREEN);
	refresh();		// Draw stdscr (workaround for windows bug)
}

/**
 * @brief Create a basic window with an associated panel and a border
 * @param window A pointer to the pointer to the location to store the window data
 * @param panel A pointer to the pointer to the location to store the panel data
 * @param height The height of the window to be created
 * @param width The width of the window to be created
 * @param y The y coordinate of the top-left corner of the window
 * @param x The x coordinate of the top-left corner of the window
 */
void create_basic_window(WINDOW ** window, PANEL ** panel, int height, int width, int y, int x) {
	// Create the window
	*window = newwin(height, width, y, x);

	if (*window == NULL) {
		exit_with_error(STR_WINDOW_ERROR);
	}

	keypad(*window, TRUE);

	// Add the window to a panel
	*panel = new_panel(*window);

	if (*panel == NULL) {
		exit_with_error(STR_PANEL_ERROR);
	}	

	// Display a border around the window
	box(*window, 0, 0);
}

/**
 * @brief Create a main window
 * @param window A pointer to a pointer to the location where window data will be stored
 * @param panel A pointer to a pointer to the location where panel data will be stored
 * @subtitle A string holding the subtitle to be displayed at the top of the window
 */
void create_main_window(WINDOW ** window, PANEL ** panel, const char * subtitle) {
	// Create the window and panel
	create_basic_window(window, panel, WIN_MAIN_HEIGHT, WIN_MAIN_WIDTH, WIN_MAIN_Y, WIN_MAIN_X);

	// Add a title and a divider between title and content area
	mvwprintw(*window, TITLE_Y, TITLE_X, "%s%s", STR_MAIN_TITLE, subtitle);
	mvwaddch(*window, DIVIDER_Y, DIVIDER_X, ACS_LTEE);
	mvwhline(*window, DIVIDER_Y, DIVIDER_X + 1, ACS_HLINE, WIN_MAIN_WIDTH - BORDER_WIDTH * 2);
	mvwaddch(*window, DIVIDER_Y, DIVIDER_X + WIN_MAIN_WIDTH - 1, ACS_RTEE);
}

/**
 * @brief Create all the permanant windows
 */
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

/**
 * @brief Return a pointer to the struct that matches the uid or NULL if no match is found
 * @param win_id The unique ID of the required window
 * @returns A pointer to the structure describing the window or NULL if a match could not be found
 */
hexwindow_t * get_hexwindow(window_id_t win_id) {
	for (int i = 0; i < ARRAY_SIZE(hexwindows); i++) {
		if (hexwindows[i].uid == win_id) {
			return &hexwindows[i];
		}
	}
	return NULL;
}

/**
 * @brief Show the specified window
 * @param win_id The unique ID of the window to be shown
 */
void show_window(window_id_t win_id) {
	hexwindow_t * hex_win = get_hexwindow(win_id);

	// Update the window
	wrefresh(hex_win->w_ptr);

	// Show the panel
	top_panel(hex_win->p_ptr);
	update_panels();
	doupdate();
}

/**
 * @brief Create the menus
 */
void initialise_menus() {
	for (int i = 0; i < ARRAY_SIZE(hexmenus); i++) {
		
		// Create the menu items
		hexmenus[i].menu_items = (ITEM **) calloc(hexmenus[i].num_items, sizeof(ITEM *));
		
		if (hexmenus[i].menu_items == NULL) {
			exit_with_error(STR_MENU_ERROR);
		}

		for (int j = 0; j < hexmenus[i].num_items; j++) {
			hexmenus[i].menu_items[j] = new_item(hexmenus[i].item_list[j], hexmenus[i].item_list[j]);

			if (j < hexmenus[i].num_items - 1 && hexmenus[i].menu_items[j] == NULL) {
				exit_with_error(STR_MENU_ERROR);
			}
		}

		// Create the menu
		hexmenus[i].menu = new_menu((ITEM **)hexmenus[i].menu_items);

		if (hexmenus[i].menu == NULL) {
			exit_with_error(STR_MENU_ERROR);
		}

		// Add menu to the relevant window
		hexwindow_t * menu_window = get_hexwindow(hexmenus[i].window);
		set_menu_win(hexmenus[i].menu, menu_window->w_ptr);
		hexmenus[i].subwindow = derwin(menu_window->w_ptr, hexmenus[i].menu_height, hexmenus[i].menu_width, hexmenus[i].menu_y, hexmenus[i].menu_x);

		if (hexmenus[i].subwindow == NULL) {
			exit_with_error(STR_MENU_ERROR);
		}

		set_menu_sub(hexmenus[i].menu, hexmenus[i].subwindow);
		set_menu_mark(hexmenus[i].menu, STR_MENU_MARK);

		// Post the menu
		post_menu(hexmenus[i].menu);
	}
}

/**
 * @brief Safely destroy the game menus
 */
void destroy_menus() {
	for (int i = 0; i < ARRAY_SIZE(hexmenus); i++) {
		unpost_menu(hexmenus[i].menu);
		free_menu(hexmenus[i].menu);
		
		for (int j = 0; j < hexmenus[i].num_items; j++) {
			free_item(hexmenus[i].menu_items[j]);	
		}		
	}
}

/**
 * @brief Return a pointer to the struct that matches the menu uid or NULL if no match is found
 * @param m_id The unique ID of the required menu
 * @returns A pointer to the structure describing the menu or NULL if a match cannot be found
 */
hexmenu_t * get_hexmenu(menu_id_t m_id) {
	for (int i = 0; i < ARRAY_SIZE(hexmenus); i++) {
		if (hexmenus[i].uid == m_id) {
			return &hexmenus[i];
		}
	}

	return NULL;
}

/**
 * @brief Handle navigation for the given menu and return the index of the selected item
 * @param m_id The unique ID of the menu to be navigated
 * @returns An int indicating the position of the selected item (0 = top item in list)
 */
int menu_navigation(menu_id_t m_id) {
	hexmenu_t * m_ptr = get_hexmenu(m_id);
	hexwindow_t * ws_ptr = get_hexwindow(m_ptr->window);
	int c;

	while ((c = wgetch(ws_ptr->w_ptr)) != '\n') {
		switch(c) {
			case KEY_DOWN:
				menu_driver(m_ptr->menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(m_ptr->menu, REQ_UP_ITEM);
				break;
			case KEY_HOME:
				menu_driver(m_ptr->menu, REQ_FIRST_ITEM);
				break;
			case KEY_END:
				menu_driver(m_ptr->menu, REQ_LAST_ITEM);
				break;
		}
		
		show_window(m_ptr->window);
	}

	for (int i = 0; i < m_ptr->num_items; i++) {
		if (current_item(m_ptr->menu) == m_ptr->menu_items[i]) {
			return i;
		}
	}

	return -1;
}
