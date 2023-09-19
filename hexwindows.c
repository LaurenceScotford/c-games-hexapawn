/**
 * @file hexwindows.c
 * @brief Functions for managing the windows in Hexapawn
 * @author Laurence Scotford
 */

#include "hexapawn.h"

static main_window_t main_windows[] = {
	{STR_MENU_SUBTITLE},
	{STR_INSTRUCTIONS_SUBTITLE},
	{STR_GAME_SUBTITLE},
	{STR_AI_SUBTITLE},
	{STR_HIST_SUBTITLE},
	{STR_SAVE_SUBTITLE},
	{STR_LOAD_SUBTITLE}
};

static form_window_t form_windows[] = {
	{FORM_PLAYER_MOVE}
};

static hexwindow_t hexwindows[] = {
	// Main menu
	{WIN_MAIN_MENU, WT_MAIN, WIN_NONE, NULL, NULL, &main_windows[0]},
	// Instructions
	{WIN_INSTRUCTIONS, WT_MAIN, WIN_NONE, NULL, NULL, &main_windows[1]},
	// Game
	{WIN_GAME, WT_MAIN, WIN_NONE, NULL, NULL, &main_windows[2]},
	// AI
	{WIN_AI, WT_MAIN, WIN_NONE, NULL, NULL, &main_windows[3]},
	// History
	{WIN_HISTORY, WT_MAIN, WIN_NONE, NULL, NULL, &main_windows[4]},
	// Save
	{WIN_SAVE, WT_MAIN, WIN_NONE, NULL, NULL, &main_windows[5]},
	// Load
	{WIN_LOAD, WT_MAIN, WIN_NONE, NULL, NULL, &main_windows[6]},
	// Move form
	{WIN_MOVE_FORM, WT_FORM, WIN_GAME, NULL, NULL, &form_windows[0]},
	// Debug window
	{WIN_DEBUG, WT_DEBUG, WIN_NONE, NULL, NULL, NULL}
};

/**
 * @brief Initalise ncurses and configure it for the game
 */
void initialise_curses(void)
{
	initscr();		// Start ncurses

	// Check that terminal window is of a sufficient size to play
	int num_lines = WIN_MAIN_HEIGHT + get_debug_mode() ? DEBUG_WIN_HEIGHT : 0;

	if (LINES < num_lines || COLS < WIN_MAIN_WIDTH) {
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
	init_pair(BLACK_MAGENTA_PAIR, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(WHITE_MAGENTA_PAIR, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(BLACK_GREEN_PAIR, COLOR_BLACK, COLOR_GREEN);
	
	refresh();		// Draw stdscr (workaround for windows bug)
}

void end_curses() {	
		endwin();		
}

// Windows functions

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
 * @brief Create a sub window
 * @param parent A pointer to the parent window
 * @param window A pointer to the pointer to thelocation to store the window data
 * @param height The height of the window to be created
 * @param width The width of the window to be created
 * @param y The y coordinate of the top-left corner of the window
 * @param x The x coordinate of the top-left corner of the window
 */
void create_sub_window (WINDOW * parent, WINDOW ** window, int height, int width, int y, int x) {
	// Create the window
	*window = derwin(parent, height, width, y, x);

	if (*window == NULL) {
		exit_with_error(STR_WINDOW_ERROR);
	}
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
 * @brief Create a form window
 * @param window A pointer to the parent window for the form
 * @param window A pointer to a pointer to the location where window data will be stored
 * @param form The uid of the form that will be displayed in this window
 */
void create_form_window(WINDOW * parent, WINDOW ** window, form_id_t form) {
	// Get the associated form
	hexform_t * form_ptr = get_hexform(form);

	// Create the window
	create_sub_window(parent, window, form_ptr->form_height, form_ptr->form_width, form_ptr->form_y, form_ptr->form_x);
}

/**
 * @brief Create a debug window
 */
void create_debug_window(WINDOW ** window) {
	*window = newwin(DEBUG_WIN_HEIGHT, DEBUG_WIN_WIDTH, DEBUG_WIN_Y, DEBUG_WIN_X);
	wbkgd(*window, COLOR_PAIR(BLACK_GREEN_PAIR));
	debug_message("Debug window initialised!");
	wrefresh(*window);
}

/**
 * @brief Create all the permanant windows
 */
void initialise_windows(void) {
	for (int i = 0; i < ARRAY_SIZE(hexwindows); i++) {
		switch (hexwindows[i].w_cat) {
			case WT_MAIN:
				create_main_window(&hexwindows[i].w_ptr, &hexwindows[i].p_ptr, ((main_window_t *)hexwindows[i].window_data)->subtitle);
				break;
			case WT_FORM:
				create_form_window(hexwindows[hexwindows[i].parent_id].w_ptr, &hexwindows[i].w_ptr, ((form_window_t *)hexwindows[i].window_data)->form);
				break;
			case WT_DEBUG:
				create_debug_window(&hexwindows[i].w_ptr);
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
	hexwindow_t *hex_win = get_hexwindow(win_id);

	// Update the window
	wrefresh(hex_win->w_ptr);

	// Show the panel, if there is one for this window
	if (hex_win->p_ptr != NULL) {		
		top_panel(hex_win->p_ptr);
		update_panels();
		doupdate();
	}
}

/**
 * @brief Get lowest common window
 * @param win_id_1 The unique ID of the first window
 * @param win_id_2 The unique ID of the second window
 * @returns The ID of the lowest common window or null if none was found
 */
window_id_t get_lowest_common_window(window_id_t win_id_1, window_id_t win_id_2) {
	
	window_id_t match = WIN_NONE;	// Will be set to lowest matching window if found
	bool match_failed = false;		// Will be set to true if no match found

	// Create temporary stores for current windows while walking the tree
	window_id_t current_win_1 = win_id_1;
	window_id_t current_win_2 = win_id_2;

	// Walk the tree to try and find a match
	do {
		// If the current IDs match, we're done
		if (current_win_1 == current_win_2) {
			match = current_win_1;
		} else {
			// Otherwise get the parent of the current window 2
			window_id_t next_win_2 = get_hexwindow(current_win_2)->parent_id;
			if (next_win_2 != WIN_NONE) {
				// If there is a parent, this is what we'll compare next
				current_win_2 = next_win_2;
			} else {
				// If there isn't, reset window 2 and get the parent of current window 1
				current_win_2 = win_id_2;
				window_id_t next_win_1 = get_hexwindow(current_win_1)->parent_id;
				if (next_win_1 != WIN_NONE) {
					// If there is a parent, this is what we'll compare next
					current_win_1 = next_win_1;
				} else {
					// If there isn't, we've explored the whole tree without finding a match
					match_failed = true;
				}
			}
		}

	} while (match == WIN_NONE && !match_failed);

	// Return a match if we found one, or WIN_NONE if we didn't
	return match;
}

/** 
 * @brief Write to rect
 * @param prompt A pointer to the prompt string to be displayed
 */
void write_to_rect(window_id_t win_id, char * text, int start_y, int start_x, int max_height, int max_width) {
	clear_rect(win_id, start_y, start_x, max_height, max_width, ' ');

	WINDOW * win = get_hexwindow(win_id)->w_ptr;
	int current_y = start_y - 1;
	int current_count = max_width;

	char *string_to_tokensise = calloc(strlen(text + 1), sizeof(char));
	strcpy(string_to_tokensise, text);

	char * next_word = strtok(string_to_tokensise," ");
	
	while (next_word != NULL)
	{
		int word_length = strlen(next_word) + 1;

		if (current_count + word_length > max_width) {
			current_count = 0;
			current_y++;

			if (current_y - start_y > max_height) {
				break;
			}

			wmove(win, current_y, start_x);
		} else {
			current_count += word_length + 1;
		}

		waddstr(win, next_word);
		waddch(win, ' ');
		next_word = strtok(NULL, " ");
	}

	free(string_to_tokensise);
}

/** 
 * @brief Clears a rectangular area in a given window to a given character
 * @param win_id The id of the target window
 * @param start_y The row to start at
 * @param start_x The column to start at
 * @param height The height of the rect to clear
 * @param width The width of the rect to clear
 * @param clr_char	The character to set the cleared area to 
 */
void clear_rect(window_id_t win_id, int start_y, int start_x, int height, int width, char clr_char) {
	WINDOW * win = get_hexwindow(win_id)->w_ptr;

	for (int i = 0; i < height; i++) {
		wmove(win, start_y + i, start_x);
		for (int j = 0; j < width; j++) {
			waddch(win, clr_char);
		}
	}
}

