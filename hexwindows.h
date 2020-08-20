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

# endif /* defined(__Hexapawn_hexwindows__) */
