#include "Board.h"

/******************************************************************************/
/* Representamos las células vivas como 'O' y las muertas como 'X' */
#define ALIVE 'O'
#define DEAD 'X'
/******************************************************************************/
/* La estructura de un juego es simplemente un tablero y la cantidad de veces
que se va a iterar */
struct _game
{
    board_t *board;
    unsigned int cycles;
};

typedef struct _game game_t;
/******************************************************************************/

/* Cargamos el juego desde un archivo */
game_t *loadGame(const char *filename);

/* Guardamos el tablero 'board' en el archivo 'filename' */
void writeBoard(board_t *board, const char *filename);

/* Argumento que toma la función board_run_row */
struct arg
{
    board_t *board;
    int row;
    int cycles;
};

/* Simulamos el Juego de la Vida de Conway con tablero 'board' la cantidad de
ciclos indicados en 'cycles' */
board_t *congwayGoL(board_t *board, unsigned int cycles);

/* La función ejecutada por los hilos que trabaja sobre la fila indicada */
void *board_run_row(void *arg);

/* Consigue el próximo valor de la célula a partir de su posición, 
fijándose en sus vecinas */
char get_next_cell_state(board_t *board, int row, int col);

/* Verifica si la célula está viva */
int is_alive(char cell);

/* Calcula el módulo para número enteros */
int mod(int a, int b);

/* Limpia la consola */
void clear_screen();