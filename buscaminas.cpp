/*
    BUSCAMINAS
    Por: Leo y Gamma

    Lo que le agregamos al Buscaminas normal:
    - Tablero personalizado (con memoria dinámica)
    - 3 vidas
    - Una pista por partida
    - Top 5 de récords guardado en records.txt
    - Sonidos con Beep

    Para compilar:
    g++ -static -std=c++11 -Wall -Wextra buscaminas.cpp -o buscaminas.exe
    (-static es para que el .exe abra en cualquier computadora)
*/

#include <iostream>
#include <iomanip>   // setw, left y right
#include <fstream>   // para el archivo de récords
#include <string>
#include <cstdlib>   // rand, srand y system
#include <ctime>     // time
#include <cctype>    // toupper
#include <windows.h> // colores, Beep y acentos

using namespace std;

// ==================== ESTRUCTURAS ====================

struct Casilla {
    bool esMina;
    bool revelada;
    bool bandera;
    int minasVecinas; // de 0 a 8
};

struct Record {
    int nivel; // 1 = Fácil, 2 = Medio, 3 = Difícil
    int jugadas;
    string nombre;
};

// ==================== DECLARACIONES ====================

// Consola
void cambiarColor(int color);
int leerEnteroEnRango(int minimo, int maximo);
char leerAccion();

// Menú
void mostrarTitulo();
int mostrarMenu();
void mostrarInstrucciones();

// Configuración
int elegirDificultad(int* filas, int* columnas, int* minas);

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
    SetConsoleOutputCP(CP_UTF8); // para que se vean los acentos
    srand(time(0));              // para que las minas cambien en cada partida

    int opcion = mostrarMenu();
    while (opcion != 0) {
        if (opcion == 1) {
            int filas = 0;
            int columnas = 0;
            int minas = 0;
            // Mandamos las direcciones (&) para que la función llene las variables.
            int nivel = elegirDificultad(&filas, &columnas, &minas);

            // Si eligió "Volver", filas sigue en 0 y no se juega.
            if (filas > 0) {
                Casilla** tablero = crearTablero(filas, columnas);
                colocarMinas(tablero, filas, columnas, minas);
                calcularNumeros(tablero, filas, columnas);

                int jugadas = 0;
                bool gano = jugarPartida(tablero, filas, columnas, minas, &jugadas);

                // Solo guardamos récord en Fácil, Medio o Difícil (no en personalizado).
                if (gano && nivel != 4) {
                    cout << "\nEscribe tu nombre para el récord (sin acentos ni ñ): ";
                    string nombre;
                    getline(cin, nombre); // getline para que acepte espacios
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
            system("pause");
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

// Cambia el color del texto. 7 = gris, 10 = verde, 12 = rojo, 14 = amarillo.
void cambiarColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Pide un número entre minimo y maximo. Si escriben letras o un número
// fuera del rango, lo vuelve a pedir.
int leerEnteroEnRango(int minimo, int maximo) {
    int numero;
    cin >> numero;
    while (cin.fail() || numero < minimo || numero > maximo) {
        cin.clear();            // quita el error si escribieron letras
        cin.ignore(1000, '\n'); // borra lo que escribieron
        cout << "Entrada inválida. Escribe un número entre "
             << minimo << " y " << maximo << ": ";
        cin >> numero;
    }
    cin.ignore(1000, '\n'); // borra el Enter para que getline funcione después
    return numero;
}

// Pide la acción del turno: R, B o P (también acepta minúsculas).
char leerAccion() {
    char letra;
    cin >> letra;
    cin.ignore(1000, '\n');
    letra = toupper(letra);
    while (letra != 'R' && letra != 'B' && letra != 'P') {
        cout << "Acción inválida. Escribe R, B o P: ";
        cin >> letra;
        cin.ignore(1000, '\n');
        letra = toupper(letra);
    }
    return letra;
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

// Muestra el menú y regresa la opción elegida.
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

// Pide la dificultad y llena filas, columnas y minas.
// Regresa la opción: 1 = Fácil, 2 = Medio, 3 = Difícil, 4 = Personalizado, 0 = Volver.
int elegirDificultad(int* filas, int* columnas, int* minas) {
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
        cout << "Filas (5 a 20): ";
        *filas = leerEnteroEnRango(5, 20);
        cout << "Columnas (5 a 30): ";
        *columnas = leerEnteroEnRango(5, 30);

        // Le restamos 1 para que siempre quede al menos una casilla sin mina.
        int maximoMinas = *filas * *columnas - 1;
        cout << "Minas (1 a " << maximoMinas << "): ";
        *minas = leerEnteroEnRango(1, maximoMinas);
    }
    return opcion;
}

// ==================== TABLERO ====================

/*
    Crea el tablero con memoria dinámica.
    Casilla** es un puntero a punteros: un arreglo de filas,
    y cada fila es un arreglo de casillas.

    tablero --> [fila 0] --> [Casilla][Casilla][Casilla]...
                [fila 1] --> [Casilla][Casilla][Casilla]...
                [fila 2] --> [Casilla][Casilla][Casilla]...
*/
Casilla** crearTablero(int filas, int columnas) {
    Casilla** tablero = new Casilla*[filas];

    for (int f = 0; f < filas; f++) {
        tablero[f] = new Casilla[columnas];
        for (int c = 0; c < columnas; c++) {
            tablero[f][c].esMina = false;
            tablero[f][c].revelada = false;
            tablero[f][c].bandera = false;
            tablero[f][c].minasVecinas = 0;
        }
    }
    return tablero;
}

// Libera la memoria: primero cada fila y al final el arreglo de filas.
void liberarTablero(Casilla** tablero, int filas) {
    for (int f = 0; f < filas; f++) {
        delete[] tablero[f];
    }
    delete[] tablero;
}

// Pone las minas al azar sin repetir casilla.
void colocarMinas(Casilla** tablero, int filas, int columnas, int minas) {
    int colocadas = 0;
    while (colocadas < minas) {
        int f = rand() % filas;
        int c = rand() % columnas;

        if (!tablero[f][c].esMina) {
            tablero[f][c].esMina = true;
            colocadas++;
        }
    }
}

// Cuenta las minas que hay en las 8 casillas alrededor de [f][c].
int contarVecinas(Casilla** tablero, int filas, int columnas, int f, int c) {
    int cantidad = 0;
    for (int filaVecina = f - 1; filaVecina <= f + 1; filaVecina++) {
        for (int columnaVecina = c - 1; columnaVecina <= c + 1; columnaVecina++) {
            // Revisamos que no se salga del tablero.
            if (filaVecina >= 0 && filaVecina < filas &&
                columnaVecina >= 0 && columnaVecina < columnas) {
                if (tablero[filaVecina][columnaVecina].esMina) {
                    cantidad++;
                }
            }
        }
    }
    return cantidad;
}

// Le pone a cada casilla sin mina su número de minas vecinas.
void calcularNumeros(Casilla** tablero, int filas, int columnas) {
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (!tablero[f][c].esMina) {
                tablero[f][c].minasVecinas = contarVecinas(tablero, filas, columnas, f, c);
            }
        }
    }
}

// Dibuja la línea de arriba o de abajo del tablero.
void dibujarSeparador(int columnas) {
    cout << "    +";
    for (int c = 0; c < columnas; c++) {
        cout << "---";
    }
    cout << "+\n";
}

// Dibuja los datos de la partida y el tablero con colores.
void dibujarTablero(Casilla** tablero, int filas, int columnas,
                    int vidas, int banderas, int minas, int jugadas) {
    // Un color para cada número del 0 al 8.
    int colores[9] = {7, 9, 10, 12, 13, 14, 11, 15, 8};

    cambiarColor(15);
    cout << "\nVidas: " << vidas << "  Banderas: " << banderas
         << "  Minas: " << minas << "  Jugadas: " << jugadas << "\n\n";
    cambiarColor(7);

    // Números de las columnas
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
            cambiarColor(7);
        }
        cout << "|\n";
    }

    dibujarSeparador(columnas);
    cout << "     # Oculta   B Bandera   * Mina   . Sin minas vecinas\n";
}

// ==================== JUEGO ====================

// Pone o quita la bandera y actualiza el contador.
// casilla->bandera es lo mismo que (*casilla).bandera
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
    Revela una casilla. Si es 0, también revela sus 8 vecinas,
    y si alguna de esas es 0 sigue con las suyas (recursión: la función
    se llama a sí misma). Se detiene en los bordes, en las banderas,
    en las minas y en las casillas que ya estaban reveladas.
*/
void revelarCasilla(Casilla** tablero, int filas, int columnas, int f, int c) {
    if (f < 0 || f >= filas || c < 0 || c >= columnas) {
        return;
    }
    if (tablero[f][c].revelada || tablero[f][c].bandera || tablero[f][c].esMina) {
        return;
    }

    tablero[f][c].revelada = true;

    if (tablero[f][c].minasVecinas == 0) {
        for (int filaVecina = f - 1; filaVecina <= f + 1; filaVecina++) {
            for (int columnaVecina = c - 1; columnaVecina <= c + 1; columnaVecina++) {
                revelarCasilla(tablero, filas, columnas, filaVecina, columnaVecina);
            }
        }
    }
}

// Revela una casilla segura al azar y guarda su posición en filaPista y columnaPista.
// Regresa false si no hay ninguna casilla disponible.
bool usarPista(Casilla** tablero, int filas, int columnas, int* filaPista, int* columnaPista) {
    // Primero revisamos que quede alguna casilla sin mina, oculta y sin bandera.
    bool hayCasilla = false;
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (!tablero[f][c].esMina && !tablero[f][c].revelada && !tablero[f][c].bandera) {
                hayCasilla = true;
            }
        }
    }
    if (!hayCasilla) {
        return false;
    }

    // Probamos casillas al azar hasta encontrar una buena.
    while (true) {
        int f = rand() % filas;
        int c = rand() % columnas;
        if (!tablero[f][c].esMina && !tablero[f][c].revelada && !tablero[f][c].bandera) {
            *filaPista = f;
            *columnaPista = c;
            revelarCasilla(tablero, filas, columnas, f, c);
            return true;
        }
    }
}

// Regresa true si ya se revelaron todas las casillas sin mina (ganó).
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

// Destapa todas las minas al terminar la partida.
void mostrarMinas(Casilla** tablero, int filas, int columnas) {
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (tablero[f][c].esMina) {
                tablero[f][c].revelada = true;
            }
        }
    }
}

// Juega una partida completa. Regresa true si ganó y guarda en jugadas
// cuántas veces reveló (la pista cuenta, las banderas no).
bool jugarPartida(Casilla** tablero, int filas, int columnas, int minas, int* jugadas) {
    int vidas = 3;
    int banderas = 0;
    bool pistaUsada = false;
    *jugadas = 0;

    // El mensaje se muestra en el siguiente turno porque system("cls") borra todo.
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

        // La pista no necesita fila ni columna.
        if (accion == 'P') {
            int filaPista = 0;
            int columnaPista = 0;
            if (pistaUsada) {
                mensaje = "Ya usaste tu pista en esta partida.";
            } else if (usarPista(tablero, filas, columnas, &filaPista, &columnaPista)) {
                pistaUsada = true;
                (*jugadas)++;
                Beep(880, 80);
                Beep(1175, 120);
                mensaje = "Pista: se reveló la fila " + to_string(filaPista + 1) +
                          ", columna " + to_string(columnaPista + 1) + ".";
            } else {
                mensaje = "No hay casillas para la pista: quita alguna bandera.";
            }
            continue; // se salta al siguiente turno
        }

        cout << "Fila (1 a " << filas << "): ";
        int fila = leerEnteroEnRango(1, filas);
        cout << "Columna (1 a " << columnas << "): ";
        int columna = leerEnteroEnRango(1, columnas);

        // Le restamos 1 porque la matriz empieza en 0.
        Casilla* casilla = &tablero[fila - 1][columna - 1];

        if (accion == 'B') {
            if (casilla->revelada) {
                mensaje = "Esa casilla ya está destapada, elige otra.";
            } else {
                alternarBandera(casilla, &banderas);
                Beep(1200, 40);
            }
        } else if (casilla->bandera) {
            mensaje = "Esa casilla tiene bandera. Si quieres revelarla, primero quítala con B.";
        } else if (casilla->revelada) {
            mensaje = "Esa casilla ya está destapada, elige otra.";
        } else if (casilla->esMina) {
            casilla->revelada = true;
            vidas--;
            (*jugadas)++;
            mensaje = "¡BOOM! Pisaste una mina y perdiste una vida.";
            Beep(150, 400);
        } else {
            revelarCasilla(tablero, filas, columnas, fila - 1, columna - 1);
            (*jugadas)++;
            Beep(900, 40);
        }
    }

    // Si todavía tiene vidas, es porque reveló todo.
    bool gano = vidas > 0;

    mostrarMinas(tablero, filas, columnas);
    system("cls");
    dibujarTablero(tablero, filas, columnas, vidas, banderas, minas, *jugadas);

    if (gano) {
        cambiarColor(10);
        cout << "\n¡GANASTE en " << *jugadas << " jugadas!\n";
        cambiarColor(7);
        // Do, Mi, Sol, Do
        Beep(523, 150);
        Beep(659, 150);
        Beep(784, 150);
        Beep(1047, 400);
    } else {
        cambiarColor(12);
        cout << "\nPERDISTE: te quedaste sin vidas.\n";
        cambiarColor(7);
        Beep(400, 300);
        Beep(300, 300);
        Beep(200, 600);
    }

    return gano;
}

// ==================== RÉCORDS ====================

// Agrega una línea al final de records.txt, por ejemplo: "1 23 Ana Lopez".
void guardarRecord(string nombre, int nivel, int jugadas) {
    ofstream archivo("records.txt", ios::app); // app = agregar sin borrar lo que había
    archivo << nivel << " " << jugadas << " " << nombre << "\n";
}

/*
    Muestra los 5 mejores de cada dificultad (los de menos jugadas):
    1. Cuenta cuántos récords hay en el archivo.
    2. Crea un arreglo dinámico de ese tamaño y los lee.
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

    // 2. Crear el arreglo y leer. clear() y seekg(0) regresan al inicio del archivo.
    Record* records = new Record[total];
    archivo.clear();
    archivo.seekg(0);
    for (int i = 0; i < total; i++) {
        archivo >> records[i].nivel >> records[i].jugadas;
        archivo.ignore(1); // salta el espacio antes del nombre
        getline(archivo, records[i].nombre);
    }

    // 3. Burbuja
    for (int pasada = 0; pasada < total - 1; pasada++) {
        for (int i = 0; i < total - 1 - pasada; i++) {
            if (records[i].jugadas > records[i + 1].jugadas) {
                Record temporal = records[i];
                records[i] = records[i + 1];
                records[i + 1] = temporal;
            }
        }
    }

    // 4. Imprimir. La posición 0 no se usa.
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
