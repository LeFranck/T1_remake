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

double distancia_entre(Posicion* a, Posicion* b);

//Van de a a b

int dist_x(Posicion* a, Posicion* b);

int dist_y(Posicion* a, Posicion* b);

int direccion_x(Posicion* a, Posicion* b);

int direccion_y(Posicion* a, Posicion* b);

char posiciones_distintas(Posicion* a, Posicion* b);

void print_posicion(Posicion* p);

void destroy_posicion(Posicion* p);

//Metodos viejos
double direccion_desde(Posicion* a, Posicion* b);


#endif
