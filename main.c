#include <stdlib.h>
#include <stdio.h>
#include "Game.h"

int main(int argc, char const *argv[])
{
    game_t *game = loadGame("Ejemplo.game");
    congwayGoL(game->board, game->cycles);
    writeBoard(game->board, "Resultado.game");
    return 0;
}
