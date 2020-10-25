/**
 * @file hexwindows.h
 * @brief Functions for managing the windows and menus in Hexapawn
 * @author Laurence Scotford
 */

#ifndef __Hexapawn__hexwindows__
#define __Hexapawn__hexwindows__

void initialise_curses(void);
void create_basic_window(WINDOW ** window, PANEL ** panel, int height, int width, int y, int x);
void create_main_window(WINDOW ** window, PANEL ** panel, const char * subtitle);
void destroy_menus(void);
void initialise_menus(void);
void initialise_windows(void);
hexmenu_t * get_hexmenu(menu_id_t m_id);
hexwindow_t * get_hexwindow(window_id_t win_id);
int menu_navigation(menu_id_t m_id);
void show_window(window_id_t win_id);

# endif /* defined(__Hexapawn_hexwindows__) */
