// Pieza: revelarCasilla. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.


/*
    Qué hace: revela una casilla segura. Si no tiene minas vecinas (0),
    también revela sus 8 vecinas, y así en cadena.

    Parámetros:
    - tablero: matriz con las minas y los números ya calculados.
    - filas y columnas: dimensiones de esa matriz.
    - f y c: posición que se quiere revelar, con índices desde cero.
      Puede estar fuera del tablero: en ese caso no se hace nada.

    Devuelve: nada (void). Cambia revelada en las casillas del tablero.

    Esta función se llama a sí misma (recursión). Cada llamada revela una
    casilla, y la cadena se detiene al llegar a casillas con número, al borde,
    a una bandera o a casillas ya reveladas.
    No revela minas: eso lo decide jugarPartida antes de llamarla.
*/
void revelarCasilla(Casilla** tablero, int filas, int columnas, int f, int c) {
    // Fuera del tablero no hay casilla que revelar.
    if (f < 0 || f >= filas || c < 0 || c >= columnas) {
        return;
    }

    // Una casilla revelada, con bandera o con mina se deja como está.
    // Revisar "revelada" evita que dos vecinas se llamen entre sí sin fin.
    if (tablero[f][c].revelada || tablero[f][c].bandera || tablero[f][c].esMina) {
        return;
    }

    tablero[f][c].revelada = true;

    // Solo un 0 abre a sus vecinas: alrededor de un 0 no puede haber minas.
    if (tablero[f][c].minasVecinas == 0) {
        for (int filaVecina = f - 1; filaVecina <= f + 1; filaVecina++) {
            for (int columnaVecina = c - 1; columnaVecina <= c + 1; columnaVecina++) {
                revelarCasilla(tablero, filas, columnas, filaVecina, columnaVecina);
            }
        }
    }
}
