// Pieza: elegirDificultad. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <iostream>

using namespace std;

/*
    Qué hace: permite escoger una dificultad fija o configurar el tamaño y
    la cantidad de minas, y guarda esos datos en las variables del main.
    Parámetros:
      filas: dirección de la variable que recibirá la cantidad de filas.
      columnas: dirección de la variable que recibirá la cantidad de columnas.
      minas: dirección de la variable que recibirá la cantidad de minas.
    Los tres punteros deben apuntar a variables int válidas y distintas.
    Devuelve: nada (void). Entrega los resultados modificando esas variables.
    Por ejemplo, &filas obtiene la dirección de la variable del main y
    *filas permite modificar su valor desde esta función.
    Si se elige 0 en el menú de dificultad, se cierra la entrada (EOF) o un
    error impide seguir leyendo, los tres resultados quedan en cero.
    Todavía no reserva memoria ni coloca minas: solo define la configuración.
*/
void elegirDificultad(int* filas, int* columnas, int* minas) {
    // Los ceros indican al main que todavía no hay una configuración lista.
    // Si se cancela, se conservan y el main no intenta crear un tablero.
    *filas = 0;
    *columnas = 0;
    *minas = 0;

    cout << "\nDIFICULTAD\n";
    cout << "1. Fácil:          9 x 9,   10 minas\n";
    cout << "2. Medio:        16 x 16,  40 minas\n";
    cout << "3. Difícil:      16 x 30,  99 minas\n";
    cout << "4. Personalizado\n";
    cout << "0. Volver al menú\n";
    cout << "Elige una opción: ";
    int opcion = leerEnteroEnRango(0, 4);

    // Las tres primeras opciones copian tamaños y cantidades ya definidos.
    // Si opcion es 0, no se cumple ninguna rama y los tres resultados quedan en 0.
    if (opcion == 1) {
        *filas = 9;
        *columnas = 9;
        *minas = 10;
    } else if (opcion == 2) {
        *filas = 16;
        *columnas = 16;
        *minas = 40;
    } else if (opcion == 3) {
        *filas = 16;
        *columnas = 30;
        *minas = 99;
    } else if (opcion == 4) {
        // Estos límites mantienen un tamaño razonable para la consola.
        // Primero leemos variables locales para no devolver datos incompletos.
        cout << "Filas (5 a 20): ";
        int nuevasFilas = leerEnteroEnRango(5, 20);
        // En estos datos, escribir 0 es inválido y vuelve a pedir el número.
        // Un 0 DEVUELTO por la función indica que ya no se puede leer (por
        // ejemplo, EOF). return; termina esta función void sin dar un valor.
        if (nuevasFilas == 0) {
            return;
        }

        cout << "Columnas (5 a 30): ";
        int nuevasColumnas = leerEnteroEnRango(5, 30);
        // También salimos si la entrada se cierra mientras se leen columnas.
        if (nuevasColumnas == 0) {
            return;
        }

        // Filas por columnas es el total de casillas. Restamos uno para que
        // siempre exista al menos una casilla segura: en 5 x 5 caben 24 minas.
        int maximoMinas = nuevasFilas * nuevasColumnas - 1;
        cout << "Minas (1 a " << maximoMinas << "): ";
        int nuevasMinas = leerEnteroEnRango(1, maximoMinas);
        // Sin una cantidad válida, no se confirma ninguna parte del resultado.
        if (nuevasMinas == 0) {
            return;
        }

        // Solo cuando los tres datos están completos los copiamos al main.
        // El asterisco accede al valor guardado en cada dirección recibida.
        *filas = nuevasFilas;
        *columnas = nuevasColumnas;
        *minas = nuevasMinas;
    }
}
