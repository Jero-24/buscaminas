#include "interfaz.h"
#include "preguntas.h"
#include "tiempo.h"

// Colores ANSI
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BOLD    "\x1b[1m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_WHITE   "\x1b[37m"

// Imprimir color según número
void printf_color(int color){
    switch(color){
        case -1: printf(ANSI_COLOR_RESET); break;
        case 0: printf(ANSI_COLOR_BOLD ANSI_COLOR_WHITE); break;
        case 1: printf(ANSI_COLOR_BOLD ANSI_COLOR_RED); break;
        case 2: printf(ANSI_COLOR_BOLD ANSI_COLOR_GREEN); break;
        case 3: printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW); break;
    }
}

void printf_reset_color(){ printf_color(-1); }

// Mostrar menú de dificultad
void mostrarMenu(EstadoJuego *juego){
    int opcion;
    printf("Menu:\n1) 8x8\t10 minas\n2) 16x16\t40 minas\n3) 16x30\t99 minas\n4) Personalizado\nElige una opción [1-4]: ");
    if(scanf("%d%*c", &opcion)!=1) exit(1);
    switch(opcion){
        case 1: juego->filas=juego->columnas=8; juego->numMinas=10; break;
        case 2: juego->filas=juego->columnas=16; juego->numMinas=40; break;
        case 3: juego->filas=16; juego->columnas=30; juego->numMinas=99; break;
        case 4:
            juego->filas = preguntar_n_en_rango("Filas? (max 36)",4,36);
            juego->columnas = preguntar_n_en_rango("Columnas? (max 36)",3,36);
            juego->numMinas = preguntar_n_en_rango("Minas?",1,juego->filas*juego->columnas-9);
            break;
        default: juego->filas=juego->columnas=8; juego->numMinas=10;
    }
}

// Dibujar tablero y estado
int dibujarTablero(EstadoJuego *juego, Celda tablero[MAX_TABLERO][MAX_TABLERO]){
    int descubiertas=0;
    for(int f=0; f<juego->filas; f++)
        for(int c=0; c<juego->columnas; c++)
            if(tablero[f][c].estadoVisible>='0' && tablero[f][c].estadoVisible<='9') descubiertas++;

    printf("\n[%d] | ", tiempo_transcurrido());
    if(juego->minaExplotada){ printf_color(1); printf("X-("); printf_reset_color();}
    else if(juego->minasMarcadasCorrectamente==juego->numMinas &&
            descubiertas+juego->minasMarcadasCorrectamente==juego->filas*juego->columnas){
        printf_color(2); printf("B-)"); printf_reset_color();
    } else printf(":-)");

    printf(" | Banderas: ");
    if(juego->banderasColocadas>juego->numMinas) printf_color(1);
    printf("%d/%d\n",juego->banderasColocadas,juego->numMinas);
    printf_reset_color();

    printf("   ");
    for(int c=0;c<juego->columnas;c++) printf(c<10?"%d ":"%c ", c<10?c:'A'+c-10);
    printf("\n");

    for(int f=0;f<juego->filas;f++){
        printf(f<10?" %d ":" %c ", f<10?f:'A'+f-10);
        for(int c=0;c<juego->columnas;c++){
            char v=tablero[f][c].estadoVisible;
            if(juego->minaExplotada && tablero[f][c].minasAdyacentes=='*'){ printf_color(1); printf("* "); printf_reset_color(); continue;}
            if(v=='0') printf("  ");
            else if(v>='0' && v<='9'){ printf_color(v-'0'); printf("%c ",v); printf_reset_color();}
            else { if(v=='*') printf_color(1); printf("%c ",v); printf_reset_color();}
        }
        printf("\n");
    }

    if(juego->minaExplotada){ printf("---> BOOM!\n"); return -1;}
    if(juego->minasMarcadasCorrectamente==juego->numMinas &&
       descubiertas+juego->minasMarcadasCorrectamente==juego->filas*juego->columnas){ 
        printf("---> GANASTE!\n"); return 1;
    }

    return 0;
}

// Manejar entrada del jugador
void manejarMovimiento(EstadoJuego *juego, Celda tablero[MAX_TABLERO][MAX_TABLERO]){
    char linea[100],rf,cf,accion;
    while(1){
        printf("Acciones: '!' bandera, '?' duda, 'x' desmarcar, ' ' revelar\n");
        printf("Ingresa movimiento [fila columna acción]: ");
        if(!fgets(linea,sizeof linea,stdin)) exit(0);
        int n=sscanf(linea," %c %c %c",&rf,&cf,&accion);
        if(n<2){printf("Entrada inválida\n"); continue;}
        if(n==2) accion=' '; if(accion==' ') accion='0';
        if(!charAInt(rf,&juego->filaSeleccionada) || !charAInt(cf,&juego->columnaSeleccionada)){printf("Posición inválida\n"); continue;}
        if(juego->filaSeleccionada<0 || juego->filaSeleccionada>=juego->filas ||
           juego->columnaSeleccionada<0 || juego->columnaSeleccionada>=juego->columnas){printf("Fuera de rango\n"); continue;}

        char actual=tablero[juego->filaSeleccionada][juego->columnaSeleccionada].estadoVisible;
        int valido=0;
        if(accion=='0' && actual!='!') valido=1;
        else if(accion=='!'||accion=='?'||accion=='x') valido=1;
        if(!valido){ printf("Acción inválida\n"); continue;}

        switch(accion){
            case '0':
                if(juego->primerMovimiento){ juego->primerMovimiento=0; colocarMinas(juego,tablero);}
                if(tablero[juego->filaSeleccionada][juego->columnaSeleccionada].minasAdyacentes=='*') juego->minaExplotada=1;
                revelarCascada(juego,tablero,juego->filaSeleccionada,juego->columnaSeleccionada);
                break;
            case '!':
                juego->banderasColocadas++;
                if(tablero[juego->filaSeleccionada][juego->columnaSeleccionada].minasAdyacentes=='*') juego->minasMarcadasCorrectamente++;
                tablero[juego->filaSeleccionada][juego->columnaSeleccionada].estadoVisible='!';
                break;
            default:
                if(actual=='!'){ juego->banderasColocadas--; 
                    if(tablero[juego->filaSeleccionada][juego->columnaSeleccionada].minasAdyacentes=='*') 
                        juego->minasMarcadasCorrectamente--;}
                tablero[juego->filaSeleccionada][juego->columnaSeleccionada].estadoVisible=accion;
                break;
        }
        break;
    }
}
