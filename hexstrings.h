//
// hexstrings.h
// Hexapawn
//
// Contains all the string literals for Hexapawn
//

#ifndef __Hexapawn__hexstrings_h
#define __Hexapawn__hexstrings_h

#define xstr(arg) str(arg)
#define str(arg) #arg

#define STR_AI_SUBTITLE "Browse AI"
#define STR_GAME_SUBTITLE "Play a game"
#define STR_HIST_SUBTITLE "Browse game history"
#define STR_INSTRUCTIONS_SUBTITLE "Instructions"
#define STR_LOAD_SUBTITLE "Load game history and AI"
#define STR_MAIN_TITLE "Hexapawn: "
#define STR_MENU_ERROR "A menu required by Hexapawn could not be created"
#define STR_MENU_EXIT "Exit"
#define STR_MENU_MARK "> "
#define STR_MENU_NEW "New game"
#define STR_MENU_RESET "Reset AI and history"
#define STR_MENU_RESUME "Resume game"
#define STR_MENU_SUBTITLE "Menu"
#define STR_PANEL_ERROR "A panel required by Hexapawn could not be created"
#define STR_SAVE_SUBTITLE "Save game history and AI"
#define STR_TERMINAL_TOO_SMALL "Your terminal window is too small to run Hexapawn\nPlease expand your terminal window to be at least " xstr(WIN_MAIN_HEIGHT) " lines high by " xstr(WIN_MAIN_WIDTH) " characters wide and try again"
#define STR_WINDOW_ERROR "A window required by Hexapawn could not be created"

#endif // defined(__Hexapawn__hexstrings_h)
