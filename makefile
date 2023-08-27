# makefile for Hexapawn

.DEFAULT_GOAL := build

files = hexapawn.c hexwindows.c hexmenus.c hexforms.c hexinput.c hexerror.c hexutils.c hexdata.c menu.c instructions.c game.c ai.c history.c save_load.c debug.c
incs = -I /usr/include/apr-1.0
libs = -lapr-1 -lmenu -lform -lpanel -lncurses
out = -o ../hexapawn

build:
	@echo "Building hexapawn..."
	gcc $(files) $(incs) $(libs) $(out)

debug:
	@echo "Building hexapawn with debugging symbols..."
	gcc $(files) $(incs) $(libs) -g $(out) 
