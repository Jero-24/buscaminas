#include "azar.h"
#include <stdlib.h>
#include <time.h>

//srand(0) para no cambiar semilla
void inicializar_azar(){ srand((unsigned)time(NULL)); }
int numero_al_azar(int max){ return rand()%max; }
