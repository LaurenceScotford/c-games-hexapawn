/**
 * @file hexutils.c
 * @brief Utility functions for Hexapawn
 * @author Laurence Scotford
 */

#include "hexapawn.h"

/**
 * @brief Converts a single digit board position to a y and x position on the board
 * @param index The single digit position on the board
 * @returns A position_t structure holding the y and x position
 */
position_t get_position(int index) {
	position_t pos;
	pos.y = index / GRID_SIZE_X;
	pos.x = index % GRID_SIZE_X;
	return pos;
}
