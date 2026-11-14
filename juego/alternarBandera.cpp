// Pieza: alternarBandera. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.


/*
    Qué hace: pone una bandera si la casilla no tiene, o la quita si ya tiene.

    Parámetros:
    - casilla: dirección de la casilla elegida. Debe estar oculta.
    - banderas: dirección del contador de banderas de la partida.

    Devuelve: nada (void). Cambia la casilla y el contador directamente.

    casilla->bandera es lo mismo que (*casilla).bandera: la flecha entra a
    un campo del struct a través de su dirección.
*/
void alternarBandera(Casilla* casilla, int* banderas) {
    if (casilla->bandera) {
        casilla->bandera = false;
        (*banderas)--; // Los paréntesis hacen que se reste al valor, no a la dirección.
    } else {
        casilla->bandera = true;
        (*banderas)++;
    }
}
