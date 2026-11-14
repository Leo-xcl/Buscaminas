// Pieza: dibujarTablero. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <iostream>
#include <iomanip> // setw: alinea casillas y coordenadas.

using namespace std;

/*
    Qué hace: imprime el tablero y un panel con los contadores de la partida.
    Parámetros:
      tablero: dirección de una matriz válida de casillas.
      filas y columnas: dimensiones con las que se creó esa matriz.
      vidas: vidas que se quieren mostrar en el panel.
      banderas: cantidad de banderas colocadas, no las que quedan disponibles.
      minas: cantidad total de minas de ese tablero.
      jugadas: contador de acciones que se quiere mostrar.
    Devuelve: nada (void). Imprime en la consola y deja el color normal al salir.

    No modifica las casillas ni calcula los contadores; recibe los valores ya
    preparados. La lógica de vidas y jugadas será de la segunda mitad.
    Se debe llamar a calcularNumeros antes de mostrar casillas reveladas.
    minasVecinas debe estar entre 0 y 8 para poder usar el arreglo de colores.
*/
void dibujarTablero(Casilla** tablero, int filas, int columnas,
                    int vidas, int banderas, int minas, int jugadas) {
    // La posición del arreglo es el número de minas vecinas (0 a 8).
    int colores[9] = {7, 9, 10, 12, 13, 14, 11, 15, 8};

    // Primero mostramos el panel en blanco; después restauramos el color gris.
    cambiarColor(15);
    cout << "\nVidas: " << vidas << "  Banderas: " << banderas
         << "  Minas: " << minas << "  Jugadas: " << jugadas << "\n\n";
    cambiarColor(7);

    // El espacio inicial alinea las coordenadas con las casillas, no con las filas.
    cout << "     ";
    for (int c = 0; c < columnas; c++) {
        // setw(3) da al siguiente valor un ancho mínimo de tres caracteres.
        cout << setw(3) << c + 1; // Coordenadas visibles desde 1.
    }
    cout << "\n";
    dibujarSeparador(columnas);

    // El ciclo exterior elige una fila y el interior recorre todas sus columnas.
    for (int f = 0; f < filas; f++) {
        cout << setw(3) << f + 1 << " |";

        for (int c = 0; c < columnas; c++) {
            // Una casilla oculta muestra B o #, aunque debajo exista una mina.
            // ! significa "no": entramos aquí si revelada es false.
            if (!tablero[f][c].revelada) {
                if (tablero[f][c].bandera) {
                    cambiarColor(14);
                    cout << setw(3) << 'B';
                } else {
                    cambiarColor(8);
                    cout << setw(3) << '#';
                }
            } else if (tablero[f][c].esMina) {
                // Este caso solo se alcanza si la casilla ya está revelada.
                cambiarColor(12);
                cout << setw(3) << '*';
            } else {
                // Es una casilla segura y revelada: el número elige su color.
                int numero = tablero[f][c].minasVecinas;
                cambiarColor(colores[numero]);
                if (numero == 0) {
                    // El punto distingue una casilla vacía de una todavía oculta.
                    cout << setw(3) << '.';
                } else {
                    cout << setw(3) << numero;
                }
            }
            // Evita que el color de esta casilla pase al marco o a la siguiente.
            cambiarColor(7);
        }

        cout << "|\n"; // Cierra la fila y pasa a la siguiente línea.
    }

    // Cerramos el marco inferior y recordamos el significado de los símbolos.
    dibujarSeparador(columnas);
    cout << "     # Oculta   B Bandera   * Mina   . Sin minas vecinas\n";
}
