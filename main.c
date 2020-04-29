#include <stdlib.h>
#include <stdio.h>
#include "Game.h"

int main(int argc, char const *argv[])
{
    if(argc != 2){
        fprintf(stderr,"Se debe ingresar el path del archivo game únicamente");
        exit(1);
    }

    game_t *game = loadGame(argv[1]);
    congwayGoL(game->board, game->cycles);
    writeBoard(game->board, "Resultado.game");
    board_destroy(game->board);
    free(game);
    
    return 0;
}
