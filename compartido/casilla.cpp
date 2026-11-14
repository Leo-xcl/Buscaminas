// Este archivo comparte el tipo Casilla entre los módulos.
// La protección evita definir el struct dos veces al incluir este archivo varias veces.
#ifndef BUSCAMINAS_CASILLA_CPP
#define BUSCAMINAS_CASILLA_CPP

/*
    Casilla es un struct: agrupa en un solo tipo los datos de una posición.
    Cada elemento tablero[f][c] tiene su propia copia de estos cuatro campos.
    f representa la fila y c la columna, ambas contadas desde cero.

    bool guarda true (verdadero) o false (falso); int guarda un número entero.
    La definición del struct no crea el tablero ni inicializa sus casillas.
    crearTablero reserva la memoria y les asigna sus valores iniciales.
    Se conservan estos nombres para integrar la segunda mitad del proyecto.
*/
struct Casilla {
    bool esMina;       // true si esta casilla contiene una mina.
    bool revelada;     // true si el jugador ya puede ver su contenido.
    bool bandera;      // true si el jugador colocó una bandera aquí.
    int minasVecinas;  // De 0 a 8 en casillas seguras; en minas se deja en 0 y no se usa.
};

#endif // BUSCAMINAS_CASILLA_CPP
