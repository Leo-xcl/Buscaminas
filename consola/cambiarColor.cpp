// Pieza: cambiarColor. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <windows.h> // Funciones de color de Windows.

/*
    Qué hace: cambia el color de los textos que se impriman después de la llamada.
    Parámetro color: código de Windows; 7 = gris, 12 = rojo y 15 = blanco.
    Devuelve: nada, porque su tipo de retorno es void.

    El color permanece hasta que se vuelva a cambiar. No modifica el texto que
    ya apareció en pantalla. Se usa desde el menú y el dibujo del tablero.
    Requiere una consola de Windows compatible con estas funciones de color.
*/
void cambiarColor(int color) {
    // GetStdHandle obtiene el identificador de la salida estándar de la consola.
    // SetConsoleTextAttribute aplica a esa salida el código de color recibido.
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
