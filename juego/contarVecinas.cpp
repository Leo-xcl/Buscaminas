// Pieza: contarVecinas. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.


/*
    Qué hace: cuenta las minas alrededor de una casilla sin modificar el tablero.

    Parámetros:
    - tablero: matriz válida cuyas minas ya se colocaron.
    - filas y columnas: dimensiones de esa matriz.
    - f y c: fila y columna de la casilla que se consulta, con índices desde cero.
      Deben cumplir 0 <= f < filas y 0 <= c < columnas.

    Devuelve: un entero entre 0 y 8 con la cantidad de minas vecinas.
    Revisa el cuadrado de 3 x 3 centrado en [f][c] y excluye la propia casilla.
    En un borde o una esquina solo considera las posiciones dentro del tablero.
*/
int contarVecinas(Casilla** tablero, int filas, int columnas, int f, int c) {
    int cantidad = 0;

    // El primer ciclo visita la fila anterior, la actual y la siguiente.
    for (int filaVecina = f - 1; filaVecina <= f + 1; filaVecina++) {
        // En cada fila se revisan también la columna anterior, la actual y la siguiente.
        for (int columnaVecina = c - 1; columnaVecina <= c + 1; columnaVecina++) {
            // Comprobamos los límites ANTES de acceder a la matriz.
            // Así las esquinas y los bordes no consultan memoria ajena.
            if (filaVecina >= 0 && filaVecina < filas &&
                columnaVecina >= 0 && columnaVecina < columnas) {
                // || significa "o": basta que una coordenada sea distinta.
                // Solo se excluye el centro, donde ambas coordenadas son iguales.
                if (filaVecina != f || columnaVecina != c) {
                    if (tablero[filaVecina][columnaVecina].esMina) {
                        // Cada vecina se visita una vez; una mina suma una unidad.
                        cantidad++;
                    }
                }
            }
        }
    }

    return cantidad;
}
