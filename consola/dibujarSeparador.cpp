// Pieza: dibujarSeparador. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <iostream>

using namespace std;

/*
    Qué hace: imprime una línea que sirve como borde superior o inferior.
    Parámetro columnas: cantidad de columnas del tablero; debe ser positiva.
    Devuelve: nada (void). Su resultado se muestra directamente en la consola.

    Cada casilla ocupa tres caracteres, igual que en dibujarTablero.
    Por ejemplo, con dos columnas se imprime "    +------+" y un salto de línea.
*/
void dibujarSeparador(int columnas) {
    // Los cuatro espacios dejan sitio para el número de fila y su separación.
    cout << "    +";

    // c empieza en 0. La condición c < columnas repite una vez por columna.
    for (int c = 0; c < columnas; c++) {
        cout << "---";
    }

    // Cierra la esquina derecha y deja la salida preparada para la siguiente fila.
    cout << "+\n";
}
