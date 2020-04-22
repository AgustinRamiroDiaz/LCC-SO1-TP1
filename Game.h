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

/* Simulamos el Juego de la Vida de Conway con tablero 'board' la cantidad de
ciclos indicados en 'cycles' en 'nuprocs' unidades de procesamiento*/
board_t *congwayGoL(board_t *board, unsigned int cycles);

void clear_screen();

int is_alive(char cell);

void *board_run_row(void *arg);

void board_step_cell(board_t *board, int row, int col);

char get_next_cell_state(board_t *board, int row, int col);

int mod(int a, int b);
