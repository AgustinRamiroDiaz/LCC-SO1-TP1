all:
	gcc -o simulador  main.c Board.c Board.h Game.c Game.h -lpthread
compile:
