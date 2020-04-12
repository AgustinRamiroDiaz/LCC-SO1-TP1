#ifndef BOARD
#define BOARD

#include <stdlib.h>

/******************************************************************************/
/* Definición de la estructura de datos del tablero */
struct _casilla
{
    char valor;
    char nuevoValor;
};

typedef struct _casilla casilla_t;

struct _board
{
    int filas;
    int columnas;
    casilla_t ***casillas;
};

typedef struct _board board_t;
/******************************************************************************/
/******************************************************************************/

/* Funciones sobre el tablero */

/* Creación del tablero */
int board_init(board_t *board, size_t row, size_t col);

/* Creación del tablero con un elemento por default*/
int board_init_def(board_t *board, size_t row, size_t col, char def);

/* Leer el tablero en una posición (col, row) */
char board_get(board_t *board, size_t row, size_t col);

/* Asignarle un valor 'val' a la posición (col, row) del tablero*/
int board_set(board_t *board, size_t row, size_t col, char val);

/* Leer de una lista de caracteres e interpretarla como un tablero */
int board_load(board_t *board, char *str);

void board_load_from_file(board_t *board, const char *nombreArchivo);

/* Función para mostrar el tablero */
/* La función 'board_show' asume que hay espacio suficiente en 'res' para alojar el tablero.*/
void board_show(board_t *board, char *res);

/* Función para mostrar el tablero por panatalla*/
void board_print(board_t *board);

/* Destroy board */
void board_destroy(board_t *board);

void board_step_cell(board_t *board, int row, int col);

void board_run(board_t *board, int cycles);

int is_alive(char cell);

#endif
