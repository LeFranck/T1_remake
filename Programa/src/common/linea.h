#ifndef T1_LIB_LINEA_H
#define T1_LIB_LINEA_H
#include <stdlib.h>
#include "posicion.h"
#include "color.h"
#include "nodo_backtracking.h"

struct linea
{
	//Caracteristicas de la linea
	Color color;
	char isReady;
	char alimentada;
	char deadEnd;
	char dead;
	int number;
	int largo;

	//IA de la Linea
	char seed_mov;
	char seed_back;

	//meta de la linea, goal es el numero de la linea meta
	int goal;
	int direction_x;
	int direction_y;

	//Data asociada con la linea
	int inicio_solucion;
	int fin_solucion;

	Posicion* inicio;
	Posicion* cabeza;

	NodoBacktracking* actual;
};

typedef struct linea Linea;

Linea* create_linea(Color c, int number, char alimentada, Posicion* inicio);

void print_linea(Linea* l);

void destroy_linea(Linea* l);

#endif
