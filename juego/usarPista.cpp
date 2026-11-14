// Pieza: usarPista. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <cstdlib> // rand: elige la casilla de la pista.

using namespace std;

/*
    Qué hace: revela al azar una casilla segura que siga oculta y sin bandera.
    Si esa casilla es un 0, se abren sus vecinas igual que al revelar.

    Parámetros:
    - tablero: matriz de la partida.
    - filas y columnas: dimensiones de esa matriz.
    - filaPista y columnaPista: direcciones donde se escribe la posición
      elegida (desde 0), para poder avisarle al jugador cuál fue.

    Devuelve: true si se reveló una casilla, o false si no había ninguna
    disponible (por ejemplo, si todas las que faltan tienen bandera).

    Primero cuenta las casillas posibles y luego elige una por su número.
    Así nunca se queda buscando sin fin, como podría pasar probando al azar.
*/
bool usarPista(Casilla** tablero, int filas, int columnas, int* filaPista, int* columnaPista) {
    // 1. Contamos cuántas casillas pueden ser la pista.
    int candidatas = 0;
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (!tablero[f][c].esMina && !tablero[f][c].revelada && !tablero[f][c].bandera) {
                candidatas++;
            }
        }
    }

    if (candidatas == 0) {
        return false;
    }

    // 2. Elegimos al azar cuál de ellas será, contando desde 0.
    int elegida = rand() % candidatas;

    // 3. Recorremos otra vez, restando 1 en cada candidata hasta llegar a la elegida.
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (!tablero[f][c].esMina && !tablero[f][c].revelada && !tablero[f][c].bandera) {
                if (elegida == 0) {
                    *filaPista = f;
                    *columnaPista = c;
                    revelarCasilla(tablero, filas, columnas, f, c);
                    return true;
                }
                elegida--;
            }
        }
    }

    // No se llega aquí: elegida siempre es menor que candidatas.
    return false;
}
