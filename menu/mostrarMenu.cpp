// Pieza: mostrarMenu. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <iostream>
#include <cstdlib> // system: limpia la pantalla.

using namespace std;

/*
    Qué hace: muestra el título y las opciones, y espera una elección válida.
    Parámetros: ninguno; las opciones se escriben dentro de esta función.
    Devuelve: 1 = jugar, 2 = récords, 3 = cómo jugar o 0 = salir.
    También devuelve 0 si la lectura termina porque se cerró la entrada (EOF)
    o porque ocurrió un error que impide seguir leyendo.
    Efecto: escribe y lee por consola. Solo recoge la elección; el main decide
    qué hacer con ella, por lo que aquí todavía no se crea ningún tablero.
*/
int mostrarMenu() {
    // Cada vez que se vuelve al menú se empieza con la pantalla limpia.
    system("cls");
    // Reutilizamos la función del título para mostrar el mismo encabezado.
    mostrarTitulo();
    cout << "1. Jugar\n";
    cout << "2. Ver récords (Top 5)\n";
    cout << "3. Cómo jugar\n";
    cout << "0. Salir\n";
    cout << "Elige una opción: ";
    // El rango incluye 0, 1, 2 y 3. La función de lectura vuelve a pedir el dato
    // si es inválido, y return entrega su resultado directamente al main.
    return leerEnteroEnRango(0, 3);
}
