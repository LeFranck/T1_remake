#ifndef T1_LIB_STATS_H
#define T1_LIB_STATS_H

#include <stdlib.h>
#include "telar.h"

struct stats
{

	int linea_mas_larga;
	int largo_max;

	//De las con largo mayor a 0 
	int linea_mas_corta;
	int largo_min;

	char* estados_de_linea;
	int lineas_muertas_count;
	int lineas_readys_count;
	int lineas_waiting_count;
	int lineas_count;
	int conexiones_max;

	double largo_promedio;
	int edificios_listos;
	int edificios_faltantes;
	int zonas_listas;
	int zonas_faltantes;

	char* estados_zonas;

};

typedef struct stats Stats;

Stats* create_stats(Telar* t);

Stats* update_stats(Telar* t, Layout* , Stats* r);

void print_stats(Stats* s);

void destroy_stats(Stats* s);

#endif

