// Pieza: guardarRecord. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <fstream> // ofstream: escribe en un archivo de texto.
#include <string>

using namespace std;

/*
    Qué hace: agrega una partida ganada al final de records.txt.

    Parámetros:
    - nombre: nombre del jugador (puede tener espacios).
    - nivel: 1, 2 o 3, como lo devuelve calcularNivel.
    - jugadas: cuántas jugadas necesitó para ganar.

    Devuelve: nada (void).

    Cada récord ocupa una línea: nivel, jugadas y nombre. Por ejemplo:
        1 23 Ana López
    El nombre va al final porque puede tener espacios; así se lee con getline.
    Si records.txt no existe, se crea en la carpeta desde donde se ejecuta el juego.
*/
void guardarRecord(string nombre, int nivel, int jugadas) {
    // ios::app agrega al final sin borrar los récords anteriores.
    ofstream archivo("records.txt", ios::app);
    archivo << nivel << " " << jugadas << " " << nombre << "\n";
    // El archivo se cierra solo al terminar la función.
}
