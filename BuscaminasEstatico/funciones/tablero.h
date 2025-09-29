#ifndef TABLERO_H
#define TABLERO_H

#include <stdio.h>
#include <stdlib.h>
#include "azar.h"

#define MAX_TABLERO 36

typedef struct {
    char estadoVisible;    // 'x', '!', '?', '0'..'8', '*'
    char minasAdyacentes;  // '0'..'8' o '*' si es mina
} Celda;

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
void inicializarTablero(EstadoJuego *juego, Celda tablero[MAX_TABLERO][MAX_TABLERO]);
int estaDentroTablero(EstadoJuego *juego, int f, int c);
void colocarMinas(EstadoJuego *juego, Celda tablero[MAX_TABLERO][MAX_TABLERO]);
void revelarCascada(EstadoJuego *juego, Celda tablero[MAX_TABLERO][MAX_TABLERO], int f, int c);
int charAInt(char c, int *out);

#endif
