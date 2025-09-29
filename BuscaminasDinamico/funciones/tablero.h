#ifndef TABLERO_H
#define TABLERO_H

#include "utilidades.h"

// Constantes
#define VERDADERO 1
#define FALSO 0

// Celda
typedef struct {
    char estadoVisible;    // 'x', '!', '?', '0'..'8', '*'
    char minasAdyacentes;  // '0'..'8' o '*' si es mina
} Celda;

// Estado del juego
typedef struct {
    int filas, columnas;
    int numMinas;
    int banderasColocadas;
    int minasMarcadasCorrectamente;
    int minaExplotada;
    int primerMovimiento;
    int filaSeleccionada, columnaSeleccionada;
} EstadoJuego;

// Prototipos
Celda **crearTablero(int filas, int columnas);
void liberarTablero(Celda **tablero, int filas);
void inicializarTablero(EstadoJuego *juego, Celda **tablero);
int estaDentroTablero(EstadoJuego *juego, int f, int c);
void colocarMinas(EstadoJuego *juego, Celda **tablero);
void revelarCascada(EstadoJuego *juego, Celda **tablero, int f, int c);
int charAInt(char c, int *out);

#endif
