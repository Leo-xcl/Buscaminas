// Pieza: liberarTablero. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.


/*
    Qué hace: devuelve al sistema la memoria reservada por crearTablero.

    Parámetros:
    - tablero: dirección de una matriz creada con crearTablero y aún no liberada.
    - filas: el mismo número de filas que se usó al reservar esa matriz.

    Devuelve: nada, porque su tipo de retorno es void.
    Primero libera cada fila y después el arreglo que guardaba sus direcciones.
    Debe llamarse una sola vez por tablero: después no se puede acceder a él
    ni volver a liberarlo. El puntero del main no se pone en cero automáticamente.
*/
void liberarTablero(Casilla** tablero, int filas) {
    // Cada fila tuvo su propio new[], por eso necesita su propio delete[].
    for (int f = 0; f < filas; f++) {
        delete[] tablero[f]; // [] porque cada fila fue creada con new[].
    }
    // El arreglo de punteros se libera al final: antes lo usamos para hallar las filas.
    delete[] tablero;
}
