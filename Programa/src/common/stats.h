#ifndef T1_LIB_STATS_H
#define T1_LIB_STATS_H

#include <stdlib.h>
#include "telar.h"

struct stats
{

	int linea_mas_larga;
	int largo_max;

	int linea_mas_corta;
	int largo_min;

	int largo_promedio;
	int edificios_listos;
	int edificios_faltantes;
	int zonas_listas;
	int zonas_faltantes;

	char* estados_zonas;

};

typedef struct stats Stats;

Stats* create_stats(Telar* t);

void destroy_stats(Stats* s);

#endif

