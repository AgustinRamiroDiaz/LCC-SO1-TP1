SO1-TP1
A la hora de implementar el juego de conways analizamos cuántos hilos deberíamos crear, para una implementación sencilla. Optamos generar hilos según la cantidad de filas existentes, siendo así posible tener una cota para una matriz de alrededor de 10000 hilos en un procesador (agregar nombre de los procesadores).
Para evitar los deadlocks decidimos implementar barreras, donde los hilos se esperan  en cada iteración del tablero. Cada hilo recorre una fila donde calcula los valores próximos de la matriz, y cuando todos terminan, cruzan la barrera y se actualizan.
Nuevamente hay una barrera para que todos se esperen a la hora de actualizar y comienza el próximo ciclo.
Optamos por usar punteros en todas las funciones para reducir la cantidad de memoría pasada entre las mismas.
