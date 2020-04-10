#include <stdlib.h>
#include <stdio.h>
#include "Board.h"

int main(int argc, char const *argv[])
{
    board_t *tablero;
    board_init(tablero, 3, 3);
    board_load(tablero, "XXX\nOOO\nXXX");
    char buff[100]; 
    board_load(tablero, buff);
    printf("%s", buff);
    return 0;
}
