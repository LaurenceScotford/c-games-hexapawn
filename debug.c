/**
 * @file debug.c
 * @brief Manages displaying messages in the debug window
 * @author Laurence Scotford
 */

#include "hexapawn.h"

static bool debug_mode = false;
static char err_str[DEBUG_STR_LENGTH];

void set_debug_mode(bool mode) {
    debug_mode = mode;
}

bool get_debug_mode() {
    return debug_mode;
}

void debug_message(char * message, ...) {
    if (debug_mode) {
        va_list args;
        va_start (args, message);
        vsprintf(err_str, message, args);
        va_end(args);

        hexwindow_t * debug_win = get_hexwindow(WIN_DEBUG);
        wclear(debug_win->w_ptr);
        wprintw(debug_win->w_ptr, err_str);
        wrefresh(debug_win->w_ptr);
    }
}