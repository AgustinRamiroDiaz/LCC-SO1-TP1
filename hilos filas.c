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
        pthread_create(&hilos[fila], NULL, board_run_cell, argumento);
    }

    // pthread_create(&hiloBarrera, NULL, board_print_on_barrier, (void *)board);

    // esperamos los hilos
    for (size_t fila = 0; fila < board->filas; fila++)
    {
        pthread_join((hilos[fila]), NULL);
    }
}