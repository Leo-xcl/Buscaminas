// Pieza: mostrarMinas. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.


/*
    Qué hace: marca todas las minas como reveladas para enseñarlas al final.

    Parámetros:
    - tablero: matriz de la partida.
    - filas y columnas: dimensiones de esa matriz.

    Devuelve: nada (void). Solo cambia revelada en las casillas con mina;
    después dibujarTablero las muestra con *.
*/
void mostrarMinas(Casilla** tablero, int filas, int columnas) {
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (tablero[f][c].esMina) {
                tablero[f][c].revelada = true;
            }
        }
    }
}
