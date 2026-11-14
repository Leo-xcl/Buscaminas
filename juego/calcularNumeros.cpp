// Pieza: calcularNumeros. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.


/*
    Qué hace: completa el campo minasVecinas después de colocar todas las minas.

    Parámetros:
    - tablero: matriz válida con las posiciones de las minas ya definidas.
    - filas y columnas: dimensiones de esa matriz.

    Devuelve: nada (void). Guarda los resultados directamente en cada casilla.
    En las casillas seguras escribe el conteo real. En las minas escribe 0
    porque el dibujo muestra un asterisco en lugar de un número; ese 0 no significa
    que la mina carezca de otras minas a su alrededor.
*/
void calcularNumeros(Casilla** tablero, int filas, int columnas) {
    // Los dos ciclos recorren todas las casillas: una fila y sus columnas a la vez.
    // Los índices comienzan en 0 y nunca alcanzan filas ni columnas.
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (tablero[f][c].esMina) {
                tablero[f][c].minasVecinas = 0;
            } else {
                // contarVecinas devuelve el número que corresponde a esta posición.
                tablero[f][c].minasVecinas =
                    contarVecinas(tablero, filas, columnas, f, c);
            }
        }
    }
}
