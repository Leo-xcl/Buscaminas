/*
    Cada función está en su propio archivo .cpp, agrupado por su tarea:
    menu/          -> título, opciones e instrucciones.
    configuracion/ -> elección de dificultad y tamaño.
    juego/         -> memoria del tablero, minas y números vecinos.
    consola/       -> colores, lectura de datos y dibujo del tablero.
    records/       -> guardar y mostrar los récords (records.txt).
    compartido/    -> estructura Casilla y declaraciones de funciones.

    Este main coordina el menú y las partidas.

    Para compilar, abre la terminal en esta carpeta (con g++ en el PATH):
    g++ -static -std=c++11 -Wall -Wextra -pedantic main.cpp menu\*.cpp configuracion\*.cpp juego\*.cpp consola\*.cpp records\*.cpp compartido\*.cpp -o buscaminas.exe

    -static mete las librerías de C++ dentro del .exe. Sin esto, Windows puede
    cargar otra versión instalada por otro programa y el juego no abre.
    También permite ejecutarlo en otra computadora que no tenga g++.

    Para ejecutar en PowerShell:
    .\buscaminas.exe

    Compila todos los .cpp juntos y conserva la estructura de carpetas.
    Los archivos que contienen funciones se unen al compilar; no se incluyen
    con #include. Se incluye compartido/declaraciones.cpp para conocer sus firmas.
*/

#include "compartido/declaraciones.cpp" // Da a conocer Casilla y las firmas de las funciones.
#include <iostream>  // cout: mensajes en la consola.
#include <cstdlib>   // srand: inicia la secuencia de números aleatorios.
#include <ctime>     // time: proporciona la hora usada como semilla.
#include <windows.h> // Salida UTF-8 en la consola de Windows.
#include <string>    // string: nombre del jugador para el récord.

using namespace std; // Permite escribir cout en lugar de std::cout.

/*
    MAIN

    Qué hace: coordina el menú, la creación del tablero, la partida y los récords.
    Parámetros: ninguno; esta versión no recibe argumentos de la terminal.
    Devuelve: 0 al terminar normalmente, como resultado para el sistema operativo.
    Efectos: lee el teclado, escribe en la consola y reserva/libera los tableros.

    Orden de uso: elegir -> crear -> colocar minas -> calcular -> dibujar
    -> jugar -> liberar.
*/
int main() {
    SetConsoleOutputCP(CP_UTF8); // Permite mostrar los acentos del archivo UTF-8.

    // Se inicia la secuencia aleatoria una sola vez, antes de crear tableros.
    // Así las siguientes llamadas a rand continúan esa secuencia sin reiniciarla.
    srand(static_cast<unsigned int>(time(0)));
    // time(0) da la hora actual; la conversión adapta su tipo al que pide srand.

    // La primera lectura decide si entramos al ciclo o salimos inmediatamente.
    int opcion = mostrarMenu();
    while (opcion != 0) {
        if (opcion == 3) {
            // Consultar las instrucciones no necesita reservar un tablero.
            mostrarInstrucciones();
            // Sin la pausa, el menú limpiaría la pantalla antes de poder leerlas.
            system("pause");
        } else if (opcion == 2) {
            mostrarRecords();
            system("pause");
        } else if (opcion == 1) {
            // Estas variables recibirán los datos elegidos por el usuario.
            // El valor 0 permite reconocer una selección cancelada o incompleta.
            int filas = 0;
            int columnas = 0;
            int minas = 0;

            // & entrega las direcciones para que la función escriba los valores.
            elegirDificultad(&filas, &columnas, &minas);

            // Solo creamos el tablero si la selección produjo los tres valores.
            // Si se canceló, saltamos este bloque y regresamos al menú.
            if (filas > 0 && columnas > 0 && minas > 0) {
                // El orden es importante: primero necesitamos memoria para las
                // casillas, luego las minas y finalmente sus números vecinos.
                Casilla** tablero = crearTablero(filas, columnas);
                colocarMinas(tablero, filas, columnas, minas);
                calcularNumeros(tablero, filas, columnas);

                // jugarPartida repite los turnos y escribe en jugadas cuántas
                // casillas se revelaron. Devuelve true si el jugador ganó.
                int jugadas = 0;
                bool gano = jugarPartida(tablero, filas, columnas, minas, &jugadas);

                // Solo se guardan victorias de Fácil, Medio o Difícil.
                int nivel = calcularNivel(filas, columnas, minas);
                if (gano && nivel != 0) {
                    // La consola de Windows no siempre lee bien acentos ni ñ,
                    // y en el archivo saldrían símbolos raros.
                    cout << "\nEscribe tu nombre para el récord (sin acentos ni ñ): ";
                    string nombre;
                    // getline lee la línea completa, así el nombre puede tener espacios.
                    getline(cin, nombre);
                    if (nombre == "") {
                        nombre = "Anónimo";
                    }
                    // Un nombre muy largo desacomodaría la tabla de récords.
                    if (nombre.size() > 20) {
                        nombre = nombre.substr(0, 20);
                    }
                    guardarRecord(nombre, nivel, jugadas);
                    cout << "Récord guardado.\n";
                } else if (gano) {
                    cout << "\nLas partidas personalizadas no entran a los récords.\n";
                }

                system("pause"); // Espera una tecla antes de volver al menú.

                // Así cada tablero deja de ocupar memoria antes de volver al menú.
                liberarTablero(tablero, filas);
                // 0 es un puntero nulo: ya no hay una matriz disponible aquí.
                tablero = 0;
            }
        }

        // Actualizamos la opción: sin esta nueva lectura repetiríamos la anterior.
        opcion = mostrarMenu();
    }

    // Al elegir 0, o al cerrarse la entrada, el ciclo termina y dejamos color gris.
    cambiarColor(7);
    cout << "\nPrograma finalizado.\n";
    return 0; // Indica al sistema operativo que el programa terminó correctamente.
}
