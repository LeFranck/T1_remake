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

//------------------------------------		LISTOS	 	-----------------------------------------//

Telar* init_telar(int l_c, int c_c, int c_m, char* colores, int* cantidad_por_color);

Telar* clon_telar(Telar* t);

Telar* create_telar(Layout* l);

void set_dintacias_otras_lineas_desde_linea(Telar* t, int color, int linea);

//Ordenar la lista de colores que tiene telar segun cantidad de lineas de cada color
void ordenar_colores(Telar* t);

void set_lines_goals(Telar* t);

void set_lines_goals_color(Telar* t, int i);

void set_lines_distancia_otras_lineas(Telar* t, int i);

void print_telar(Telar* t);

void print_telar_colores(Telar* t);

void destroy_telar(Telar* t);

//------------------------------------		FALTA	 	-----------------------------------------//


//------------------------------------		VACIOS	 	-----------------------------------------//

void reset_goal_linea(Telar* t, Linea* l);

void print_solucion_final_desde_telar(Telar* t);






#endif
