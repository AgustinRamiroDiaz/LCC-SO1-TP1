#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "string.h"




int main(int argc, char const *argv[])
{
    // Validamos la cantidad de parámetros ingresados
    if(argc != 2){
        fprintf(stderr,"Se debe ingresar el path del archivo game como parámetro");
        exit(1);
    }

    // Calculamos la dirección de destino
    char destinationPath[200];
    strcpy(destinationPath, argv[1]);
    int pos = strlen(destinationPath) - 4;
    strcpy(destinationPath + pos, "final");

    // Cargamos el juego del archivo
    game_t *game = loadGame(argv[1]);
    // Ejecutamos el juego
    congwayGoL(game->board, game->cycles);
    // Guardamos el juego
    writeBoard(game->board, destinationPath);
    // Liberamos la memoria utilizada
    board_destroy(game->board);
    free(game);

    return 0;
}
