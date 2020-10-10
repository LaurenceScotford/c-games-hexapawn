//
// ai.c
// Hexapawn
//
// Manages the game artificial intelligence browser for Hexapawn
//

#include "hexapawn.h"
#include "hexstrings.h"

void ai_controller() {
	show_window(WIN_AI);
	while(wgetch(get_hexwindow(WIN_AI)->w_ptr) != 'x');
}
