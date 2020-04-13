#include "Board.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

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

int board_load_from_file(board_t *board, const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    char buffer[10000];
    int ciclos, filas, columnas;
    if (1 == fscanf(archivo, "%s", buffer))
    {
        ciclos = atoi(buffer);
    }

    if (1 == fscanf(archivo, "%s", buffer))
    {
        filas = atoi(buffer);
    }

    if (1 == fscanf(archivo, "%s", buffer))
    {
        columnas = atoi(buffer);
    }

    // salteamos el \n
    getc(archivo);
    char c;
    int contador = 0;
    while ((c = getc(archivo)) != EOF)
    {
        buffer[contador++] = c;
    }
    buffer[contador] = '\0';
    fclose(archivo);

    board_init(board, filas, columnas);
    board_load(board, buffer);
    return ciclos;
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
    res[(board->filas + 1) * (board->columnas + 1)] = '\0';
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

void board_step_cell(board_t *board, int row, int col)
{
    char viejoValor = board_get(board, row, col);
    char vecina;
    int vecinasVivas = 0;
    for (int offset = -1; offset < 2; offset++)
    {
        // vecinas de abajo
        vecina = board_get(board,
                           (row + 1) % board->filas,
                           (col + board->columnas + offset) % board->columnas);
        if (is_alive(vecina))
        {
            vecinasVivas++;
        }
        // vecinas de arriba
        vecina = board_get(board,
                           (row + board->filas + -1) % board->filas,
                           (col + board->columnas + offset) % board->columnas);
        if (is_alive(vecina))
        {
            vecinasVivas++;
        }
    }

    // vecina de la izquierda
    vecina = board_get(board,
                       row,
                       (col + board->columnas + -1) % board->columnas);

    if (is_alive(vecina))
    {
        vecinasVivas++;
    }
    // vecina de la derecha
    vecina = board_get(board,
                       row,
                       (col + 1) % board->columnas);

    if (is_alive(vecina))
    {
        vecinasVivas++;
    }

    char nuevoValor;
    if (is_alive(viejoValor))
    {
        if (vecinasVivas == 2 || vecinasVivas == 3)
        {
            nuevoValor = viejoValor;
        }
        else
        {
            nuevoValor = 'X';
        }
    }
    else
    {
        if (vecinasVivas == 3)
        {
            nuevoValor = 'O';
        }
        else
        {
            nuevoValor = viejoValor;
        }
    }

    // wait for barrier
    pthread_barrier_wait(&barrera);

    if (row == 0 && col == 0)
    {
        clear_screen();
        board_print(board);
        //sleep(1);
    }

    pthread_barrier_wait(&barrera);

    board_set(board, row, col, nuevoValor);

    pthread_barrier_wait(&barrera);
}

void clear_screen()
{
    printf("\e[1;1H\e[2J");
}

struct arg
{
    board_t *board;
    int row;
    int col;
    int cycles;
};

void board_run(board_t *board, int cycles)
{
    pthread_barrier_init(&barrera, NULL, board->filas * board->columnas);
    pthread_t hilos[board->filas][board->columnas];
    // inicializamos los hilos
    for (size_t fila = 0; fila < board->filas; fila++)
    {
        for (size_t columna = 0; columna < board->columnas; columna++)
        {
            // crear hilos
            struct arg *argumento = malloc(sizeof(struct arg));
            argumento->board = board;
            argumento->row = fila;
            argumento->col = columna;
            argumento->cycles = cycles;
            pthread_create(&hilos[fila][columna], NULL, board_run_cell, argumento);
        }
    }

    // pthread_create(&hiloBarrera, NULL, board_print_on_barrier, (void *)board);

    // esperamos los hilos
    for (size_t fila = 0; fila < board->filas; fila++)
    {
        for (size_t columna = 0; columna < board->columnas; columna++)
        {
            pthread_join((hilos[fila][columna]), NULL);
        }
    }
}

void *board_run_cell(void *arg) // board_t *board, int row, int col, int cycles)
{
    struct arg argumento = *((struct arg *)arg);
    for (size_t i = 0; i < argumento.cycles; i++)
    {
        board_step_cell(argumento.board, argumento.row, argumento.col);
    }
}

int is_alive(char cell)
{
    return cell == 'O';
}