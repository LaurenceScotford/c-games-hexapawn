//
// history.c
// Hexapawn
//
// Manages the history browser for Hexapawn
//

#include "hexapawn.h"
#include "hexstrings.h"

void history_controller() {
	show_window(WIN_HISTORY);
	while(getch() != 'x');
}
