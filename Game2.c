#include "Game.h"
#include <stdio.h>
#include <pthread.h>
#include <time.h>

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
    char boardStr[board->filas * (board->columnas + 1)];
    board_show(board, boardStr);
    FILE *archivo = fopen(filename, "w");
    fputs(boardStr, archivo);
    fclose(archivo);
}

pthread_barrier_t barrera;
struct arg
{
    board_t *board;
    int row;
    int col;
    int cycles;
};

imprimirFila(struct arg **argumento){
    for (size_t i = 0; i < 10; i++)
    {
        printf("\n%d,%d,%d\n",argumento[i]->row,argumento[i]->col,argumento[i]->cycles);
    }
    
}

/* Simulamos el Juego de la Vida de Conway con tablero 'board' la cantidad de
ciclos indicados en 'cycles' en 'nuprocs' unidades de procesamiento*/
void congwayGoL(board_t *board, unsigned int cycles, const int nuproc)
{
    pthread_barrier_init(&barrera, NULL, board->filas);
    pthread_t hilos[board->filas];
    // inicializamos los hilos
    for (size_t fila = 0; fila < board->filas; fila++)
    {
        struct arg **argumento = malloc(sizeof(struct arg*)*board->columnas);
        for (size_t columna = 0; columna < board->columnas; columna++)
        {
            argumento[columna] = malloc(sizeof(struct arg));
            // crear hilos
            argumento[columna]->board = board;
            argumento[columna]->row = fila;
            argumento[columna]->col = columna;
            argumento[columna]->cycles = cycles;
        }
        //imprimirFila(argumento);
        pthread_create(&hilos[fila], NULL, board_run_cell, (void*)argumento);
    }

    //pthread_create(&hiloBarrera, NULL, board_print_on_barrier, (void *)board);

    // esperamos los hilos
    for (size_t fila = 0; fila < board->filas; fila++)
    {
        pthread_join((hilos[fila]), NULL);
    }
    board_print(board);
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
        sleep(1);
    }
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
    struct arg **argumento = ((struct arg **)arg);
    imprimirFila(argumento);
    
    for (size_t i = 0; i < 5; i++) //ciclos
    {
        for (size_t j = 0; j < 10; j++) //columna
        {
        board_step_cell(argumento[j]->board, [j]->row, argumento[j]->col);          
        }
    }
}

int is_alive(char cell)
{
    return cell == ALIVE;
}
