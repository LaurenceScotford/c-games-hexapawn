/**
 * @file hexdata.h
 * @brief Manages the underlying Hexapawn data structure
 * @author Laurence Scotford
 */ 

#ifndef __Hexapawn__hexdata__
#define __Hexapawn__hexdata__

void initialise_game_data(void);
hexgame_t * create_new_game(void);
void close_game_data(void);

#endif /* defined(__Hexapawn__hexdata__) */ 
