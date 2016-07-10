#include "posicion.h"
#include <stdio.h>
#include <math.h>
#define PI 3.14159265


Posicion* create_posicion(int z, int b, int x, int y)
{
	Posicion* r = malloc(sizeof(Posicion));
	r->z = z;
	r->b = b;
	r->x = x;
	r->y = y;
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

int direccion_x(Posicion* a, Posicion* b)
{
	int difx = dist_x(a,b);
	if( difx < 0 )
	{
		return -1;
	}else if( difx > 0)
	{
		return  1;
	}else{
		return 0;
	}
}

int direccion_y(Posicion* a, Posicion* b)
{
	int dify = dist_y(a,b);
	if( dify < 0 )
	{
		return -1;
	}else if( dify > 0)
	{
		return  1;
	}else{
		return 0;
	}
}

char posiciones_distintas(Posicion* posa, Posicion* posb)
{
	if(posa->z != posb->z || posa->b != posb->b)
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




//METODOS VIEJOS ------------------------------
double direccion_desde(Posicion* a, Posicion* b)
{
	double dist_x = b->x - a->x;
	double dist_y = b->y - a->y;
	double r = 0.0;
	double grados = 180.0/ PI;
	if(dist_y!=0.0)
	{
		//retorna en grados
		r = atan(dist_y/dist_x)*grados;
	}
	return r;
}
