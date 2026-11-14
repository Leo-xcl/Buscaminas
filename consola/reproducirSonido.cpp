// Pieza: reproducirSonido. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <windows.h> // Beep: produce un tono.

/*
    Qué hace: toca el efecto de sonido que corresponde a lo que pasó.
    Parámetro tipo: una de las constantes SONIDO_ de declaraciones.cpp.
    Devuelve: nada (void).

    Beep(frecuencia, duracion): la frecuencia es el tono en Hz (más alto =
    más agudo) y la duración está en milisegundos. El programa espera a que
    termine cada tono, por eso los sonidos de los turnos son cortos.
*/
void reproducirSonido(int tipo) {
    switch (tipo) {
        case SONIDO_REVELAR:
            Beep(900, 40);
            break;
        case SONIDO_BANDERA:
            Beep(1200, 40);
            break;
        case SONIDO_MINA:
            Beep(150, 400);
            break;
        case SONIDO_PISTA:
            Beep(880, 80);
            Beep(1175, 120);
            break;
        case SONIDO_VICTORIA:
            // Do, Mi, Sol y Do agudo.
            Beep(523, 150);
            Beep(659, 150);
            Beep(784, 150);
            Beep(1047, 400);
            break;
        case SONIDO_DERROTA:
            Beep(400, 300);
            Beep(300, 300);
            Beep(200, 600);
            break;
    }
}
