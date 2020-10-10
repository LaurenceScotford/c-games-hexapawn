//
// instructions.c
// Hexapawn
//
// Manages the instructions window for Hexapawn
//

#include "hexapawn.h"
#include "hexstrings.h"

void instructions_controller() {
	show_window(WIN_INSTRUCTIONS);
	while(wgetch(get_hexwindow(WIN_INSTRUCTIONS)->w_ptr) != 'x');
}
