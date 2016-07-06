#include "posicion.h"
#include <stdio.h>
#include <math.h>


Posicion* create_posicion(int z, int b, int x, int y)
{
	Posicion* r = malloc(sizeof(Posicion));
	r->z = z;
	r->b = b;
	r->x = x;
	r->y = y;
	return r;
}

double direccion_desde(Posicion* a, Posicion* b)
{
	double dist_x = b->x - a->x;
	double dist_y = b->y - b->x;
	double r = 0.0;
	if(dist_y!=0.0)
	{
		//retorna en grados
		r = atan(dist_x/dist_y)*57.23;
	}
	return r;
}

double distancia_entre(Posicion* a, Posicion* b)
{
	double x = pow((a->x - b->x),2);
	double y = pow((a->y - b->y),2);
	double r = sqrt(x+y);
	return r;
}

int dist_x(Posicion* a, Posicion* b)
{
	int r = b->x - a->x;
	return r;
}

int dist_y(Posicion* a, Posicion* b)
{
	int r = b->y - a->y;
	return r;
}

char posiciones_distintas(Posicion* a, Posicion* b)
{
	if(a->z != b->z || a->b != b->b)
	{
		return 'T';
	}else{
		return 'F';
	}
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

