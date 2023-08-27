//
// history.c
// Hexapawn
//
// Manages the history browser for Hexapawn
//

#include "hexapawn.h"

void history_controller() {
	show_window(WIN_HISTORY);
	while(wgetch(get_hexwindow(WIN_HISTORY)->w_ptr) != 'x');
}
