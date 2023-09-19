/**
 * @file hexmenus.c
 * @brief Functions for managing the menus in Hexapawn
 * @author Laurence Scotford
 */

#include "hexapawn.h"

static apr_pool_t * hex_menus_pool;

static hexmenu_t hexmenus[] = {
	// Main menu
	{MENU_MAIN, WIN_MAIN_MENU, NULL, NULL, NULL, FORM_NONE, MAIN_MENU_Y, MAIN_MENU_X, MAIN_MENU_HEIGHT, MAIN_MENU_WIDTH, MAIN_MENU_ITEMS, MENU_VERTICAL,
		(char *[MAIN_MENU_ITEMS]){
			STR_INSTRUCTIONS_SUBTITLE, 
			STR_MENU_NEW, 
			STR_MENU_RESUME,
			STR_AI_SUBTITLE,
			STR_HIST_SUBTITLE,
			STR_SAVE_SUBTITLE,
			STR_LOAD_SUBTITLE,
			STR_MENU_RESET,
			STR_MENU_EXIT,
			(char *) NULL}
	},
	// Player move menu
	{MENU_PLAY, WIN_GAME, NULL, NULL, NULL, FORM_PLAYER_MOVE, PLAY_MENU_Y, PLAY_MENU_X, PLAY_MENU_HEIGHT, PLAY_MENU_WIDTH, PLAY_MENU_ITEMS, MENU_HORIZONTAL,
		(char *[PLAY_MENU_ITEMS]){
			STR_PLAY_MOVE,
			STR_MENU_EXIT,
			(char *) NULL}
	},
};

/**
 * @brief Create the menus
 */
void initialise_menus() {

    // Create a data pool for hexmenus
    if (apr_pool_create(&hex_menus_pool, NULL) != APR_SUCCESS) {
		exit_with_error(STR_DATA_ERROR);
	}

	for (int i = 0; i < ARRAY_SIZE(hexmenus); i++) {
		
		// Create the menu items
		hexmenus[i].menu_items = (ITEM **) apr_palloc(hex_menus_pool, hexmenus[i].num_items * sizeof(ITEM *));
		
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

		// Switch off display of descriptions
		menu_opts_off(hexmenus[i].menu, O_SHOWDESC);

		//Set single item selection
		menu_opts_on(hexmenus[i].menu, O_ONEVALUE);

		// Change the default orientation if the menu is horizontal
		if (hexmenus[i].orientation == MENU_HORIZONTAL) {
			set_menu_format(hexmenus[i].menu, 1, hexmenus[i].num_items - 1);	
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

	apr_pool_destroy(hex_menus_pool);
}

/**
 * @brief Return a pointer to the struct that matches the menu uid or NULL if no match is found
 * @param m_id The unique ID of the required menu
 * @returns A pointer to the structure describing the menu or NULL if a match cannot be found
 */
hexmenu_t * get_hexmenu(menu_id_t m_id) {
	for (int i = 0; i < ARRAY_SIZE(hexmenus); i++)
	{
		if (hexmenus[i].uid == m_id) {
			return &hexmenus[i];
		}
	}

	return NULL;
}

/**
 * @brief Handle navigation for the given menu and return the index of the selected item
 * @param m_id The unique ID of the menu to be navigated
 * @param c The numeric representation of the character passed through from the navigation function
 * @returns A nav_ouput struct indicating if navigation should continue and the
 * 			position of the selected item (0 = top item in list) if it should not
 */
nav_output_t menu_navigation(menu_id_t m_id, int c) {
	hexmenu_t * m_ptr = get_hexmenu(m_id);
	nav_output_t nav_output = {true, -1};

	switch(c) {
		case KEY_DOWN:
			if (m_ptr->orientation == MENU_VERTICAL) {	
				menu_driver(m_ptr->menu, REQ_DOWN_ITEM);
			}
			break;
		case KEY_UP:
			if (m_ptr->orientation == MENU_VERTICAL) {
				menu_driver(m_ptr->menu, REQ_UP_ITEM);
			}
			break;
		case KEY_LEFT:
			if (m_ptr->orientation == MENU_HORIZONTAL) {
				menu_driver(m_ptr->menu, REQ_LEFT_ITEM);
			}
			break;
		case KEY_RIGHT:
			if(m_ptr->orientation == MENU_HORIZONTAL) {
				menu_driver(m_ptr->menu, REQ_RIGHT_ITEM);
			}
			break;
		case KEY_HOME:
			menu_driver(m_ptr->menu, REQ_FIRST_ITEM);
			break;
		case KEY_END:
			menu_driver(m_ptr->menu, REQ_LAST_ITEM);
			break;
		case '\n':
			for (int i = 0; i < m_ptr->num_items; i++) {
				if (current_item(m_ptr->menu) == m_ptr->menu_items[i]) {
					nav_output = (nav_output_t){false, i};
					break;
				}
			}
			break;
		}

	if (m_ptr->form != FORM_NONE) {
		pos_form_cursor(get_hexform(m_ptr->form)->form);
	}

	show_window(m_ptr->window);

	return nav_output;
}