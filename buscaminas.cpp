/*
    BUSCAMINAS
    Por: Leo y Gamma

    Funcionalidades agregadas al Buscaminas clásico:
    - Tablero personalizado (el tamaño se reserva con memoria dinámica)
    - 3 vidas: pisar una mina no termina la partida a la primera
    - Una pista por partida
    - Top 5 de récords guardado en records.txt
    - Efectos de sonido

    Para compilar:
    g++ -static -std=c++11 -Wall -Wextra buscaminas.cpp -o buscaminas.exe

    -static mete las librerías de C++ dentro del .exe. Así el juego abre en
    cualquier computadora, aunque no tenga g++ o tenga otra versión instalada.
*/

#include <iostream>
#include <iomanip>   // setw, left y right: alinear columnas
#include <fstream>   // ifstream y ofstream: archivo de récords
#include <string>
#include <cstdlib>   // rand, srand y system
#include <ctime>     // time: semilla para rand
#include <cctype>    // toupper: convertir a mayúscula
#include <windows.h> // colores, Beep y salida UTF-8

using namespace std;

// ==================== ESTRUCTURAS Y CONSTANTES ====================

struct Casilla {
    bool esMina;
    bool revelada;
    bool bandera;
    int minasVecinas; // De 0 a 8. En las minas no se usa.
};

struct Record {
    int nivel; // 1 = Fácil, 2 = Medio, 3 = Difícil
    int jugadas;
    string nombre;
};

// Tipos de sonido que entiende reproducirSonido.
const int SONIDO_REVELAR = 1;
const int SONIDO_BANDERA = 2;
const int SONIDO_MINA = 3;
const int SONIDO_PISTA = 4;
const int SONIDO_VICTORIA = 5;
const int SONIDO_DERROTA = 6;

// ==================== DECLARACIONES ====================

// Consola
void cambiarColor(int color);
int leerEnteroEnRango(int minimo, int maximo);
char leerAccion();
void reproducirSonido(int tipo);

// Menú
void mostrarTitulo();
int mostrarMenu();
void mostrarInstrucciones();

// Configuración
void elegirDificultad(int* filas, int* columnas, int* minas);
int calcularNivel(int filas, int columnas, int minas);

// Tablero
Casilla** crearTablero(int filas, int columnas);
void liberarTablero(Casilla** tablero, int filas);
void colocarMinas(Casilla** tablero, int filas, int columnas, int minas);
int contarVecinas(Casilla** tablero, int filas, int columnas, int f, int c);
void calcularNumeros(Casilla** tablero, int filas, int columnas);
void dibujarSeparador(int columnas);
void dibujarTablero(Casilla** tablero, int filas, int columnas,
                    int vidas, int banderas, int minas, int jugadas);

// Juego
void alternarBandera(Casilla* casilla, int* banderas);
void revelarCasilla(Casilla** tablero, int filas, int columnas, int f, int c);
bool usarPista(Casilla** tablero, int filas, int columnas, int* filaPista, int* columnaPista);
bool todasReveladas(Casilla** tablero, int filas, int columnas);
void mostrarMinas(Casilla** tablero, int filas, int columnas);
bool jugarPartida(Casilla** tablero, int filas, int columnas, int minas, int* jugadas);

// Récords
void guardarRecord(string nombre, int nivel, int jugadas);
void mostrarRecords();

// ==================== MAIN ====================

int main() {
    SetConsoleOutputCP(CP_UTF8); // Para que se vean los acentos.
    srand(time(0));              // Semilla: cada partida tiene minas distintas.

    int opcion = mostrarMenu();
    while (opcion != 0) {
        if (opcion == 1) {
            int filas = 0;
            int columnas = 0;
            int minas = 0;
            // Se pasan las direcciones (&) para que la función cambie las variables.
            elegirDificultad(&filas, &columnas, &minas);

            // Si el jugador eligió "Volver", las tres siguen en 0.
            if (filas > 0 && columnas > 0 && minas > 0) {
                Casilla** tablero = crearTablero(filas, columnas);
                colocarMinas(tablero, filas, columnas, minas);
                calcularNumeros(tablero, filas, columnas);

                int jugadas = 0;
                bool gano = jugarPartida(tablero, filas, columnas, minas, &jugadas);

                // Solo entran a los récords las victorias en Fácil, Medio o Difícil.
                int nivel = calcularNivel(filas, columnas, minas);
                if (gano && nivel != 0) {
                    // La consola de Windows no siempre lee bien acentos ni ñ.
                    cout << "\nEscribe tu nombre para el récord (sin acentos ni ñ): ";
                    string nombre;
                    getline(cin, nombre); // getline acepta nombres con espacios.
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

                system("pause");
                liberarTablero(tablero, filas);
            }
        } else if (opcion == 2) {
            mostrarRecords();
            system("pause"); // Sin la pausa, el menú borraría la pantalla.
        } else if (opcion == 3) {
            mostrarInstrucciones();
            system("pause");
        }

        opcion = mostrarMenu();
    }

    cambiarColor(7);
    cout << "\nPrograma finalizado.\n";
    return 0;
}

// ==================== CONSOLA ====================

// Cambia el color del texto que se imprima después.
// Algunos códigos: 7 = gris, 9 = azul, 10 = verde, 12 = rojo, 14 = amarillo.
void cambiarColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Lee un entero entre minimo y maximo (incluidos). Si escriben letras,
// decimales o un número fuera del rango, lo vuelve a pedir.
// Devuelve 0 si la entrada se cerró y ya no se puede leer.
int leerEnteroEnRango(int minimo, int maximo) {
    while (true) {
        int numero = 0;
        cin >> numero;
        bool valido = !cin.fail();

        // Si no hay número y la entrada se terminó, no tiene caso seguir pidiendo.
        if (!valido && (cin.eof() || cin.bad())) {
            return 0;
        }

        // Quitamos el error y leemos el resto de la línea.
        // Si hay algo más que espacios (por ejemplo "3abc"), no es válido.
        cin.clear();
        char resto;
        while (cin.get(resto) && resto != '\n') {
            if (resto != ' ' && resto != '\t' && resto != '\r') {
                valido = false;
            }
        }

        if (valido && numero >= minimo && numero <= maximo) {
            return numero;
        }
        if (cin.eof() || cin.bad()) {
            return 0;
        }

        cout << "Entrada inválida. Escribe un entero entre "
             << minimo << " y " << maximo << ": ";
    }
}

// Lee la acción del turno: 'R' (revelar), 'B' (bandera) o 'P' (pista).
// Acepta minúsculas. Devuelve 0 si la entrada se cerró.
char leerAccion() {
    while (true) {
        char letra;
        cin >> letra;
        if (cin.fail()) {
            return 0;
        }
        cin.ignore(1000, '\n'); // Descarta lo que quede en la línea.

        letra = toupper(letra);
        if (letra == 'R' || letra == 'B' || letra == 'P') {
            return letra;
        }
        cout << "Acción inválida. Escribe R, B o P: ";
    }
}

// Toca un efecto de sonido. Beep(frecuencia en Hz, duración en milisegundos).
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
        case SONIDO_VICTORIA: // Do, Mi, Sol, Do agudo
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

// ==================== MENÚ ====================

void mostrarTitulo() {
    cambiarColor(11);
    cout << "\n+--------------------------------------+\n";
    cout << "|              BUSCAMINAS              |\n";
    cout << "+--------------------------------------+\n";
    cambiarColor(7);
    cout << "Por: Leo y Gamma\n\n";
}

// Muestra el menú y devuelve la opción elegida (0 a 3).
int mostrarMenu() {
    system("cls");
    mostrarTitulo();
    cout << "1. Jugar\n";
    cout << "2. Ver récords (Top 5)\n";
    cout << "3. Cómo jugar\n";
    cout << "0. Salir\n";
    cout << "Elige una opción: ";
    return leerEnteroEnRango(0, 3);
}

void mostrarInstrucciones() {
    cambiarColor(14);
    cout << "\nCÓMO JUGAR\n";
    cambiarColor(7);
    cout << "Descubre todas las casillas que no tienen mina para ganar.\n";
    cout << "Cada número indica cuántas minas hay en las ocho casillas vecinas.\n";
    cout << "Las filas y columnas se escriben como aparecen en pantalla, desde 1.\n\n";

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

// ==================== CONFIGURACIÓN ====================

// Pide la dificultad y guarda filas, columnas y minas en las variables del main.
// Si el jugador elige "Volver", las tres quedan en 0.
void elegirDificultad(int* filas, int* columnas, int* minas) {
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
        // Primero en variables locales, para no devolver datos a medias.
        cout << "Filas (5 a 20): ";
        int nuevasFilas = leerEnteroEnRango(5, 20);
        if (nuevasFilas == 0) {
            return;
        }

        cout << "Columnas (5 a 30): ";
        int nuevasColumnas = leerEnteroEnRango(5, 30);
        if (nuevasColumnas == 0) {
            return;
        }

        // Se resta 1 para que siempre quede al menos una casilla sin mina.
        int maximoMinas = nuevasFilas * nuevasColumnas - 1;
        cout << "Minas (1 a " << maximoMinas << "): ";
        int nuevasMinas = leerEnteroEnRango(1, maximoMinas);
        if (nuevasMinas == 0) {
            return;
        }

        *filas = nuevasFilas;
        *columnas = nuevasColumnas;
        *minas = nuevasMinas;
    }
}

// Devuelve 1 = Fácil, 2 = Medio, 3 = Difícil o 0 = Personalizado.
// Sirve para no comparar en los récords partidas de distinto tamaño.
int calcularNivel(int filas, int columnas, int minas) {
    if (filas == 9 && columnas == 9 && minas == 10) {
        return 1;
    }
    if (filas == 16 && columnas == 16 && minas == 40) {
        return 2;
    }
    if (filas == 16 && columnas == 30 && minas == 99) {
        return 3;
    }
    return 0;
}

// ==================== TABLERO ====================

/*
    Reserva el tablero con memoria dinámica y deja todas las casillas vacías.

    Casilla** es un "puntero a punteros de Casilla":

    tablero --> [puntero fila 0] --> [Casilla][Casilla][Casilla]...
                [puntero fila 1] --> [Casilla][Casilla][Casilla]...
                [puntero fila 2] --> [Casilla][Casilla][Casilla]...

    Quien lo use debe llamar a liberarTablero al terminar.
*/
Casilla** crearTablero(int filas, int columnas) {
    Casilla** tablero = new Casilla*[filas]; // Arreglo con la dirección de cada fila.

    for (int f = 0; f < filas; f++) {
        tablero[f] = new Casilla[columnas];  // Las casillas de esta fila.
        for (int c = 0; c < columnas; c++) {
            tablero[f][c].esMina = false;
            tablero[f][c].revelada = false;
            tablero[f][c].bandera = false;
            tablero[f][c].minasVecinas = 0;
        }
    }
    return tablero;
}

// Cada new[] necesita su delete[]: primero las filas y al final el arreglo principal.
void liberarTablero(Casilla** tablero, int filas) {
    for (int f = 0; f < filas; f++) {
        delete[] tablero[f];
    }
    delete[] tablero;
}

// Coloca las minas en posiciones al azar, sin repetir casilla.
void colocarMinas(Casilla** tablero, int filas, int columnas, int minas) {
    int colocadas = 0;
    while (colocadas < minas) {
        int f = rand() % filas;    // Número entre 0 y filas - 1
        int c = rand() % columnas;

        // Si ya había mina ahí, no cuenta y se intenta otra posición.
        if (!tablero[f][c].esMina) {
            tablero[f][c].esMina = true;
            colocadas++;
        }
    }
}

// Cuenta las minas en las 8 casillas alrededor de [f][c].
int contarVecinas(Casilla** tablero, int filas, int columnas, int f, int c) {
    int cantidad = 0;
    for (int filaVecina = f - 1; filaVecina <= f + 1; filaVecina++) {
        for (int columnaVecina = c - 1; columnaVecina <= c + 1; columnaVecina++) {
            // Primero revisamos que esté dentro del tablero (bordes y esquinas).
            if (filaVecina >= 0 && filaVecina < filas &&
                columnaVecina >= 0 && columnaVecina < columnas) {
                // La propia casilla no cuenta.
                if (filaVecina != f || columnaVecina != c) {
                    if (tablero[filaVecina][columnaVecina].esMina) {
                        cantidad++;
                    }
                }
            }
        }
    }
    return cantidad;
}

// Guarda en cada casilla segura cuántas minas tiene alrededor.
void calcularNumeros(Casilla** tablero, int filas, int columnas) {
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (!tablero[f][c].esMina) {
                tablero[f][c].minasVecinas = contarVecinas(tablero, filas, columnas, f, c);
            }
        }
    }
}

// Borde de arriba o de abajo del tablero. Cada casilla ocupa 3 caracteres.
void dibujarSeparador(int columnas) {
    cout << "    +";
    for (int c = 0; c < columnas; c++) {
        cout << "---";
    }
    cout << "+\n";
}

// Muestra el panel de la partida y el tablero con colores.
void dibujarTablero(Casilla** tablero, int filas, int columnas,
                    int vidas, int banderas, int minas, int jugadas) {
    // El color de cada número: la posición del arreglo es el número (0 a 8).
    int colores[9] = {7, 9, 10, 12, 13, 14, 11, 15, 8};

    cambiarColor(15);
    cout << "\nVidas: " << vidas << "  Banderas: " << banderas
         << "  Minas: " << minas << "  Jugadas: " << jugadas << "\n\n";
    cambiarColor(7);

    // Números de columna, contando desde 1.
    cout << "     ";
    for (int c = 0; c < columnas; c++) {
        cout << setw(3) << c + 1;
    }
    cout << "\n";
    dibujarSeparador(columnas);

    for (int f = 0; f < filas; f++) {
        cout << setw(3) << f + 1 << " |";

        for (int c = 0; c < columnas; c++) {
            if (!tablero[f][c].revelada) {
                if (tablero[f][c].bandera) {
                    cambiarColor(14);
                    cout << setw(3) << 'B';
                } else {
                    cambiarColor(8);
                    cout << setw(3) << '#';
                }
            } else if (tablero[f][c].esMina) {
                cambiarColor(12);
                cout << setw(3) << '*';
            } else {
                int numero = tablero[f][c].minasVecinas;
                cambiarColor(colores[numero]);
                if (numero == 0) {
                    cout << setw(3) << '.';
                } else {
                    cout << setw(3) << numero;
                }
            }
            cambiarColor(7); // Para que el color no pase al marco.
        }
        cout << "|\n";
    }

    dibujarSeparador(columnas);
    cout << "     # Oculta   B Bandera   * Mina   . Sin minas vecinas\n";
}

// ==================== JUEGO ====================

// Pone o quita la bandera de una casilla y actualiza el contador.
// casilla->bandera es lo mismo que (*casilla).bandera.
void alternarBandera(Casilla* casilla, int* banderas) {
    if (casilla->bandera) {
        casilla->bandera = false;
        (*banderas)--;
    } else {
        casilla->bandera = true;
        (*banderas)++;
    }
}

/*
    Revela una casilla segura. Si es un 0, revela también sus 8 vecinas,
    y así en cadena. Para eso la función se llama a sí misma (recursión).

    La cadena se detiene en el borde, en las casillas con número, en las
    banderas y en las casillas ya reveladas. Revisar "revelada" es lo que
    evita que dos vecinas se llamen entre sí sin fin.
*/
void revelarCasilla(Casilla** tablero, int filas, int columnas, int f, int c) {
    if (f < 0 || f >= filas || c < 0 || c >= columnas) {
        return;
    }
    if (tablero[f][c].revelada || tablero[f][c].bandera || tablero[f][c].esMina) {
        return;
    }

    tablero[f][c].revelada = true;

    // Alrededor de un 0 no hay minas, así que se pueden abrir todas las vecinas.
    if (tablero[f][c].minasVecinas == 0) {
        for (int filaVecina = f - 1; filaVecina <= f + 1; filaVecina++) {
            for (int columnaVecina = c - 1; columnaVecina <= c + 1; columnaVecina++) {
                revelarCasilla(tablero, filas, columnas, filaVecina, columnaVecina);
            }
        }
    }
}

/*
    Revela al azar una casilla segura que siga oculta y sin bandera, y
    escribe su posición en filaPista y columnaPista.
    Devuelve false si no había ninguna disponible.

    Primero cuenta las casillas posibles y luego elige una por su número.
    Así nunca se queda buscando sin fin, como podría pasar probando al azar.
*/
bool usarPista(Casilla** tablero, int filas, int columnas, int* filaPista, int* columnaPista) {
    int candidatas = 0;
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (!tablero[f][c].esMina && !tablero[f][c].revelada && !tablero[f][c].bandera) {
                candidatas++;
            }
        }
    }

    if (candidatas == 0) {
        return false;
    }

    int elegida = rand() % candidatas;

    // Recorremos otra vez, restando 1 en cada candidata hasta llegar a la elegida.
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (!tablero[f][c].esMina && !tablero[f][c].revelada && !tablero[f][c].bandera) {
                if (elegida == 0) {
                    *filaPista = f;
                    *columnaPista = c;
                    revelarCasilla(tablero, filas, columnas, f, c);
                    return true;
                }
                elegida--;
            }
        }
    }
    return false;
}

// Devuelve true si ya no queda ninguna casilla sin mina oculta (el jugador ganó).
bool todasReveladas(Casilla** tablero, int filas, int columnas) {
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (!tablero[f][c].esMina && !tablero[f][c].revelada) {
                return false;
            }
        }
    }
    return true;
}

// Destapa todas las minas para mostrarlas al terminar la partida.
void mostrarMinas(Casilla** tablero, int filas, int columnas) {
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (tablero[f][c].esMina) {
                tablero[f][c].revelada = true;
            }
        }
    }
}

/*
    Juega una partida completa. Cada turno: limpiar pantalla, dibujar,
    pedir la acción y aplicarla. Termina al ganar o al quedarse sin vidas.

    Devuelve true si el jugador ganó, y escribe en jugadas cuántas veces
    reveló casillas (la pista cuenta; las banderas no).
*/
bool jugarPartida(Casilla** tablero, int filas, int columnas, int minas, int* jugadas) {
    int vidas = 3;
    int banderas = 0;
    bool pistaUsada = false;
    *jugadas = 0;

    // Se guarda en un turno y se muestra en el siguiente,
    // porque system("cls") borra la pantalla.
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

        // La pista no pide fila ni columna.
        if (accion == 'P') {
            int filaPista = 0;
            int columnaPista = 0;
            if (pistaUsada) {
                mensaje = "Ya usaste tu pista en esta partida.";
            } else if (usarPista(tablero, filas, columnas, &filaPista, &columnaPista)) {
                pistaUsada = true;
                (*jugadas)++;
                reproducirSonido(SONIDO_PISTA);
                mensaje = "Pista: se reveló la fila " + to_string(filaPista + 1) +
                          ", columna " + to_string(columnaPista + 1) + ".";
            } else {
                mensaje = "No hay casillas para la pista: quita alguna bandera.";
            }
            continue; // Pasa directo al siguiente turno.
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

    // Si todavía hay vidas, el ciclo terminó porque se revelaron todas.
    bool gano = vidas > 0;

    mostrarMinas(tablero, filas, columnas);
    system("cls");
    dibujarTablero(tablero, filas, columnas, vidas, banderas, minas, *jugadas);

    if (gano) {
        cambiarColor(10);
        cout << "\n¡GANASTE en " << *jugadas << " jugadas!\n";
        cambiarColor(7);
        reproducirSonido(SONIDO_VICTORIA);
    } else {
        cambiarColor(12);
        cout << "\nPERDISTE: te quedaste sin vidas.\n";
        cambiarColor(7);
        reproducirSonido(SONIDO_DERROTA);
    }

    return gano;
}

// ==================== RÉCORDS ====================

// Agrega una línea al final de records.txt, por ejemplo: "1 23 Ana Lopez".
// El nombre va al final porque puede tener espacios y así se lee con getline.
void guardarRecord(string nombre, int nivel, int jugadas) {
    ofstream archivo("records.txt", ios::app); // app = agregar sin borrar lo anterior
    archivo << nivel << " " << jugadas << " " << nombre << "\n";
}

/*
    Muestra los 5 mejores de cada dificultad (los de menos jugadas):
    1. Cuenta cuántos récords hay en el archivo.
    2. Reserva un arreglo dinámico de ese tamaño y los lee.
    3. Los ordena de menos a más jugadas (método de la burbuja).
    4. Imprime los primeros 5 de cada nivel.
    5. Libera el arreglo.
*/
void mostrarRecords() {
    cambiarColor(14);
    cout << "\nRÉCORDS (menos jugadas = mejor)\n";
    cambiarColor(7);

    ifstream archivo("records.txt");

    // 1. Contar
    int total = 0;
    int nivel;
    int jugadas;
    string nombre;
    while (archivo >> nivel >> jugadas && getline(archivo, nombre)) {
        total++;
    }

    if (total == 0) {
        cout << "Todavía no hay récords. Gana una partida en Fácil, Medio o Difícil.\n";
        return;
    }

    // 2. Reservar y leer. clear() y seekg(0) regresan al inicio del archivo.
    Record* records = new Record[total];
    archivo.clear();
    archivo.seekg(0);
    for (int i = 0; i < total; i++) {
        archivo >> records[i].nivel >> records[i].jugadas;
        archivo.ignore(1); // Salta el espacio antes del nombre.
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

    // 4. Una tabla por nivel. La posición 0 no se usa.
    string nombresNivel[4] = {"", "FÁCIL (9 x 9)", "MEDIO (16 x 16)", "DIFÍCIL (16 x 30)"};
    for (int n = 1; n <= 3; n++) {
        cambiarColor(11);
        cout << "\n" << nombresNivel[n] << "\n";
        cambiarColor(7);

        int mostrados = 0;
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

    // 5. Liberar
    delete[] records;
}