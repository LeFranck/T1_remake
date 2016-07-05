#include "linea.h"
#include "posicion.h"
#include <stdio.h>

Linea* create_linea(Color c, int number, char alimentada, Posicion* inicio)
{
	Linea* r = malloc(sizeof(Linea));
	r->color = c;
	r->isReady = 'F';
	r->alimentada = alimentada;
	r->deadEnd = 'F';
	r->number = number;
	r->largo = 0;
	r->inicio = inicio;
	r->cabeza = inicio;
	return r;
}

void print_linea(Linea* l)
{
	fprintf(stderr,"\n");
	fprintf(stderr,"linea number: %d\n",l->number);
	print_posicion(l->inicio);
	//fprintf(stderr,"\tgoal: %d\n",l->goal);
	//fprintf(stderr,"\tdirection x: %d\n",l->direction_x);
	//fprintf(stderr,"\tdirection y: %d\n",l->direction_y);
	fprintf(stderr,"\tcolor: %d",l->color);
	fprintf(stderr,"\n");

}

void destrot_linea(Linea* l)
{
	free(l);
}
