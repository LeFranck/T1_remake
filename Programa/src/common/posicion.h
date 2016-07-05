#ifndef T1_LIB_POSICION_H
#define T1_LIB_POSICION_H

#include <stdlib.h>
struct posicion
{
	//zona,building,x,y
	int z;
	int b;
	int x;
	int y;
};

typedef struct posicion Posicion;

Posicion* create_posicion(int z, int b, int x, int y);

int direccion_desde(Posicion* a, Posicion* b);

int distancia_entre(Posicion* a, Posicion* b);

int dist_x(Posicion* a, Posicion* b);

int dist_y(Posicion* a, Posicion* b);

void print_posicion(Posicion* p);

void destroy_posicion(Posicion* p);

#endif
