// Pieza: mostrarRecords. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <iostream>
#include <iomanip> // setw y left: alinean la tabla.
#include <fstream> // ifstream: lee un archivo de texto.
#include <string>

using namespace std;

// Un récord leído de records.txt. Solo se usa en este archivo.
struct Record {
    int nivel;
    int jugadas;
    string nombre;
};

/*
    Qué hace: lee records.txt y muestra los 5 mejores de cada dificultad
    (los que ganaron con menos jugadas).

    Parámetros: ninguno.
    Devuelve: nada (void). Escribe en la consola.

    Pasos:
    1. Contar cuántos récords hay en el archivo.
    2. Reservar un arreglo dinámico de ese tamaño y leerlos.
    3. Ordenarlos de menos a más jugadas (método de la burbuja).
    4. Imprimir los primeros 5 de cada nivel.
    5. Liberar el arreglo.
*/
void mostrarRecords() {
    cambiarColor(14);
    cout << "\nRÉCORDS (menos jugadas = mejor)\n";
    cambiarColor(7);

    ifstream archivo("records.txt");

    // 1. Contamos las líneas válidas: nivel, jugadas y nombre.
    int total = 0;
    int nivel;
    int jugadas;
    string nombre;
    while (archivo >> nivel >> jugadas && getline(archivo, nombre)) {
        total++;
    }

    // Si el archivo no existe o está vacío, no hay nada que mostrar.
    if (total == 0) {
        cout << "Todavía no hay récords. Gana una partida en Fácil, Medio o Difícil.\n";
        return;
    }

    // 2. Ya sabemos cuántos son: reservamos justo ese espacio.
    Record* records = new Record[total];

    // Regresamos al inicio del archivo para leerlo otra vez.
    // clear() quita la marca de "fin de archivo" que dejó el primer recorrido.
    archivo.clear();
    archivo.seekg(0);
    for (int i = 0; i < total; i++) {
        archivo >> records[i].nivel >> records[i].jugadas;
        archivo.ignore(1); // Salta el espacio que hay antes del nombre.
        getline(archivo, records[i].nombre);
    }

    // 3. Burbuja: si uno tiene más jugadas que el siguiente, se intercambian.
    for (int pasada = 0; pasada < total - 1; pasada++) {
        for (int i = 0; i < total - 1 - pasada; i++) {
            if (records[i].jugadas > records[i + 1].jugadas) {
                Record temporal = records[i];
                records[i] = records[i + 1];
                records[i + 1] = temporal;
            }
        }
    }

    // 4. Una tabla por nivel. La posición 0 queda vacía para usar 1, 2 y 3.
    string nombresNivel[4] = {"", "FÁCIL (9 x 9)", "MEDIO (16 x 16)", "DIFÍCIL (16 x 30)"};
    for (int n = 1; n <= 3; n++) {
        cambiarColor(11);
        cout << "\n" << nombresNivel[n] << "\n";
        cambiarColor(7);

        int mostrados = 0;
        // Como ya están ordenados, los primeros que encontremos son los mejores.
        for (int i = 0; i < total && mostrados < 5; i++) {
            if (records[i].nivel == n) {
                mostrados++;
                cout << "  " << mostrados << ". " << left << setw(20) << records[i].nombre
                     << right << records[i].jugadas << " jugadas\n";
            }
        }
        if (mostrados == 0) {
            cout << "  (sin récords)\n";
        }
    }

    // 5. Cada new[] necesita su delete[].
    delete[] records;
}
