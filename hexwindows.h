/**
 * @file hexwindows.h
 * @brief Functions for managing the windows in Hexapawn
 * @author Laurence Scotford
 */

#ifndef __Hexapawn__hexwindows__
#define __Hexapawn__hexwindows__

void initialise_curses(void);
void end_curses(void);
void create_basic_window(WINDOW ** window, PANEL ** panel, int height, int width, int y, int x);
void create_sub_window(WINDOW * parent, WINDOW ** window, int height, int width, int y, int x);
void create_main_window(WINDOW ** window, PANEL ** panel, const char * subtitle);
void create_form_window(WINDOW * parent, WINDOW ** window, form_id_t form);
hexwindow_t * get_hexwindow(window_id_t win_id);
void show_window(window_id_t win_id);
window_id_t get_lowest_common_window(window_id_t win_id_1, window_id_t win_id_2);
void write_to_rect(window_id_t win_id, char *text, int start_y, int start_x, int max_height, int max_width);
void clear_rect(window_id_t win_id, int start_y, int start_x, int height, int width, char clr_char);

#endif /* defined(__Hexapawn_hexwindows__) */
