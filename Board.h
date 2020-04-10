#ifndef BOARD
#define BOARD

#include <stdlib.h>

/******************************************************************************/
/* Definición de la estructura de datos del tablero */
struct _casilla
{
    char valor;
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
int board_init(board_t *board, size_t col, size_t row);

/* Creación del tablero con un elemento por default*/
int board_init_def(board_t *board, size_t col, size_t row, char def);

/* Leer el tablero en una posición (col, row) */
char board_get(board_t board, size_t col, size_t row);

/* Asignarle un valor 'val' a la posición (col, row) del tablero*/
int board_set(board_t board, size_t col, size_t row, char val);

/* Leer de una lista de caracteres e interpretarla como un tablero */
int board_load(board_t *board, char *str);

/* Función para mostrar el tablero */
/* La función 'board_show' asume que hay espacio suficiente en 'res' para alojar el tablero.*/
void board_show(board_t board, char *res);

/* Destroy board */
void board_destroy(board_t *board);
#endif
