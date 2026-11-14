// Pieza: leerAccion. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <iostream>
#include <cctype> // toupper: convierte una letra a mayúscula.

using namespace std;

/*
    Qué hace: lee la letra de la acción que quiere hacer el jugador.
    Parámetros: ninguno. El mensaje se imprime antes de llamarla.
    Devuelve: 'R' para revelar, 'B' para bandera o 'P' para pista.
    Devuelve 0 si ya no se puede leer (EOF), igual que leerEnteroEnRango.

    Acepta minúsculas: "r" se convierte en 'R'.
    Si se escribe otra letra, vuelve a pedirla.
*/
char leerAccion() {
    while (true) {
        char letra;
        cin >> letra;

        // Si la lectura falló, la entrada se cerró y no hay más que leer.
        if (cin.fail()) {
            return 0;
        }

        // Descartamos lo que quede en la línea, por ejemplo "R 3 4".
        cin.ignore(1000, '\n');

        letra = toupper(letra);
        if (letra == 'R' || letra == 'B' || letra == 'P') {
            return letra;
        }

        cout << "Acción inválida. Escribe R, B o P: ";
    }
}
