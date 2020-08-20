//
// hexapawn.c
// Hexapawn
//

#include <ncurses.h>
#include <panel.h>

int main(void) {
	// Start ncurses
	initscr();
	noecho();
	curs_set(0);
	refresh();

	// Create first window and link it to a panel
	WINDOW * window1 = newwin(10, 15, 5, 12);
	box(window1, 0, 0);
	mvwaddstr(window1, 4, 3, "window 1");
	wrefresh(window1);
	PANEL * win1panel = new_panel(window1);
	
	// Add a second window and link it to a panel
	WINDOW * window2 = newwin(10, 15, 8, 20);
	box(window2, 0, 0);
	mvwaddstr(window2, 4, 3, "window2");
	wrefresh(window2);
	PANEL * win2panel = new_panel(window2);

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
