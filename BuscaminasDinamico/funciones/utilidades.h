#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// Funciones de azar
void inicializar_azar();
int numero_al_azar(int max);

// Funciones de color en terminal
void printf_color(int color);
void printf_reset_color();

// Funciones de tiempo
void empieza_tiempo();
int tiempo_transcurrido();

// Funciones de input
int preguntar_n_en_rango(char *s, int min, int max);
int preguntar_si_o_no(char *s);

#endif
