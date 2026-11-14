// Pieza: calcularNivel. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.


/*
    Qué hace: averigua qué dificultad fija corresponde a una configuración.

    Parámetros: filas, columnas y minas de la partida.

    Devuelve: 1 = Fácil, 2 = Medio, 3 = Difícil, o 0 si no coincide con
    ninguna (Personalizado). Los récords solo comparan partidas del mismo
    nivel, porque no es justo comparar un 9 x 9 con un 16 x 30.
    Usa los mismos valores que elegirDificultad.
*/
int calcularNivel(int filas, int columnas, int minas) {
    if (filas == 9 && columnas == 9 && minas == 10) {
        return 1;
    }
    if (filas == 16 && columnas == 16 && minas == 40) {
        return 2;
    }
    if (filas == 16 && columnas == 30 && minas == 99) {
        return 3;
    }
    return 0;
}
