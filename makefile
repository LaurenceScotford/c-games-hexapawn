# makefile for Hexapawn

.DEFAULT_GOAL := build

files = hexapawn.c hexwindows.c hexerror.c hexutils.c hexdata.c menu.c instructions.c game.c ai.c history.c save_load.c
incs = -I /usr/include/apr-1.0
libs = -lapr-1 -lmenu -lncurses -lpanel
out = -o ../hexapawn

build:
	@echo "Building hexapawn..."
	gcc $(files) $(incs) $(libs) $(out)

debug:
	@echo "Building hexapawn with debugging symbols..."
	gcc $(files) $(inc) $(libs) -g $(out) 
