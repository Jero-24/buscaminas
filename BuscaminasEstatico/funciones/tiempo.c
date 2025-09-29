#include "tiempo.h"
#include <time.h>

static time_t start,end;

void empieza_tiempo(){ time(&start); }
int tiempo_transcurrido(){ time(&end); return (int)difftime(end,start); }
