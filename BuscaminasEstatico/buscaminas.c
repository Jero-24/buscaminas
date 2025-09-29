#include "funciones/tablero.h"
#include "funciones/interfaz.h"
#include "funciones/tiempo.h"

int main(){
    EstadoJuego juego={0};
    juego.primerMovimiento=1;
    juego.minaExplotada=0;
    juego.minasMarcadasCorrectamente=0;
    juego.banderasColocadas=0;
    juego.filaSeleccionada=juego.columnaSeleccionada=-1;

    mostrarMenu(&juego);

    Celda tablero[MAX_TABLERO][MAX_TABLERO];
    inicializarTablero(&juego, tablero);
    empieza_tiempo();

    int estado;
    do{
        estado=dibujarTablero(&juego, tablero);
        if(estado==0) manejarMovimiento(&juego, tablero);
    }while(estado==0);

    return 0;
}

