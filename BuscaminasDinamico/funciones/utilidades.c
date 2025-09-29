#include "utilidades.h"

// Variables de tiempo
static time_t start, end;

// -----------------------------------
// Azar
// -----------------------------------
void inicializar_azar() {
    srand((unsigned)time(NULL));
}

int numero_al_azar(int max) {
    return rand() % max;
}

// -----------------------------------
// Colores
// -----------------------------------
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BOLD    "\x1b[1m"
#define ANSI_COLOR_BLACK   "\x1b[30m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_WHITE   "\x1b[37m"

void printf_color(int color) {
    switch (color) {
        case -1: printf(ANSI_COLOR_RESET); break;
        case 0:  printf(ANSI_COLOR_BOLD ANSI_COLOR_WHITE); break;
        case 1:  printf(ANSI_COLOR_BOLD ANSI_COLOR_RED); break;
        case 2:  printf(ANSI_COLOR_BOLD ANSI_COLOR_GREEN); break;
        case 3:  printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW); break;
        case 4:  printf(ANSI_COLOR_BOLD ANSI_COLOR_BLUE); break;
        case 5:  printf(ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA); break;
        case 6:  printf(ANSI_COLOR_BOLD ANSI_COLOR_CYAN); break;
        case 7:  printf(ANSI_COLOR_BOLD ANSI_COLOR_RED); break;
        case 8:  printf(ANSI_COLOR_BOLD ANSI_COLOR_GREEN); break;
        case 9:  printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW); break;
    }
}


void printf_reset_color() {
    printf_color(-1);
}

// -----------------------------------
// Tiempo
// -----------------------------------
void empieza_tiempo() {
    time(&start);
}

int tiempo_transcurrido() {
    time(&end);
    return (int)difftime(end, start);
}

// -----------------------------------
// Preguntas
// -----------------------------------
static int fuera_de_rango(int n, int min, int max) {
    return n < min || n > max;
}

static void convertir_a_minusculas(char *c) {
    if(*c >= 'A' && *c <= 'Z') *c = *c - 'A' + 'a';
}

int preguntar_n_en_rango(char *s, int min, int max) {
    int n;
    do {
        printf("%s [%d-%d]: ", s, min, max);
        scanf("%d%*c", &n);
    } while(fuera_de_rango(n, min, max));
    return n;
}

int preguntar_si_o_no(char *s) {
    char c;
    do {
        printf("%s [s/n]: ", s);
        scanf("%c%*c", &c);
        convertir_a_minusculas(&c);
    } while(c != 's' && c != 'n');
    return c == 's';
}
