//
// game.h
// Hexapawn
//
// Manages the game in Hexapawn
//

#ifndef __Hexapawn__game__
#define __Hexapawn__game__

void new_game_controller(void);
void resume_game_controller(void);
void play_game(game_mode_t mode);
int get_cell_number(char *buffer);
hexmove_t get_player_move();
piece_t get_piece_at(int cell);
void set_piece_at(int cell, piece_t piece);
int get_file(int cell);
bool validate_player_move(int from, int to);
void move_piece(int from, int to);
void display_prompt(char *prompt);
void clear_prompt(void);
void draw_board(void);
void draw_horiz_board_divider(int y, chtype left_edge, chtype middle, chtype right_edge);
void draw_vert_board_divider(int x);
void draw_piece(int location, piece_t piece_type);
#endif /* defined(__Hexapawn__game__) */
