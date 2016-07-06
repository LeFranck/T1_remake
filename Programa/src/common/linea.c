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

void set_goal_linea(Linea* l, int l_comp_count, int index_en_telar)
{
	int k = 0;
	int j = 0;
	int min = 1000000;
	int index = 0;
	for(k = 0; k < l_comp_count; k++){
		for(j = 0; j < l_comp_count; j++)
		{
			double dist_j = l->distancia_otras_lineas[j];
			if(dist_j < min && dist_j != -1.0)
			{
				index = j + index_en_telar;
				min = dist_j;
			}
		}
		l->goal = index;
		index = 0;
		min = 1000000;
	}	
}

void actualizar_linea(Linea* l, Posicion* nueva, char isReady, double direccion)
{
	//Nodo Actual en linea
	if(l->actual)
	{
		l->actual->opciones[nueva->b] = 'T';
		l->actual->taken_count++;
		NodoBacktracking* nuevo = create_nodo_backtracking(l->number);
		push_nodo_backtracking(l->actual , nuevo);
	}
	//linea
	if(posiciones_distintas(nueva, l->cabeza)=='T')
	{
		l->largo++;
		l->fin_solucion++;
	}
	l->isReady = isReady;
	l->direccion = direccion;
	l->cabeza = nueva;
}

int get_building_objetivo(Linea* l)
{
	return 0;
}


void print_linea(Linea* l)
{
	fprintf(stderr,"\n");
	fprintf(stderr,"linea number: %d\n",l->number);
	print_posicion(l->inicio);
	fprintf(stderr,"\tgoal: %d\n",l->goal);
	//fprintf(stderr,"\tdirection x: %d\n",l->direction_x);
	//fprintf(stderr,"\tdirection y: %d\n",l->direction_y);
	fprintf(stderr,"\tcolor: %d",l->color);
	fprintf(stderr,"\talimentada: %c",l->alimentada);
	fprintf(stderr,"\n");

}

void destrot_linea(Linea* l)
{
	free(l);
}
