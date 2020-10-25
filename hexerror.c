/**
 * @file hexerror.c
 * @brief Error handling routines for Hexapawn
 * @author Laurence Scotford
 */

#include "hexapawn.h"

void exit_with_error(char * error_string) {
	if (stdscr != NULL) {
		endwin();
	}

	printf("Hexapawn has closed because of an error:\n%s\n", error_string);
	exit(1);
}
