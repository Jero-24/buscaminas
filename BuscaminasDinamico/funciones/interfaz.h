#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "tablero.h"

// Menú
void mostrarMenu(EstadoJuego *juego);

// Dibujar tablero y manejar movimiento
int dibujarTablero(EstadoJuego *juego, Celda **tablero);
void manejarMovimiento(EstadoJuego *juego, Celda **tablero);

#endif
