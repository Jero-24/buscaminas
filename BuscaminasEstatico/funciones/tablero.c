#include "tablero.h"
#include <ctype.h>

// Inicializa el tablero vacío y el temporizador
void inicializarTablero(EstadoJuego *juego, Celda tablero[MAX_TABLERO][MAX_TABLERO]) {
    inicializar_azar(); // función de azar.c
    // No ponemos tiempo aquí, lo pone interfaz.c o main
    for(int f=0; f<juego->filas; f++)
        for(int c=0; c<juego->columnas; c++){
            tablero[f][c].estadoVisible='x';
            tablero[f][c].minasAdyacentes='0';
        }
}

// Comprueba si posición dentro del tablero
int estaDentroTablero(EstadoJuego *juego, int f, int c){
    return f>=0 && f<juego->filas && c>=0 && c<juego->columnas;
}

// Coloca minas al azar
void colocarMinas(EstadoJuego *juego, Celda tablero[MAX_TABLERO][MAX_TABLERO]){
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

// Revela celdas recursivamente
void revelarCascada(EstadoJuego *juego, Celda tablero[MAX_TABLERO][MAX_TABLERO], int f, int c){
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

// Convierte un char a número
int charAInt(char c, int *out){
    if(isalpha(c)) *out = tolower(c)-'a'+10;
    else if(isdigit(c)) *out = c-'0';
    else return 0;
    return 1;
}
