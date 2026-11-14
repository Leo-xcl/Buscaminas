// Pieza: colocarMinas. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <cstdlib> // rand: obtiene las posiciones aleatorias.

using namespace std;

/*
    Qué hace: elige posiciones aleatorias y coloca la cantidad de minas solicitada,
    sin contar dos veces una misma casilla.

    Parámetros:
    - tablero: matriz recién inicializada, con esMina en false en todas las casillas.
    - filas y columnas: dimensiones positivas de esa matriz.
    - minas: cantidad que se desea colocar, entre 0 y filas * columnas.
    Estas condiciones deben cumplirse antes de llamar a la función; aquí no se validan.
    elegirDificultad limita las minas para dejar al menos una casilla segura.

    Devuelve: nada (void). Modifica esMina directamente en la matriz recibida.
    No cambia revelada, bandera ni minasVecinas; los números se calculan después.
    srand se llama una sola vez en main para iniciar la secuencia aleatoria.
*/
void colocarMinas(Casilla** tablero, int filas, int columnas, int minas) {
    // Cuenta las minas nuevas que ya se colocaron, no los intentos realizados.
    int colocadas = 0;

    // Si minas es 0, la condición es falsa desde el inicio y no se coloca ninguna.
    while (colocadas < minas) {
        // El residuo (%) produce índices entre 0 y el tamaño menos uno.
        int f = rand() % filas;
        int c = rand() % columnas;

        // ! significa "no": entramos solo si la casilla todavía no contiene una mina.
        // Si ya tenía una, el contador no cambia y el while intenta otra posición.
        if (!tablero[f][c].esMina) {
            tablero[f][c].esMina = true;
            colocadas++;
        }
    }
}
