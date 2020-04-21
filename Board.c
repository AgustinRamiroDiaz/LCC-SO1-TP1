#include "Board.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

pthread_barrier_t barrera;

/* Creación del tablero */
int board_init(board_t *board, size_t row, size_t col)
{
    board->filas = row;
    board->columnas = col;
    board->casillas = malloc(sizeof(casilla_t **) * row);
    for (size_t fila = 0; fila < row; fila++)
    {
        board->casillas[fila] = malloc(sizeof(casilla_t *) * col);
        for (size_t columna = 0; columna < col; columna++)
        {
            board->casillas[fila][columna] = malloc(sizeof(casilla_t));
        }
    }
    return 0;
}

/* Creación del tablero con un elemento por default*/
int board_init_def(board_t *board, size_t row, size_t col, char def)
{
    board_init(board, row, col);
    for (size_t fila = 0; fila < row; fila++)
    {
        for (size_t columna = 0; columna < col; columna++)
        {
            board->casillas[fila][columna]->valor = def;
        }
    }
    return 0;
}

/* Leer el tablero en una posición (col, row) */
char board_get(board_t *board, size_t row, size_t col)
{
    return board->casillas[row][col]->valor;
}

/* Asignarle un valor 'val' a la posición (col, row) del tablero*/
int board_set(board_t *board, size_t row, size_t col, char val)
{
    board->casillas[row][col]->valor = val;
    return 0;
}

/* Leer de una lista de caracteres e interpretarla como un tablero */
int board_load(board_t *board, char *str)
{
    int fila = 0, columna = 0;
    for (size_t posicion = 0; posicion < strlen(str); posicion++)
    {
        if (str[posicion] == '\n')
        {
            fila++;
            columna = 0;
        }
        else
        {
            board->casillas[fila][columna]->valor = str[posicion];
            columna++;
        }
    }
    return 0;
}

/* Función para mostrar el tablero */
/* La función 'board_show' asume que hay espacio suficiente en 'res' para alojar el tablero.*/
void board_show(board_t *board, char *res)
{
    for (size_t fila = 0; fila < board->filas; fila++)
    {
        size_t columna;
        for (columna = 0; columna < board->columnas; columna++)
        {
            res[fila * (board->columnas + 1) + columna] = board->casillas[fila][columna]->valor;
        }
        res[fila * (board->columnas + 1) + columna] = '\n';
    }
    res[board->filas * (board->columnas + 1)] = '\0';
}

/* Función para mostrar el tablero por panatalla*/
void board_print(board_t *board)
{
    for (size_t fila = 0; fila < board->filas; fila++)
    {
        for (size_t columna = 0; columna < board->columnas; columna++)
        {
            printf("%c", board->casillas[fila][columna]->valor);
        }
        printf("\n");
    }
    printf("\n");
}

/* Destroy board */
void board_destroy(board_t *board)
{
    for (size_t fila = 0; fila < board->filas; fila++)
    {
        for (size_t columna = 0; columna < board->columnas; columna++)
        {
            free(board->casillas[fila][columna]);
        }
        free(board->casillas[fila]);
    }
    free(board->casillas);
    free(board);
}
