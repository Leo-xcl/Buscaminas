/*
    Este archivo contiene las declaraciones compartidas del proyecto.
    Una declaración anuncia el nombre, los parámetros y el resultado de una
    función. Sus instrucciones están en el .cpp que tiene su mismo nombre.

    Por ejemplo, crearTablero se declara aquí y se desarrolla en juego/crearTablero.cpp.
    Así los archivos pueden llamarse entre sí sin repetir las implementaciones.

    Se usa la extensión .cpp para mantener todas las piezas en archivos C++.
    Este archivo se incluye con #include porque solo contiene declaraciones.
    #ifndef, #define y #endif evitan procesarlo dos veces dentro del mismo archivo.
    #ifndef comprueba si falta la marca; #define la crea; #endif cierra ese bloque.
    En los archivos de otras carpetas, ../compartido/ significa subir un nivel
    y entrar en compartido para encontrar estas declaraciones.

    Esta lista sirve como índice. La explicación completa, los requisitos y
    los pasos de cada función están sobre su implementación en el otro .cpp.
*/
#ifndef BUSCAMINAS_DECLARACIONES_CPP
#define BUSCAMINAS_DECLARACIONES_CPP

#include "casilla.cpp" // Comparte la definición del struct Casilla.
#include <string>      // std::string: nombre del jugador en los récords.

// Color y presentación del menú.
// color es el código de Windows para los textos siguientes. No devuelve un valor.
void cambiarColor(int color);

// No recibe parámetros ni devuelve un valor; imprime el encabezado del programa.
void mostrarTitulo();

// No recibe parámetros. Devuelve 1 = jugar, 2 = récords, 3 = cómo jugar o 0 = salir.
int mostrarMenu();

// No recibe parámetros ni devuelve un valor; explica las reglas, controles y símbolos.
void mostrarInstrucciones();

// Lectura de números y selección de dificultad.
// Devuelve un entero entre ambos límites, o 0 si no puede seguir leyendo.
int leerEnteroEnRango(int minimo, int maximo);

// No recibe parámetros. Devuelve 'R', 'B' o 'P', o 0 si no puede seguir leyendo.
char leerAccion();

// Efectos de sonido. Cada constante es un tipo que entiende reproducirSonido.
const int SONIDO_REVELAR = 1;
const int SONIDO_BANDERA = 2;
const int SONIDO_MINA = 3;
const int SONIDO_PISTA = 4;
const int SONIDO_VICTORIA = 5;
const int SONIDO_DERROTA = 6;

// Recibe una constante SONIDO_ y toca su efecto con Beep. No devuelve un valor.
void reproducirSonido(int tipo);

// Recibe direcciones de variables int para escribir dimensiones y minas en ellas.
// No devuelve un valor; deja las tres variables en 0 si se cancela o no hay datos.
void elegirDificultad(int* filas, int* columnas, int* minas);

// Devuelve 1 = Fácil, 2 = Medio, 3 = Difícil o 0 = Personalizado.
int calcularNivel(int filas, int columnas, int minas);

// Reserva y liberación de la matriz dinámica.
// Recibe el tamaño y devuelve el puntero al tablero inicializado, todavía sin minas.
Casilla** crearTablero(int filas, int columnas);

// Recibe un tablero y su cantidad original de filas; libera la memoria una sola vez.
// No devuelve un valor. Después de esta llamada el tablero ya no se puede usar.
void liberarTablero(Casilla** tablero, int filas);

// Colocación de minas y cálculo de los números de las casillas.
// Recibe un tablero nuevo, su tamaño y la cantidad de minas; modifica esMina.
// No devuelve un valor; las minas se colocan en posiciones distintas.
void colocarMinas(Casilla** tablero, int filas, int columnas, int minas);

// Recibe el tablero, sus dimensiones y la posición [f][c] que se quiere consultar.
// Devuelve entre 0 y 8 minas vecinas, sin contar la propia casilla ni modificarla.
int contarVecinas(Casilla** tablero, int filas, int columnas, int f, int c);

// Recibe el tablero y su tamaño; escribe minasVecinas después de colocar las minas.
// No devuelve un valor: los resultados quedan guardados dentro de las casillas.
void calcularNumeros(Casilla** tablero, int filas, int columnas);

// Dibujo del marco, las casillas y los contadores.
// Recibe el ancho en columnas e imprime un borde horizontal. No devuelve un valor.
void dibujarSeparador(int columnas);

// Recibe la matriz, sus dimensiones y cuatro contadores para mostrarlos en pantalla.
// banderas indica las colocadas. No modifica esos datos ni devuelve un valor.
void dibujarTablero(Casilla** tablero, int filas, int columnas,
                    int vidas, int banderas, int minas, int jugadas);

// Ciclo de juego.
// Recibe una casilla oculta y el contador; pone o quita su bandera.
void alternarBandera(Casilla* casilla, int* banderas);

// Revela [f][c] y, si es un 0, sus vecinas en cadena. No revela minas.
void revelarCasilla(Casilla** tablero, int filas, int columnas, int f, int c);

// Revela al azar una casilla segura y escribe su posición. false si no hay ninguna.
bool usarPista(Casilla** tablero, int filas, int columnas, int* filaPista, int* columnaPista);

// Devuelve true si todas las casillas sin mina ya están reveladas.
bool todasReveladas(Casilla** tablero, int filas, int columnas);

// Marca todas las minas como reveladas para mostrarlas al final.
void mostrarMinas(Casilla** tablero, int filas, int columnas);

// Juega una partida completa. Devuelve true si se gana y escribe las jugadas.
bool jugarPartida(Casilla** tablero, int filas, int columnas, int minas, int* jugadas);

// Récords.
// Agrega una línea a records.txt con el nivel, las jugadas y el nombre.
void guardarRecord(std::string nombre, int nivel, int jugadas);

// Lee records.txt y muestra el Top 5 de cada dificultad.
void mostrarRecords();

#endif // BUSCAMINAS_DECLARACIONES_CPP
