#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "tablero.h"

// Colores en terminal
void printf_color(int color);
void printf_reset_color();

// Mostrar menú de selección de dificultad
void mostrarMenu(EstadoJuego *juego);

// Dibujar tablero y estado del juego
int dibujarTablero(EstadoJuego *juego, Celda tablero[MAX_TABLERO][MAX_TABLERO]);

// Manejar movimiento del jugador
void manejarMovimiento(EstadoJuego *juego, Celda tablero[MAX_TABLERO][MAX_TABLERO]);

#endif
