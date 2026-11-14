// Pieza: mostrarTitulo. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <iostream>

using namespace std;

/*
    Qué hace: imprime el nombre del proyecto dentro de un marco de texto
    y los nombres de quienes lo programaron.
    Parámetros: ninguno; el título siempre es el mismo.
    Devuelve: nada, porque es una función void.
    Efecto: escribe en la consola y deja su color en 7 (gris) al terminar.
*/
void mostrarTitulo() {
    // El color 11 resalta el marco y el nombre con un tono cian claro.
    cambiarColor(11);
    cout << "\n+--------------------------------------+\n";
    cout << "|              BUSCAMINAS              |\n";
    cout << "+--------------------------------------+\n";
    // Restauramos el color antes de los nombres y de los mensajes siguientes.
    cambiarColor(7);
    cout << "Por: [Nombre 1] y [Nombre 2]\n\n";
}
