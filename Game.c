#include "Game.h"
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

/* Cargamos el juego desde un archivo */
game_t *loadGame(const char *filename)
{
    FILE *archivo = fopen(filename, "r");
    char buff[10];
    int ciclos,filas,columnas;
    fscanf(archivo, "%d", &ciclos);
    fscanf(archivo, "%d", &filas);
    fscanf(archivo, "%d", &columnas);

    // salteamos el \n
    getc(archivo);
    char buffer[filas * (columnas + 1)];
    char c;
    int contador = 0;
    while ((c = getc(archivo)) != EOF)
    {
        buffer[contador++] = c;
    }
    buffer[contador] = '\0';
    fclose(archivo);

    board_t *board = malloc(sizeof(board_t));
    board_init(board, filas, columnas);
    board_load(board, buffer);

    game_t *game = malloc(sizeof(game_t));
    game->board = board;
    game->cycles = ciclos;

    return game;
}

/* Guardamos el tablero 'board' en el archivo 'filename' */
void writeBoard(board_t *board, const char *filename)
{
    char boardStr[board->filas * (board->columnas + 1) + 1];
    board_show(board, boardStr);
    FILE *archivo = fopen(filename, "w");
    fputs(boardStr, archivo);
    fclose(archivo);
}

pthread_barrier_t barrera;

/* Simulamos el Juego de la Vida de Conway con tablero 'board' la cantidad de
ciclos indicados en 'cycles' en 'nuprocs' unidades de procesamiento*/
board_t *congwayGoL(board_t *board, unsigned int cycles)
{
    pthread_barrier_init(&barrera, NULL, board->filas);
    pthread_t hilos[board->filas];
    // inicializamos los hilos
    for (size_t fila = 0; fila < board->filas; fila++)
    {
        struct arg *argumento = malloc(sizeof(struct arg));
        // crear hilos
        argumento->board = board;
        argumento->row = fila;
        argumento->cycles = cycles;

        pthread_create(&hilos[fila], NULL, board_run_row, argumento);
    }

    // esperamos los hilos
    for (size_t fila = 0; fila < board->filas; fila++)
    {
        pthread_join((hilos[fila]), NULL);
    }
}

/* Calculamos la cantidad de vecinas vivas y de ahí se decide si el estado de la célula es viva o muerta*/
char get_next_cell_state(board_t *board, int row, int col)
{
    char valor = board_get(board, row, col);
    int vecinasVivas = 0;

    if (is_alive(board_get(board, mod(row + -1, board->filas), mod(col + -1, board->columnas))))
        vecinasVivas++;
    if (is_alive(board_get(board, mod(row + -1, board->filas), mod(col + 0, board->columnas))))
        vecinasVivas++;
    if (is_alive(board_get(board, mod(row + -1, board->filas), mod(col + 1, board->columnas))))
        vecinasVivas++;

    if (is_alive(board_get(board, mod(row + 0, board->filas), mod(col + -1, board->columnas))))
        vecinasVivas++;
    if (is_alive(board_get(board, mod(row + 0, board->filas), mod(col + 1, board->columnas))))
        vecinasVivas++;

    if (is_alive(board_get(board, mod(row + 1, board->filas), mod(col + -1, board->columnas))))
        vecinasVivas++;
    if (is_alive(board_get(board, mod(row + 1, board->filas), mod(col + 0, board->columnas))))
        vecinasVivas++;
    if (is_alive(board_get(board, mod(row + 1, board->filas), mod(col + 1, board->columnas))))
        vecinasVivas++;

    if (!is_alive(valor) && vecinasVivas == 3)
        valor = ALIVE;
    else if (is_alive(valor) && vecinasVivas != 2 && vecinasVivas != 3)
        valor = DEAD;

    return valor;
}

/*Función con que trabajan los hilos*/
void *board_run_row(void *arg)
{
    struct arg *argumento = (struct arg *)arg;
    char listaValores[argumento->board->columnas];
    for (size_t i = 0; i < argumento->cycles; i++)
    {
        for (size_t columna = 0; columna < argumento->board->columnas; columna++)
        {
            /* code */
            listaValores[columna] = get_next_cell_state(argumento->board, argumento->row, columna);
        }
        pthread_barrier_wait(&barrera);
        for (size_t columna = 0; columna < argumento->board->columnas; columna++)
        {
            /* code */
            board_set(argumento->board, argumento->row, columna, listaValores[columna]);
        }
        pthread_barrier_wait(&barrera);

        // Descomentar si se quiere ver la animación
        // if (argumento->row == 0)
        // {
        //     clear_screen();
        //     board_print(argumento->board);
        //     usleep(.5 * 1000000);
        // }
        // pthread_barrier_wait(&barrera);
    }
}

/*Devuelve si una célula esta viva o muerta*/
int is_alive(char cell)
{
    return cell == ALIVE;
}

/*Calcula el módulo*/
int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

/*Limpia la consola*/
void clear_screen()
{
    printf("\e[1;1H\e[2J");
}