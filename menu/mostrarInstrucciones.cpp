// Pieza: mostrarInstrucciones. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <iostream>

using namespace std;

/*
    Qué hace: muestra las reglas básicas, los controles y el significado
    de los símbolos del tablero.
    Parámetros: ninguno; no necesita recibir un tablero para mostrar la ayuda.
    Devuelve: nada, porque es una función void.
    Efecto: escribe en la consola; no solicita datos ni cambia las casillas.
    Al terminar, el main continúa su recorrido y vuelve a mostrar el menú.
*/
void mostrarInstrucciones() {
    // El encabezado se destaca en amarillo; el resto de la ayuda queda en gris.
    cambiarColor(14);
    cout << "\nCÓMO JUGAR\n";
    cambiarColor(7);
    cout << "Descubre todas las casillas que no tienen mina para ganar.\n";
    cout << "Cada número indica cuántas minas hay en las ocho casillas vecinas.\n";
    cout << "Las filas y columnas se escriben como aparecen en pantalla, desde 1.\n\n";

    // Los controles se resaltan para que se encuentren rápido.
    cambiarColor(14);
    cout << "CONTROLES\n";
    cambiarColor(7);
    cout << "R = revelar una casilla\n";
    cout << "B = poner o quitar una bandera\n";
    cout << "P = usar la pista (una por partida)\n\n";

    cout << "Tienes 3 vidas: cada mina que pises te quita una.\n";
    cout << "Si te quedas sin vidas, pierdes la partida.\n\n";
    cout << "Si ganas en Fácil, Medio o Difícil, tu nombre entra a los récords.\n";
    cout << "Gana quien necesite menos jugadas.\n\n";
    cout << "Símbolos: # = oculta, B = bandera, * = mina, . = cero minas vecinas.\n";
}
