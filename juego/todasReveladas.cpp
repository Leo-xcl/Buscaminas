// Pieza: todasReveladas. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.


/*
    Qué hace: revisa si el jugador ya ganó.

    Parámetros:
    - tablero: matriz de la partida.
    - filas y columnas: dimensiones de esa matriz.

    Devuelve: true si todas las casillas SIN mina están reveladas.
    Las minas no importan: pueden estar ocultas, con bandera o pisadas.
*/
bool todasReveladas(Casilla** tablero, int filas, int columnas) {
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            // Basta una casilla segura oculta para saber que falta.
            if (!tablero[f][c].esMina && !tablero[f][c].revelada) {
                return false;
            }
        }
    }
    return true;
}
