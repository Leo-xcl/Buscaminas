// Pieza: jugarPartida. Este archivo contiene solo esta función.
#include "../compartido/declaraciones.cpp" // Tipos y funciones compartidos; no contiene implementaciones.
#include <iostream>
#include <cstdlib> // system: limpia la pantalla.
#include <string>  // string: guarda el mensaje del turno anterior.

using namespace std;

/*
    Qué hace: repite los turnos de una partida hasta ganar o quedarse sin vidas,
    y al final muestra el tablero con todas las minas.

    Parámetros:
    - tablero: matriz con las minas y los números ya calculados.
    - filas y columnas: dimensiones de esa matriz.
    - minas: total de minas, solo para mostrarlo en el panel.
    - jugadas: dirección donde se guarda cuántas casillas reveló el jugador.
      Las banderas no cuentan como jugada.

    Devuelve: true si el jugador ganó y false si perdió o se cerró la entrada.

    Cada turno: limpiar pantalla -> dibujar -> pedir acción, fila y columna
    -> aplicar la acción. Se pisa una mina = se pierde una vida.
    La pista (P) se puede usar una vez, no pide fila ni columna y cuenta
    como jugada porque revela casillas.
*/
bool jugarPartida(Casilla** tablero, int filas, int columnas, int minas, int* jugadas) {
    int vidas = 3;
    int banderas = 0;
    bool pistaUsada = false;
    *jugadas = 0;

    // Se escribe en un turno y se muestra al inicio del siguiente,
    // porque system("cls") borra todo lo que estaba en pantalla.
    string mensaje = "";

    while (vidas > 0 && !todasReveladas(tablero, filas, columnas)) {
        system("cls");
        dibujarTablero(tablero, filas, columnas, vidas, banderas, minas, *jugadas);

        if (pistaUsada) {
            cout << "     Pista: usada\n";
        } else {
            cout << "     Pista: disponible (P)\n";
        }

        if (mensaje != "") {
            cambiarColor(14);
            cout << "\n" << mensaje << "\n";
            cambiarColor(7);
            mensaje = "";
        }

        cout << "\nAcción (R = revelar, B = bandera, P = pista): ";
        char accion = leerAccion();
        if (accion == 0) {
            return false;
        }

        // La pista no necesita fila ni columna, así que se resuelve aquí.
        // continue salta al siguiente turno sin ejecutar el resto del ciclo.
        if (accion == 'P') {
            int filaPista = 0;
            int columnaPista = 0;
            if (pistaUsada) {
                mensaje = "Ya usaste tu pista en esta partida.";
            } else if (usarPista(tablero, filas, columnas, &filaPista, &columnaPista)) {
                pistaUsada = true;
                (*jugadas)++;
                reproducirSonido(SONIDO_PISTA);
                // to_string convierte el número en texto para unirlo al mensaje.
                mensaje = "Pista: se reveló la fila " + to_string(filaPista + 1) +
                          ", columna " + to_string(columnaPista + 1) + ".";
            } else {
                mensaje = "No hay casillas para la pista: quita alguna bandera.";
            }
            continue;
        }

        cout << "Fila (1 a " << filas << "): ";
        int fila = leerEnteroEnRango(1, filas);
        if (fila == 0) {
            return false;
        }

        cout << "Columna (1 a " << columnas << "): ";
        int columna = leerEnteroEnRango(1, columnas);
        if (columna == 0) {
            return false;
        }

        // En pantalla se cuenta desde 1, pero la matriz empieza en 0.
        // casilla guarda la dirección de la elegida para no repetir tablero[f][c].
        Casilla* casilla = &tablero[fila - 1][columna - 1];

        if (accion == 'B') {
            if (casilla->revelada) {
                mensaje = "Esa casilla ya está revelada.";
            } else {
                alternarBandera(casilla, &banderas);
                reproducirSonido(SONIDO_BANDERA);
            }
        } else if (casilla->bandera) {
            mensaje = "Quita la bandera (B) antes de revelar esa casilla.";
        } else if (casilla->revelada) {
            mensaje = "Esa casilla ya está revelada.";
        } else if (casilla->esMina) {
            casilla->revelada = true;
            vidas--;
            (*jugadas)++;
            mensaje = "¡BOOM! Pisaste una mina y perdiste una vida.";
            reproducirSonido(SONIDO_MINA);
        } else {
            revelarCasilla(tablero, filas, columnas, fila - 1, columna - 1);
            (*jugadas)++;
            reproducirSonido(SONIDO_REVELAR);
        }
    }

    // Si el ciclo terminó con vidas, fue porque se revelaron todas las casillas seguras.
    bool gano = vidas > 0;

    mostrarMinas(tablero, filas, columnas);
    system("cls");
    dibujarTablero(tablero, filas, columnas, vidas, banderas, minas, *jugadas);

    if (gano) {
        cambiarColor(10);
        cout << "\n¡GANASTE en " << *jugadas << " jugadas!\n";
    } else {
        cambiarColor(12);
        cout << "\nPERDISTE: te quedaste sin vidas.\n";
    }
    cambiarColor(7);

    // El sonido va después del mensaje para que se lea mientras suena.
    if (gano) {
        reproducirSonido(SONIDO_VICTORIA);
    } else {
        reproducirSonido(SONIDO_DERROTA);
    }

    return gano;
}
