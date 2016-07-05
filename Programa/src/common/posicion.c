#include "posicion.h"
#include <stdio.h>

Posicion* create_posicion(int z, int b, int x, int y)
{
	Posicion* r = malloc(sizeof(Posicion));
	r->z = z;
	r->b = b;
	r->x = x;
	r->y = y;
	return r;
}

int direccion_desde(Posicion* a, Posicion* b)
{
	return 0;
}

int distancia_entre(Posicion* a, Posicion* b)
{
	return 0;
}

int dist_x(Posicion* a, Posicion* b)
{
	return 0;
}

int dist_y(Posicion* a, Posicion* b)
{
	return 0;
}

void print_posicion(Posicion* p)
{
	fprintf(stderr,"\tz: %d\n",p->z);
	fprintf(stderr,"\tb: %d\n",p->b);
	fprintf(stderr,"\tx: %d\n",p->x);
	fprintf(stderr,"\ty: %d\n",p->y);
}

void destroy_posicion(Posicion* p)
{
	free(p);
}

