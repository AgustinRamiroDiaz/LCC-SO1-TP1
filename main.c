#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "string.h"




int main(int argc, char const *argv[])
{
    if(argc != 2){
        fprintf(stderr,"Se debe ingresar el path del archivo game como parámetro");
        exit(1);
    }

    char destinationPath[200];
    strcpy(destinationPath, argv[1]);
    int pos = strlen(destinationPath) - 4;
    strcpy(destinationPath + pos, "final");

    game_t *game = loadGame(argv[1]);
    congwayGoL(game->board, game->cycles);
    writeBoard(game->board, destinationPath);
    board_destroy(game->board);
    free(game);

    return 0;
}
