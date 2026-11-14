// Pieza: crearTablero. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.


/*
    Qué hace: reserva una matriz dinámica y deja todas sus casillas vacías,
    ocultas, sin banderas y con el contador de minas vecinas en cero.

    Parámetros:
    - filas: cantidad de filas que tendrá el tablero.
    - columnas: cantidad de casillas en cada fila.
    Las dos cantidades deben ser positivas; esta función no valida esos valores.

    Devuelve: un Casilla** con la dirección del arreglo de punteros a las filas.
    La memoria sigue existiendo al terminar esta función. Quien recibe el tablero
    debe llamar a liberarTablero cuando deje de usarlo.

    Casilla** significa "puntero a punteros de Casilla":

    tablero --> [puntero fila 0] --> [Casilla][Casilla][Casilla]...
                [puntero fila 1] --> [Casilla][Casilla][Casilla]...
                [puntero fila 2] --> [Casilla][Casilla][Casilla]...

    Cada fila se reserva por separado; tablero[f][c] accede a una casilla.
*/
Casilla** crearTablero(int filas, int columnas) {
    // new reserva memoria durante la ejecución, según el tamaño elegido.
    // Primer nivel: un arreglo que guarda la dirección de cada fila.
    Casilla** tablero = new Casilla*[filas];

    // f es el índice de la fila: comienza en 0 y termina en filas - 1.
    for (int f = 0; f < filas; f++) {
        // Segundo nivel: las casillas que forman esta fila.
        tablero[f] = new Casilla[columnas];

        // c recorre las columnas de esta fila, desde 0 hasta columnas - 1.
        for (int c = 0; c < columnas; c++) {
            // Asignamos un estado inicial conocido a cada campo de la casilla.
            tablero[f][c].esMina = false;
            tablero[f][c].revelada = false;
            tablero[f][c].bandera = false;
            tablero[f][c].minasVecinas = 0;
        }
    }

    // Se entrega la dirección de la matriz; no se copia toda la matriz.
    return tablero;
}
