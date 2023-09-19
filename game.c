/**
 * @file game.c
 * @brief Manages the game in Hexapawn
 * @author Laurence Scotford
 */

#include "hexapawn.h"

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
 * @param mode An enum (GAME_MODE_NEW or GAME_MODE_RESUME) to indicate the game mode
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
	show_window(WIN_MOVE_FORM);

	hexmove_t player_move;

	// While game in progress
	while (current_game->state == STATE_IN_PROGRESS) {
			// if current player is white
			if (current_game->next_player == PIECE_WHITE) {
				player_move = get_player_move();
			} else {
				// TO DO: Get AI move
			}

			// If player has resigned...
			if (player_move.from == -1 && player_move.to == -1) {
				// ... set status to end game
				current_game->state = STATE_ENDED_RESIGNED;
			} else {
				// Make move
				move_piece(player_move.from, player_move.to);
			}
			
			// TO DO: If win condition 
				// Show win statement
				// End game
			// Else switch player
	}	
}

/**
 * @brief Get a player move
 * @returns A move entered by the player (will bet set to -1 -1 if the player is resigning)
*/
hexmove_t get_player_move() {
	int choice;
	hexmove_t move;
	bool valid_move = true;
	
	curs_set(1);		// Switch on the cursor

	display_prompt(STR_ENTER_MOVE);
	reset_form(FORM_PLAYER_MOVE, false);


	do {
		choice = navigation_dispatch(MENU_PLAY, FORM_PLAYER_MOVE);

		if (choice == PLAY_MOVE) {
			char * from_ptr = get_form_field_pointer(FORM_PLAYER_MOVE, 0);
			char * to_ptr = get_form_field_pointer(FORM_PLAYER_MOVE, 1);

			move.from = get_cell_number(from_ptr);
			move.to = get_cell_number(to_ptr);

			if (choice == PLAY_MOVE) {
				valid_move = validate_player_move(move.from, move.to);
			}
		}

	} while (!(choice == PLAY_EXIT || choice == PLAY_MOVE) || !valid_move);

	if (choice == PLAY_EXIT) {
		move.from = -1;
		move.to = -1;
	}

	curs_set(0); // Switch off the cursor

	return move;
}

/** 
 * @brief Gets a cell number from the given field buffer
 * @param buffer A pointer to buffer for a form field
 * @returns An integer number that was entered or -1 if a non-integer was entered
 */
int get_cell_number(char * buffer) {
	errno = 0;
	long val = strtol(buffer, NULL, 10);

	if (errno == EINVAL) {
		return -1;
	}

	return (int)val;
}

/** 
 * @brief Validates a move and shows an error message if the move is invalid
 * @param from The cell the player is trying to move from
 * @param to The cell the player is trying to move to
 * @returns True if the move is valid, False otherwise
 */
bool validate_player_move(int from, int to) {
	bool is_valid_move = false;
	int move_offset = from - to;
	int file = get_file(from);

	// Check move is within the board bounds
	if (from < 1 || to < 1 || from > 9 || to > 9) {
		display_prompt(STR_ENTER_VALID_MOVE);
	} else if (get_piece_at(from) != PIECE_WHITE) {
		// There is no white piece at the selected from cell
		display_prompt(STR_NO_WHITE);
	} else if (move_offset == 3) {
		// Player is moving forward one square
		piece_t piece_at_dest = get_piece_at(to);
		if (piece_at_dest == PIECE_WHITE) {
			display_prompt(STR_CELL_OCCUPIED);
		} else if (piece_at_dest == PIECE_BLACK) {
			display_prompt(STR_INVALID_CAPTURE);
		} else {
			is_valid_move = true;
		}
	} else if ((file == 1 && move_offset == 2) || (file == 2 && (move_offset == 2 || move_offset == 4)) || (file = 3 && move_offset == 4)) {
		// Player is moving diagonally
		if (get_piece_at(to) != PIECE_BLACK) {
			// Diagonal move doesn't lead to capture
			display_prompt(STR_DIAGONAL_NO_CAPTURE);
		} else {
			is_valid_move = true;
		}
	} else {
		// Player is attempting to move to an invalid position
		display_prompt(STR_INVALID_MOVE);
	}

	if (is_valid_move) {
		reset_form(FORM_PLAYER_MOVE, false);
	} else {
		reset_form(FORM_PLAYER_MOVE, true);
	}

	return is_valid_move;
}

/** 
 * @brief Returns the piece at a given cell in the current game
 * @param cell The number of the cell to interrogate
 * @returns The piece at the selected cell
 */
piece_t get_piece_at(int cell) {
	return current_game->board[cell - 1];
}

/** 
 * @brief Sets the piece at the given cell
 * @param cell The number of the cell to set the piece
 * @param piece The piece to set at that cell
 */
void set_piece_at(int cell, piece_t piece) {
	current_game->board[cell - 1] = piece;
}

/** 
 * @brief Returns the file for a given cell
 * @param cell The number of the cell to interrogate
 * @returns The file from 1(left) to 3(right) 
 */
int get_file(int cell) {
	int modulus = cell % 3;
	return modulus == 0 ? 3 : modulus; 
}

/** 
 * @brief Displays a prompt
 * @param prompt A pointer to the prompt string to be displayed
 */
void display_prompt(char * prompt) {
	write_to_rect(WIN_GAME, prompt, GAME_PROMPT_Y, GAME_PROMPT_X, GAME_PROMPT_HEIGHT, GAME_PROMPT_WIDTH);
}

/** 
 * @brief Moves a piece
 * @param from The number of the cell to move from
 * @param to The number of the cell to move to
 */
void move_piece(int from, int to) {
	piece_t piece_to_move = get_piece_at(from);
	set_piece_at(from, PIECE_EMPTY);
	set_piece_at(to, piece_to_move);
	draw_piece(from, PIECE_EMPTY); 
	draw_piece(to, piece_to_move); 
}

/** 
 * @brief Draws the game board
 */
void draw_board() {
	WINDOW * win = get_hexwindow(WIN_GAME)->w_ptr;
	wattron(win, COLOR_PAIR(WHITE_MAGENTA_PAIR));

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
	
	wattroff(win, COLOR_PAIR(WHITE_MAGENTA_PAIR)); 

	// Draw the correct piece in each position on the board
	for (int i = 1; i <= GRID_SIZE_Y * GRID_SIZE_X; i++) {
		draw_piece(i, get_piece_at(i));
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
 * @param location A number representing the board cell in which to draw the piece (1-9)
 * @param piece_type The type of piece to draw
 */
void draw_piece(int location, piece_t piece_type) {
	char_run_t * piece_sprite;
	piece_sprite = (piece_type == PIECE_EMPTY ? empty_sprite : pawn_sprite);

	WINDOW * win = get_hexwindow(WIN_GAME)->w_ptr;

	wattron(win, COLOR_PAIR(piece_type == PIECE_BLACK ? BLACK_MAGENTA_PAIR : WHITE_MAGENTA_PAIR));

	// Calculate position of top left character in sprite
	position_t pos = get_position(location - 1);
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
				
	wattroff(win, COLOR_PAIR(piece_type == PIECE_BLACK ? BLACK_MAGENTA_PAIR : WHITE_MAGENTA_PAIR));
}
