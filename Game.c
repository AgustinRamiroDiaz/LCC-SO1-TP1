#include "Game.h"
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#define MAXHILOS 5000

/* Cargamos el juego desde un archivo */
game_t *loadGame(const char *filename)
{
    FILE *archivo = fopen(filename, "r");
    char buff[10];

    fscanf(archivo, "%s", buff);
    int ciclos = atoi(buff);

    fscanf(archivo, "%s", buff);
    int filas = atoi(buff);

    fscanf(archivo, "%s", buff);
    int columnas = atoi(buff);
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

sem_t semaforo;
pthread_barrier_t barrera;
struct arg
{
    board_t *board;
    int row;
    int col;
    int cycles;
};

/* Simulamos el Juego de la Vida de Conway con tablero 'board' la cantidad de
ciclos indicados en 'cycles' en 'nuprocs' unidades de procesamiento*/
board_t *congwayGoL(board_t *board, unsigned int cycles, const int nuproc)
{
    if(nuproc < MAXHILOS){
        sem_init(&semaforo, 0, nuproc);
        printf("\nBien wacho\n");
    }
    else{
        sem_init(&semaforo, 0, MAXHILOS);
        printf("\nExcedido\n");
    }
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
            sem_wait(&semaforo);
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
            nuevoValor = DEAD;
        }
    }
    else
    {
        if (vecinasVivas == 3)
        {
            nuevoValor = ALIVE;
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
        //sleep();
    }
    sem_post(&semaforo);
    pthread_barrier_wait(&barrera);
    board_set(board, row, col, nuevoValor);
    pthread_barrier_wait(&barrera);
}

void clear_screen()
{
    printf("\e[1;1H\e[2J");
}

void *board_run_cell(void *arg)
{
    struct arg argumento = *((struct arg *)arg);
    for (size_t i = 0; i < argumento.cycles; i++)
    {
        board_step_cell(argumento.board, argumento.row, argumento.col);
    }
}

int is_alive(char cell)
{
    return cell == ALIVE;
}
