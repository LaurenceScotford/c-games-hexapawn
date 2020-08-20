//
// hexapawn.c
// Hexapawn
//

#include <ncurses.h>
#include <panel.h>
#include "hexwindows.h"

int main(void) {
	// Start ncurses
	initialise_curses();

	// Create first window and link it to a panel
	WINDOW * window1;
	PANEL * win1panel;
	create_basic_window(&window1, &win1panel, 10, 15, 5, 12);
	mvwaddstr(window1, 4, 3, "window 1");
	wrefresh(window1);
	
	// Add a second window and link it to a panel
	WINDOW * window2;
	PANEL * win2panel;
	create_basic_window(&window2, &win2panel, 10, 15, 8, 20);
	mvwaddstr(window2, 4, 3, "window2");
	wrefresh(window2);

	int c, order = 1;

	do {
		c = getch();

		if (c == 's') {
			top_panel(order ? win1panel : win2panel);
			update_panels();
			doupdate();
			order = !order;
		}
	} while (c != 'x');

	// End ncurses
	endwin();

	return 0;
}
