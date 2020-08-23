//
// hexapawn.h
// Hexapawn
//
// Common definitions and values for Hexapawn
//

#include <ncurses.h>
#include <panel.h>

#ifndef __Hexapawn__hexapawn_h
#define __Hexapawn__hexapawn_h

#define BORDER_WIDTH 1
#define DIVIDER_X 0
#define DIVIDER_Y 2
#define TITLE_X 3
#define TITLE_Y 1
#define WIN_MAIN_HEIGHT 24
#define WIN_MAIN_WIDTH 80
#define WIN_MAIN_X 0
#define WIN_MAIN_Y 0

enum window_id {
	WIN_MAIN_MENU,
	WIN_INSTRUCTIONS,
	WIN_GAME,
	WIN_AI,
	WIN_HISTORY,
	WIN_SAVE,
	WIN_LOAD
};

enum win_cat {
	WT_MAIN
};

struct hexwindow {
	enum window_id uid;
	enum win_cat w_cat;
	WINDOW * w_ptr;
	PANEL * p_ptr;
	const char * subtitle;
};

// Macro to find size of array
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#endif // defined(__Hexapawn__hexapawn_h)

#include "hexwindows.h"
