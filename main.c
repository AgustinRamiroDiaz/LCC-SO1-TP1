#include <stdlib.h>
#include <stdio.h>
#include "Board.h"

int main(int argc, char const *argv[])
{
    board_t tablero;
    // printf("Inicializando tablero\n");
    // board_init_def(&tablero, 2, 5, 'X');
    // printf("filas: %d \n columnas: %d \n", tablero.filas, tablero.columnas);
    // printf("Imprimiendo tablero\n");
    // board_print(tablero);
    // printf("Cargando tablero\n");
    // board_load(tablero, "XXX\nOOO\nXXX");
    // printf("Tablero cargado\n");
    // char buff[100]; 
    // printf("Mostrando tablero\n");
    // board_show(tablero, buff);
    // printf("Tablero\n");

    // printf("%s", buff);

    int ciclos = board_load_from_file(&tablero, "Ejemplo.game");
    board_run(&tablero, ciclos);
    board_print(&tablero);
    
    
    printf("Fin");
    return 0;
}
