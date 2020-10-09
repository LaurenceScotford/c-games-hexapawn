//
// hexapawn.h
// Hexapawn
//
// Common definitions and values for Hexapawn
//

#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>
#include <menu.h>

#ifndef __Hexapawn__hexapawn_h
#define __Hexapawn__hexapawn_h

#define BORDER_WIDTH 1
#define DIVIDER_X 0
#define DIVIDER_Y 2
#define MAIN_MENU_HEIGHT 10
#define MAIN_MENU_ITEMS 10
#define MAIN_MENU_WIDTH 26
#define MAIN_MENU_X 3
#define MAIN_MENU_Y 5
#define MENU_EXIT 8
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

enum menu_id {
	MENU_MAIN
};

enum game_mode {
	GAME_MODE_NEW,
	GAME_MODE_RESUME
};

struct hexwindow {
	enum window_id uid;
	enum win_cat w_cat;
	WINDOW * w_ptr;
	PANEL * p_ptr;
	const char * subtitle;
};

struct hexmenu {
	enum menu_id uid;
	enum window_id window;
	WINDOW * subwindow;
	MENU * menu;
	ITEM ** menu_items;
	int menu_y;
	int menu_x;
	int menu_height;
	int menu_width;
	int num_items;
	char * item_list[10];
};

// Macro to find size of array
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#endif // defined(__Hexapawn__hexapawn_h)

#include "hexwindows.h"
#include "hexerror.h"
#include "menu.h"
#include "instructions.h"
#include "game.h"
#include "ai.h"
#include "history.h"
#include "save_load.h"
