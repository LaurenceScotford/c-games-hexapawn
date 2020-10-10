# c-games-hexapawn
A C adaptation of the Hexapawn game from David Ahl's book "BASIC Computer Games".

This project is currently in progress.

To build work in progress download files and run **make**. Note that by default this creates an executable called **hexapawn** in the parent directory of the directory containing the source files. Please amend your local copy of makefile if you want to change this. If you want debugging symbols, run **make debug**

This project uses the [ncurses library](https://github.com/mirror/ncurses) including the panel and menu sub-libraries, so you will need to install those first.

Currently only the windows and menu handling code are complete.

The menu uses up and down arrow keys, HOME and END for navigation and return or enter to make a choice. When a sub-window is displayed, press x to return to the main menu.
