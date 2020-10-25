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
void draw_board(void);
void draw_horiz_board_divider(int y, chtype left_edge, chtype middle, chtype right_edge);
void draw_vert_board_divider(int x);
void draw_piece(int location, piece_t piece_type);
#endif /* defined(__Hexapawn__game__) */
