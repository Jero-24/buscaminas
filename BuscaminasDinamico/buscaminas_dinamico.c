#include "funciones/interfaz.h"

int main(){
    EstadoJuego juego={0};
    juego.primerMovimiento=VERDADERO;
    juego.minaExplotada=FALSO;
    juego.minasMarcadasCorrectamente=0;
    juego.banderasColocadas=0;
    juego.filaSeleccionada=juego.columnaSeleccionada=-1;

    mostrarMenu(&juego);

    Celda **tablero = crearTablero(juego.filas,juego.columnas);
    inicializarTablero(&juego,tablero);

    int estado;
    do{
        estado=dibujarTablero(&juego,tablero);
        if(estado==0) manejarMovimiento(&juego,tablero);
    }while(estado==0);

    liberarTablero(tablero,juego.filas);
    return 0;
}
