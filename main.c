#include <stdlib.h>
#include <stdio.h>
#include "Game.h"

int main(int argc, char const *argv[])
{
    game_t *game = loadGame("Ejemplo.game");
    congwayGoL(game->board, game->cycles, 0);
    board_print(game->board);

    printf("\nFin\n");
    return 0;
}
