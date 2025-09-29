#include "interfaz.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* 
 * parse_coord:
 *  - Si s es una letra única devuelve (a/A)->10, (b/B)->11, ...
 *  - Si s es una cadena numérica devuelve su valor entero (p. ej. "12" -> 12).
 *  - Devuelve 1 en caso de éxito y 0 en caso de fallo.
 */
static int parse_coord(const char *s, int *out) {
    if (!s || !*s) return 0;

    size_t len = strlen(s);

    /* letra única -> coordenada a partir de 10 */
    if (len == 1 && isalpha((unsigned char)s[0])) {
        *out = tolower((unsigned char)s[0]) - 'a' + 10;
        return 1;
    }

    /* intentamos leer entero decimal */
    char *endptr = NULL;
    long v = strtol(s, &endptr, 10);
    if (endptr != s && *endptr == '\0') {
        *out = (int)v;
        return 1;
    }

    return 0;
}

void mostrarMenu(EstadoJuego *juego) {
    int opcion;
    printf("Menu:\n");
    printf("1) 8x8  10 minas\n");
    printf("2) 16x16 40 minas\n");
    printf("3) 16x30 99 minas\n");
    printf("4) Personalizado\n");
    printf("Elige una opción [1-4]: ");

    if (scanf("%d%*c", &opcion) != 1) exit(1);

    switch (opcion) {
        case 1:
            juego->filas = juego->columnas = 8;  
            juego->numMinas = 10; 
            break;

        case 2:
            juego->filas = juego->columnas = 16; 
            juego->numMinas = 40; 
            break;

        case 3:
            juego->filas = 16; 
            juego->columnas = 30; 
            juego->numMinas = 99; 
            break;

        case 4:
            juego->filas = preguntar_n_en_rango("Filas?", 1, 10000);      // con 50000 explota
            juego->columnas = preguntar_n_en_rango("Columnas?", 1, 10000);
            juego->numMinas = preguntar_n_en_rango(
                "Minas?", 1, juego->filas * juego->columnas - 1
            );
            break;

        default:
            juego->filas = juego->columnas = 8;  
            juego->numMinas = 10;
    }
}

int dibujarTablero(EstadoJuego *juego, Celda **tablero) {
    int descubiertas = 0;
    for (int f = 0; f < juego->filas; f++)
        for (int c = 0; c < juego->columnas; c++)
            if (tablero[f][c].estadoVisible >= '0' && tablero[f][c].estadoVisible <= '9')
                descubiertas++;

    /* Cabecera: tiempo y estado */
    printf("\n[%d] | ", tiempo_transcurrido());

    if (juego->minaExplotada) {
        printf_color(1); printf("X-("); printf_reset_color();
    } else if (juego->minasMarcadasCorrectamente == juego->numMinas &&
               descubiertas + juego->minasMarcadasCorrectamente == juego->filas * juego->columnas) {
        printf_color(2); printf("B-)"); printf_reset_color();
    } else {
        printf(":-)");
    }

    printf(" | Banderas: ");
    if (juego->banderasColocadas > juego->numMinas) printf_color(1);
    printf("%d/%d\n", juego->banderasColocadas, juego->numMinas);
    printf_reset_color();

    /* Encabezado columnas (ancho fijo para alinear) */
    printf("    ");
    for (int c = 0; c < juego->columnas; c++)
        printf("%2d ", c);
    printf("\n");

    /* Filas y celdas: cada celda ocupa 3 caracteres */
    for (int f = 0; f < juego->filas; f++) {
        printf("%2d ", f);
        for (int c = 0; c < juego->columnas; c++) {
            char v = tablero[f][c].estadoVisible;

            /* Si explotó, mostrar todas las minas en rojo */
            if (juego->minaExplotada && tablero[f][c].minasAdyacentes == '*') {
                printf_color(1); printf(" * "); printf_reset_color();
                continue;
            }

            if (v == '0') {
                printf("   "); /* celda vacía */
            } else if (v >= '0' && v <= '9') {
                printf_color(v - '0'); printf(" %c ", v); printf_reset_color();
            } else if (v == '*') { /* mina visible (si en algún modo la mostramos) */
                printf_color(1); printf(" * "); printf_reset_color();
            } else {
                /* símbolos como '!' '?' 'x' */
                printf(" %c ", v);
            }
        }
        printf("\n");
    }

    /* Resultado del juego */
    if (juego->minaExplotada) {
        printf("---> BOOM!\n");
        return -1;
    }
    if (juego->minasMarcadasCorrectamente == juego->numMinas &&
        descubiertas + juego->minasMarcadasCorrectamente == juego->filas * juego->columnas) {
        printf("---> GANASTE!\n");
        return 1;
    }
    return 0;
}

/* Manejar movimiento — ahora acepta coordenadas multi-dígito o letras */
void manejarMovimiento(EstadoJuego *juego, Celda **tablero) {
    char linea[128], t1[32], t2[32], t3[32];

    while (1) {
        printf("Acciones: '!' bandera, '?' duda, 'x' desmarcar, ' ' revelar\n");
        printf("Ingresa movimiento [fila columna acción]: ");

        if (!fgets(linea, sizeof linea, stdin)) exit(0);

        /* Leemos hasta tres tokens: fila, columna, acción (opcional) */
        int n = sscanf(linea, " %31s %31s %31s", t1, t2, t3);
        if (n < 2) { printf("Entrada inválida\n"); continue; }

        int fila, col;
        if (!parse_coord(t1, &fila) || !parse_coord(t2, &col)) { printf("Posición inválida\n"); continue; }

        char accion;
        if (n == 2) accion = '0';           /* revelar por omisión */
        else { accion = t3[0]; if (accion == ' ') accion = '0'; }

        /* Validar rango */
        if (!estaDentroTablero(juego, fila, col)) { printf("Fuera de rango\n"); continue; }

        /* Validar acción sobre estado actual */
        char actual = tablero[fila][col].estadoVisible;
        int valido = 0;
        if (accion == '0' && actual != '!') valido = 1;
        else if (accion == '!' || accion == '?' || accion == 'x') valido = 1;
        if (!valido) { printf("Acción inválida\n"); continue; }

        /* Aplicar acción */
        juego->filaSeleccionada = fila;
        juego->columnaSeleccionada = col;

        switch (accion) {
            case '0':
                if (juego->primerMovimiento) {
                    juego->primerMovimiento = FALSO;
                    colocarMinas(juego, tablero);
                }
                if (tablero[fila][col].minasAdyacentes == '*') juego->minaExplotada = VERDADERO;
                revelarCascada(juego, tablero, fila, col);
                break;

            case '!':
                juego->banderasColocadas++;
                if (tablero[fila][col].minasAdyacentes == '*') juego->minasMarcadasCorrectamente++;
                tablero[fila][col].estadoVisible = '!';
                break;

            default:
                if (actual == '!') {
                    juego->banderasColocadas--;
                    if (tablero[fila][col].minasAdyacentes == '*') juego->minasMarcadasCorrectamente--;
                }
                tablero[fila][col].estadoVisible = accion;
                break;
        }

        break; /* salir del while después de un movimiento válido */
    }
}

