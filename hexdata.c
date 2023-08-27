/**
 * @file hexdata.c
 * @brief Manages the underlying Hexapawn data structure
 * @author Laurence Scotford
 */

#include "hexapawn.h"

apr_pool_t * hex_data_pool;
games_list_t * games_list; 

/**
 * @brief Initialise data structure
 */ 
void initialise_game_data() {
	// Attempt to initialise Apache Portable Runtime and create the memory pool for the game data
	if (apr_initialize() != APR_SUCCESS || apr_pool_create(&hex_data_pool, NULL) != APR_SUCCESS) {
		exit_with_error(STR_DATA_ERROR);
	}

	// Create a new linked list to hold the game history
	games_list = apr_palloc(hex_data_pool, sizeof(games_list_t));
	APR_RING_INIT(games_list, _hexgame_t, link);
}

/**
 * @brief Create the data structure for a new game
 * @returns A pointer to the game structure
 */
hexgame_t * create_new_game() {
	// The starting board configuration for a new game
	piece_t starting_board[9] = {
		PIECE_BLACK, PIECE_BLACK, PIECE_BLACK,
		PIECE_EMPTY, PIECE_EMPTY, PIECE_EMPTY,
		PIECE_WHITE, PIECE_WHITE, PIECE_WHITE
	};

	// Create a new game with the initial board configuration and an empty moves list
	hexgame_t * new_game = (hexgame_t *) apr_palloc(hex_data_pool, sizeof(hexgame_t));
	memcpy(new_game->board, starting_board, sizeof(starting_board));
	new_game->next_player = PIECE_WHITE;
	new_game->state = STATE_IN_PROGRESS;
    new_game->moves = apr_palloc(hex_data_pool, sizeof(moves_list_t));
	APR_RING_INIT(new_game->moves, _hexmove_t, link);
	new_game->start_time = apr_time_now();
	APR_RING_INSERT_TAIL(games_list, new_game, _hexgame_t, link);
	return new_game;	
}

/**
 * @brief Removes the game data and terminates Apache Portable Runtime
 */
void close_game_data() {
	apr_pool_destroy(hex_data_pool);
	apr_terminate();
}
