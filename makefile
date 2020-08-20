# makefile for Hexapawn

.DEFAULT_GOAL := build

build:
	@echo "Building hexapawn..."
	gcc hexapawn.c hexwindows.c -l ncurses -l panel -o ../hexapawn
