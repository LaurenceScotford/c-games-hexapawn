# makefile for Hexapawn

.DEFAULT_GOAL := build

build:
	@echo "Building hexapawn..."
	gcc hexapawn.c hexwindows.c hexerror.c menu.c instructions.c game.c ai.c history.c save_load.c -l menu -l ncurses -l panel -o ../hexapawn

debug:
	@echo "Building hexapawn with debugging symbols..."
	gcc hexapawn.c hexwindows.c hexerror.c menu.c instructions.c game.c ai.c history.c save_load.c -l menu -l ncurses -l panel -g -o ../hexapawn 
