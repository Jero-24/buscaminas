#include "tablero.h"
#include <ctype.h>

// Crear tablero dinámico
Celda **crearTablero(int filas, int columnas) {
    Celda **tablero = malloc(filas * sizeof(Celda *));
    if(!tablero) { perror("malloc"); exit(1); }

    for(int f=0; f<filas; f++) {
        tablero[f] = malloc(columnas * sizeof(Celda));
        if(!tablero[f]) { perror("malloc"); exit(1); }
    }
    return tablero;
}

// Liberar memoria
void liberarTablero(Celda **tablero, int filas){
    for(int f=0; f<filas; f++) free(tablero[f]);
    free(tablero);
}

// Inicializar tablero con valores por defecto
void inicializarTablero(EstadoJuego *juego, Celda **tablero) {
    inicializar_azar();
    empieza_tiempo();

    for (int f = 0; f < juego->filas; f++)
        for (int c = 0; c < juego->columnas; c++) {
            tablero[f][c].estadoVisible = 'x';
            tablero[f][c].minasAdyacentes = '0';
        }
}

// Comprobar dentro del tablero
int estaDentroTablero(EstadoJuego *juego, int f, int c){
    return f>=0 && f<juego->filas && c>=0 && c<juego->columnas;
}

// Convertir char a int
int charAInt(char c, int *out){
    if(isalpha(c)) *out = tolower(c)-'a'+10;
    else if(isdigit(c)) *out = c-'0';
    else return 0;
    return 1;
}

// Colocar minas
void colocarMinas(EstadoJuego *juego, Celda **tablero){
    for(int k=0; k<juego->numMinas; k++){
        int f,c;
        do{
            f = numero_al_azar(juego->filas);
            c = numero_al_azar(juego->columnas);
        } while(tablero[f][c].minasAdyacentes=='*' || (f==juego->filaSeleccionada && c==juego->columnaSeleccionada));

        tablero[f][c].minasAdyacentes='*';

        for(int df=-1; df<=1; df++)
            for(int dc=-1; dc<=1; dc++)
                if(estaDentroTablero(juego,f+df,c+dc) && tablero[f+df][c+dc].minasAdyacentes!='*')
                    tablero[f+df][c+dc].minasAdyacentes++;
    }
}

// Revelar cascada
void revelarCascada(EstadoJuego *juego, Celda **tablero, int f, int c){
    if(!estaDentroTablero(juego,f,c)) return;
    Celda *celda = &tablero[f][c];
    if(celda->estadoVisible!='x') return;

    celda->estadoVisible = celda->minasAdyacentes;

    if(celda->minasAdyacentes=='0'){
        for(int df=-1; df<=1; df++)
            for(int dc=-1; dc<=1; dc++)
                if(df!=0 || dc!=0)
                    revelarCascada(juego, tablero, f+df, c+dc);
    }
}

