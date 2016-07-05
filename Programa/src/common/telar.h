#ifndef T1_LIB_TELAR_H
#define T1_LIB_TELAR_H
#include "linea.h"
#include "city.h"

struct telar
{
	Linea** lineas;
	Color* colores;
	int* lineas_por_color;
	int* index_en_lineas_por_color;
	char* estados_de_lineas;

	int lineas_count;
	int colores_count;

	int c_m;
};

typedef struct telar Telar;

Telar* init_telar(int l_c, int c_c, int c_m, char* colores, int* cantidad_por_color);

Telar* create_telar(Layout* l);

void ordenar_colores(Telar* t);

void set_lines_goals(Telar* t);

void print_telar(Telar* t);

void print_telar_colores(Telar* t);

void destroy_telar(Telar* t);

#endif
