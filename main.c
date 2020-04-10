#include <stdlib.h>
#include <stdio.h>
#include "Board.h"

int main(int argc, char const *argv[])
{
    board_t *tablero;
    printf("Inicializando tablero\n");
    board_init(tablero, 3, 3);
    printf("Cargando tablero\n");
    board_load(tablero, "XXX\nOOO\nXXX");
    printf("Tablero cargado\n");
    char buff[100]; 
    printf("Mostrando tablero\n");
    board_show(*tablero, buff);
    printf("%s", buff);
    return 0;
}
