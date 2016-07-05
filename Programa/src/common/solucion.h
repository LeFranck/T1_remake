#ifndef T1_LIB_SOLUCION_H
#define T1_LIB_SOLUCION_H

#include <stdlib.h>

struct solucion
{
	char** conexiones;
	int conexiones_max;
	int cabeza;
	char ready;
};

typedef struct solucion Solucion;

Solucion* create_solucion(int c_m);

#endif
