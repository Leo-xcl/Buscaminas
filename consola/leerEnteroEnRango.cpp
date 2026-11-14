// Pieza: leerEnteroEnRango. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <iostream>

using namespace std;

/*
    Qué hace: lee y valida un número entero escrito en la consola.
    Parámetros minimo y maximo: límites permitidos, ambos incluidos.
    Se requiere minimo <= maximo. El mensaje inicial se imprime antes de llamarla.
    Devuelve: el entero válido, o 0 si termina la entrada (EOF) o hay un error
    de lectura que impide continuar. Los menús interpretan ese 0 como salir.

    Repite la petición si hay letras, decimales o un valor fuera del rango.
    Por ejemplo, leerEnteroEnRango(5, 20) acepta 12 y rechaza 4, 0 y "12abc".
    Escribir 0 solo es válido si pertenece al rango; el 0 devuelto al llegar a
    EOF es un aviso especial y puede quedar fuera de ese rango.
*/
int leerEnteroEnRango(int minimo, int maximo) {
    // El ciclo continúa hasta que un return entrega un valor y sale de la función.
    while (true) {
        int numero = 0;
        cin >> numero;

        // fail() detecta una conversión fallida, por ejemplo letras o un entero
        // demasiado grande. ! invierte el resultado: valido es true si no falló.
        bool valido = !cin.fail();

        // Si no se obtuvo un número y ya no hay datos (eof), no tiene sentido
        // volver a pedirlo. bad() indica un problema grave del flujo de entrada.
        if (!valido && (cin.eof() || cin.bad())) {
            return 0;
        }

        // clear() quita el estado de error para poder leer el resto de la línea.
        // No borra los caracteres: el siguiente ciclo se encarga de consumirlos.
        cin.clear();

        // Consumimos el resto de la línea para no afectar la lectura siguiente.
        // Solo permitimos espacios después del entero: "3abc" no es válido.
        // get lee un carácter; se detiene al llegar al salto de línea (\n) o EOF.
        char resto;
        while (cin.get(resto) && resto != '\n') {
            // También aceptamos tabulaciones (\t) y retornos de carro (\r).
            if (resto != ' ' && resto != '\t' && resto != '\r') {
                valido = false;
            }
        }

        // Se necesitan las tres condiciones: formato correcto y ambos límites.
        if (valido && numero >= minimo && numero <= maximo) {
            return numero;
        }

        // La entrada también puede haberse terminado al consumir el resto.
        if (cin.eof() || cin.bad()) {
            return 0;
        }

        // Si todavía podemos leer, explicamos el rango y repetimos el ciclo.
        cout << "Entrada inválida. Escribe un entero entre "
             << minimo << " y " << maximo << ": ";
    }
}
