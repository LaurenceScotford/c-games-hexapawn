/**
 * @file game.c
 * @brief Manages the game in Hexapawn
 * @author Laurence Scotford
 */

#include "hexapawn.h"
#include "hexstrings.h"

static char_run_t * empty_sprite = NULL;
static char_run_t * pawn_sprite = NULL;
static hexgame_t * current_game;

/** 
 * @brief Starts a new game
 */
void new_game_controller() {
	play_game(GAME_MODE_NEW);
}

/**
 * @brief Resumes an existing game
 */
void resume_game_controller() {
	play_game(GAME_MODE_RESUME);
}

/** 
 * @brief Sets up and plays a game in the requested mode
 */
void play_game(game_mode_t mode) {
	empty_sprite = (char_run_t[EMPTY_SPRITE_DATA_LENGTH]){
		{50, ' '}	
	};

	pawn_sprite = (char_run_t[PAWN_SPRITE_DATA_LENGTH]){
		{4, ' '},
		{1, '/'},
		{1, '\\'},
		{8, ' '},
		{1, '\\'},
		{1, '/'},
		{8, ' '},
		{2, ACS_VLINE},
		{7, ' '},
		{1, '/'},
		{2, ' '},
		{1, '\\'},
		{5, ' '},
		{1, ACS_LLCORNER},
		{4, ACS_HLINE},
		{1, ACS_LRCORNER},
		{2, ' '}
	};

	//Temporary fix to stop resume game breaking
	mode = GAME_MODE_NEW;

	// If starting a new game, create a new game in the linked list
	if (mode == GAME_MODE_NEW) {
		current_game = create_new_game();
	} else {
		// TO DO: Resume a game in progress
	}

	draw_board();
	show_window(WIN_GAME);
	while(wgetch(get_hexwindow(WIN_GAME)->w_ptr) != 'x');
}

/** 
 * @brief Draws the game board
 */
void draw_board() {
	WINDOW * win = get_hexwindow(WIN_GAME)->w_ptr;
	wattron(win, COLOR_PAIR(WHITE_GREEN_PAIR));

	// Draw horizontal dividers and cross pieces
	for (int i = 0; i <= GRID_SIZE_Y; i++) {
		switch(i) {
			case 0:
				draw_horiz_board_divider(BOARD_Y, ACS_ULCORNER, ACS_TTEE, ACS_URCORNER);
				break;
			case GRID_SIZE_Y:
				draw_horiz_board_divider(BOARD_Y + BOARD_CELL_HEIGHT * i + i, ACS_LLCORNER, ACS_BTEE, ACS_LRCORNER);
				break;
			default:
				draw_horiz_board_divider(BOARD_Y + BOARD_CELL_HEIGHT * i + i, ACS_LTEE, ACS_PLUS, ACS_RTEE);
				break;
		}
	}

	// Draw vertical dividers
	for (int i = 0; i <= GRID_SIZE_Y; i++) {
		draw_vert_board_divider(BOARD_X + i * BOARD_CELL_WIDTH + i);
	}

	// Add numbers to squares on board
	for (int i = 0; i < GRID_SIZE_Y * GRID_SIZE_X; i++) {
		position_t pos = get_position(i);
		int y_pos = BOARD_Y + 1 + pos.y * BOARD_CELL_HEIGHT + pos.y;
		int x_pos = BOARD_X + 1 + pos.x * BOARD_CELL_WIDTH + pos.x;
		mvwaddch(win, y_pos, x_pos, '1' + i);
	}
	
	wattroff(win, COLOR_PAIR(WHITE_GREEN_PAIR)); 

	// Draw the correct piece in each position on the board
	for (int i = 0; i < GRID_SIZE_Y * GRID_SIZE_X; i++) {
		draw_piece(i, current_game->board[i]);
	}
}

/**
 * @brief Draws a horizontal divider
 * @param y The y position to draw the divider at within the game window
 * @param left_edge The character to use at the join on the left edge of the divider
 * @param middle The character to use at joins in the middle of the divider
 * @param right_edge The character to use at the join on the right edge of the divider
 */ 
void draw_horiz_board_divider(int y, chtype left_edge, chtype middle, chtype right_edge) {
	WINDOW * win = get_hexwindow(WIN_GAME)->w_ptr;
	
	// Draw left edge join at starting position
	mvwaddch(win, y, BOARD_X, left_edge);

	// Draw a horizontal line for each cell, ending with an appropriate join character
	for (int i = 0; i < GRID_SIZE_X; i++) {
    	mvwhline(win, y, BOARD_X + i * (BOARD_CELL_WIDTH) + i + 1, ACS_HLINE, BOARD_CELL_WIDTH);
	
		chtype join_char = (i < GRID_SIZE_X -1 ? middle : right_edge);
		mvwaddch(win, y, BOARD_X + BOARD_CELL_WIDTH + i * (BOARD_CELL_WIDTH) + i + 1, join_char);
	}
}

/**
 * @brief Draws a vertical divider
 * @param x The x position to draw the divider at within the game window
 */
void draw_vert_board_divider(int x) {
	WINDOW * win = get_hexwindow(WIN_GAME)->w_ptr;

	// Draw a vertical line for each cell, leaving gaps for the joins (drawn by draw_horizontal_board_divider)
	for (int i = 0; i < GRID_SIZE_Y; i++) {
		mvwvline(win, BOARD_Y + 1 + BOARD_CELL_HEIGHT * i + i, x, ACS_VLINE, BOARD_CELL_HEIGHT);
	}
}

/**
 * @brief Draws a single piece on the board
 * @param location A number representing the board cell in which to draw the piece (0-9)
 * @param piece_type The type of piece to draw
 */
void draw_piece(int location, piece_t piece_type) {
	char_run_t * piece_sprite;
	piece_sprite = (piece_type == PIECE_EMPTY ? empty_sprite : pawn_sprite);

	WINDOW * win = get_hexwindow(WIN_GAME)->w_ptr;

	wattron(win, COLOR_PAIR(piece_type == PIECE_BLACK ? BLACK_GREEN_PAIR : WHITE_GREEN_PAIR));

	// Calculate position of top left character in sprite
	position_t pos = get_position(location);
	int y_pos = BOARD_Y + 1 + pos.y * BOARD_CELL_HEIGHT + pos.y;
	int x_pos = BOARD_X + 1 + pos.x * BOARD_CELL_WIDTH + pos.x;

	int char_run_index = -1;	// Current poisition in sprite definition
	int char_run_count = 0;		// Number of instances of current character left to display
	chtype current_char;		// The character currently being written

	for (int i = 0; i < BOARD_CELL_HEIGHT; i++) {
		// Move cursor to start of current line (note 1st character of 1st line is skipped over to avoid overwriting cell numbers)
		wmove(win, y_pos + i, i == 0 ? x_pos + 1 : x_pos);  

		for (int j = 0; j < BOARD_CELL_WIDTH; j++) {
			// If all the instances of the current character have been drawn (or first character is needed) get the next character to draw
			if (char_run_count == 0) {
				char_run_index++;
				char_run_count = piece_sprite[char_run_index].count;
				current_char = piece_sprite[char_run_index].character;	
			}

			// Draw the character (note 1st character of 1st line is skipped over to avoid overwriting cell numbers)
			if (i > 0 || j > 0) {
				waddch(win, current_char);
			}

			char_run_count--;
		}
	}
				
	wattroff(win, COLOR_PAIR(piece_type == PIECE_BLACK ? BLACK_GREEN_PAIR : WHITE_GREEN_PAIR));
}
