//
// hexwindows.h
// Hexapawn
//
// Functions for managing the windows in Hexapawn
//

#ifndef __Hexapawn__hexwindows__
#define __Hexapawn__hexwindows__

void initialise_curses(void);
void create_basic_window(WINDOW ** window, PANEL ** panel, int height, int width, int y, int x);
void create_main_window(WINDOW ** window, PANEL ** panel, const char * subtitle);
void destroy_menus(void);
void initialise_menus(void);
void initialise_windows(void);
struct hexmenu * get_hexmenu(enum menu_id m_id);
struct hexwindow * get_hexwindow(enum window_id win_id);
int menu_navigation(enum menu_id m_id);
void show_window(enum window_id win_id);

# endif /* defined(__Hexapawn_hexwindows__) */
