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
#define STR_CELL_OCCUPIED "The to position is occupied - you cannot move there! Enter a valid move..."
#define STR_DATA_ERROR "The data required by Hexapawn could not be created"
#define STR_DIAGONAL_NO_CAPTURE "You can only move diagonally when capturing a black pawn! Enter a valid move..." 
#define STR_ENTER_MOVE "Enter your move..."
#define STR_ENTER_VALID_MOVE "Enter the cell number of the piece to move (1 to 9) and the cell number to move it to (1 to 9)."
#define STR_FORM_ERROR "A form required by Hexapawn could not be created"
#define STR_GAME_SUBTITLE "Play a game"
#define STR_HIST_SUBTITLE "Browse game history"
#define STR_INSTRUCTIONS_SUBTITLE "Instructions"
#define STR_INVALID_NAV_DISPATCH "The navigation dispatcher was called with invalid arguments"
#define STR_INVALID_CAPTURE "You can only capture diagonally! Enter a valid move..."
#define STR_INVALID_MOVE "You can only move one space forward or one space diagnoally forward when capturing! Enter a valid move..."
#define STR_LOAD_SUBTITLE "Load game history and AI"
#define STR_MAIN_TITLE "Hexapawn: "
#define STR_MENU_ERROR "A menu required by Hexapawn could not be created"
#define STR_MENU_EXIT "Exit"
#define STR_MENU_MARK "> "
#define STR_MENU_NEW "New game"
#define STR_MENU_RESET "Reset AI and history"
#define STR_MENU_RESUME "Resume game"
#define STR_MENU_SUBTITLE "Menu"
#define STR_MOVE_FORM_FROM "From:"
#define STR_MOVE_FORM_TO "To:"
#define STR_NO_COLOUR "Your terminal does not support colour, which is needed for Hexapawn"
#define STR_NO_WHITE "There is no white pawn in the from position! Enter a valid move..."
#define STR_PANEL_ERROR "A panel required by Hexapawn could not be created"
#define STR_PLAY_MOVE "Move"
#define STR_SAVE_SUBTITLE "Save game history and AI"
#define STR_TERMINAL_TOO_SMALL "Your terminal window is too small to run Hexapawn\nPlease expand your terminal window to be at least " xstr(WIN_MAIN_HEIGHT) " lines high by " xstr(WIN_MAIN_WIDTH) " characters wide and try again"
#define STR_WINDOW_ERROR "A window required by Hexapawn could not be created"

#endif // defined(__Hexapawn__hexstrings_h)
