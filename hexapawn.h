/**
 * @file hexapawn.h
 * @brief Common definitions and values for Hexapawn
 * @author Laurence Scotford
 */

#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include <menu.h>
#include <apr_general.h>
#include <apr_ring.h>
#include <apr_time.h>

#ifndef __Hexapawn__hexapawn_h
#define __Hexapawn__hexapawn_h

// Unitary values

#define BOARD_CELL_HEIGHT 5
#define BOARD_CELL_WIDTH 10
#define BOARD_X 2
#define BOARD_Y 3
#define BORDER_WIDTH 1
#define DIVIDER_X 0
#define DIVIDER_Y 2
#define EMPTY_SPRITE_DATA_LENGTH 1 
#define GRID_SIZE_X 3
#define GRID_SIZE_Y 3
#define MAIN_MENU_HEIGHT 10
#define MAIN_MENU_ITEMS 10
#define MAIN_MENU_WIDTH 26
#define MAIN_MENU_X 3
#define MAIN_MENU_Y 5
#define MENU_EXIT 8
#define PAWN_SPRITE_DATA_LENGTH 17
#define TITLE_X 3
#define TITLE_Y 1
#define WIN_MAIN_HEIGHT 24
#define WIN_MAIN_WIDTH 80
#define WIN_MAIN_X 0
#define WIN_MAIN_Y 0

// Enumerated values

/**
 * @brief Possible modes when starting a game
 */
typedef enum _game_mode_t {
	GAME_MODE_NEW,
	GAME_MODE_RESUME
} game_mode_t;

/**
 * @brief Possible states for a game
 */
typedef enum _game_state_t {
	STATE_IN_PROGRESS,
	STATE_ENDED_RESIGNED,
	STATE_ENDED_FAR_SIDE,
	STATE_ENDED_BLOCKED,
	STATE_ENDED_TAKEN
} game_state_t;

/**
 * @brief Indexes for ncurses colour pairs defined for use in the game
 */
typedef enum _hex_colour_pairs_t {
	BLACK_GREEN_PAIR = 1,
	WHITE_GREEN_PAIR
} hex_colour_pairs_t;

/**
 * @brief Unique IDs for menus
 */
typedef enum _menu_id_t {
	MENU_MAIN
} menu_id_t;

/**
 * @brief The possible piece types that can occupy a board position
 */
typedef enum _piece_t {
	PIECE_EMPTY,
	PIECE_WHITE,
	PIECE_BLACK
} piece_t;

/**
 * @brief Categories for game windows
 */
typedef enum _win_cat_t {
	WT_MAIN
} win_cat_t;

/**
 * @brief Unique IDs for game windows
 */
typedef enum _window_id_t {
	WIN_MAIN_MENU,
	WIN_INSTRUCTIONS,
	WIN_GAME,
	WIN_AI,
	WIN_HISTORY,
	WIN_SAVE,
	WIN_LOAD
} window_id_t;

// View structures

/**
 * @brief Describes a character and run length for use in a character sprite definition
 */
typedef struct _char_run_t {
	int count;			///< The number of times the character is to be repreated
	chtype character;	///< The character to display
} char_run_t;

/**
 * @brief Describes a game menu
 */
typedef struct _hexmenu_t {
	menu_id_t uid;			///< The unqiue ID for the menu
	window_id_t window;		///< The uid of the window to which the menu belongs
	WINDOW * subwindow;		///< A pointer to the subwindow in which the menu will be drawn
	MENU * menu;			///< A pointer to the menu data
	ITEM ** menu_items;		///< A pointer to an array of menu items
	int menu_y;				///< The y position of the menu in the subwindow
	int menu_x;				///< The x position of the menu in the subwindow
	int menu_height;		///< The height of the menu	
	int menu_width;			///< The width of the menu
	int num_items;			///< The number of items in the menu
	char * item_list[10];	///< An array of strings describing the menu items
} hexmenu_t;

/**
 * @brief Describes a game window
 */
typedef struct _hexwindow_t {
	window_id_t uid;		///< The unique id for the window 
	win_cat_t w_cat;		///< The window's category
	WINDOW * w_ptr;			///< A pointer to the window data
	PANEL * p_ptr;			///< A pointer to the panel data associated with this window
	const char * subtitle;	///< The subtitle to show at the top of the window
} hexwindow_t;

// Model structures

/**
 * @brief Describes a single move in a game - used in an APR ring type (cyclic linked list)
 */
typedef struct _hexmove_t {
	APR_RING_ENTRY(_hexmove_t) link;	///< Holds the links to the neighbouring elements in the linked list
	int from;							///< The location the piece moved from
	int to;								///< The location the piece moved to
} hexmove_t;

/**
 * @brief APR linked list that holds the move history for a game
 */
typedef struct _moves_list_t moves_list_t;
APR_RING_HEAD(_moves_list_t, _hexmove_t);

/**
 * @brief Describes an entire game of Hexapawn - used in an APR Ring type (cyclic linked list)
 */
typedef struct _hexgame_t {
	APR_RING_ENTRY(_hexgame_t) link;	///< Holds the links to the neighbouring elements in the linked list
	piece_t board[9];					///< An array holding the current configuration of the board
	apr_time_t start_time;				///< A time stamp recording when the game started
	apr_time_t end_time;				///< A time stamp recording when the gaem ended
	piece_t next_player;				///< The player due to play next
	game_state_t state;					///< The current state of the game
	moves_list_t * moves;				///< A pointer to a cyclic linked list with the move history for the game
} hexgame_t;

/**
 * @brief An APR ring type (cyclic linked list) holding descriptions of games of hexapawn
 */
typedef struct _games_list_t games_list_t;
APR_RING_HEAD(_games_list_t, _hexgame_t);

/**
 * @brief Describes a position on the game board
 */
typedef struct _position_t {
	int y;	///< The y position or row (0 = top row)
	int x;	///< The x position or column (0 = left column)
} position_t;

// Macro to find size of array
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#endif // defined(__Hexapawn__hexapawn_h)

#include "hexwindows.h"
#include "hexerror.h"
#include "hexutils.h"
#include "hexdata.h"
#include "menu.h"
#include "instructions.h"
#include "game.h"
#include "ai.h"
#include "history.h"
#include "save_load.h"
