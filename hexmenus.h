/**
 * @file hexmenus.h
 * @brief Functions for managing the menus in Hexapawn
 * @author Laurence Scotford
 */

#ifndef __Hexapawn__hexmenus__
#define __Hexapawn__hexmenus__

void initialise_menus(void);
void initialise_windows(void);
hexmenu_t * get_hexmenu(menu_id_t m_id);
nav_output_t menu_navigation(menu_id_t m_id, int c);
void destroy_menus(void);

#endif /* defined(__Hexapawn_hexmenus__) */