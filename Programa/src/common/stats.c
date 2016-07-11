#include "stats.h"
#include <stdio.h>

Stats* create_stats(Telar* t)
{
	Stats* r = malloc(sizeof(Stats));
	r->lineas_count = t->lineas_count;
	r->linea_mas_larga = -1;
	r->largo_max = -1;
	//De las con largo mayor a 0 
	r->linea_mas_corta = -1;
	r->largo_min = 100000000;
	r->lineas_muertas_count = 0;
	r->lineas_readys_count = 0;
	r->lineas_waiting_count = 0;
	r->largo_promedio = 0;
	r->edificios_listos = 0;
	r->edificios_faltantes = 0;
	r->zonas_listas = 0;
	r->zonas_faltantes = 0;
	return r;
}

Stats* update_stats(Telar* t, Layout*  l, Stats* r)
{

	int i = 0;
	r->largo_promedio = 0;
	for(i = 0; i < r->lineas_count; i++)
	{
		if(t->estados_de_lineas[i] == 'R')
		{
			r->lineas_readys_count++;
		}else if(t->estados_de_lineas[i] == 'D'){
			r->lineas_muertas_count++;
		}else{
			r->lineas_waiting_count++;
		}

		if(t->lineas[i]->largo > 0)
		{
			r->largo_promedio+=t->lineas[i]->largo;
			if(t->lineas[i]->largo > r->largo_max)
			{
				r->linea_mas_larga = i;
				r->largo_max = t->lineas[i]->largo;
			}
			if(t->lineas[i]->largo < r->largo_min)
			{
				r->linea_mas_corta = i;
				r->largo_min = t->lineas[i]->largo;
			}
		}
	}
	r->largo_promedio = r->largo_promedio/(r->lineas_count / 2);
	return r;
}

void print_stats(Stats* r)
{
	fprintf(stderr, "\n\nLas maravillosas estadisticas son\n\n");
	fprintf(stderr, "La linea mas larga es %d\n" , r->linea_mas_larga);
	fprintf(stderr, "La linea mas larga mide %d\n" , r->largo_max);
	fprintf(stderr, "La linea mas corta es %d\n" , r->linea_mas_corta);
	fprintf(stderr, "El largo minimo es %d\n" , r-> largo_min);
	fprintf(stderr, "El largo promedio de las linea es : %f" , r->largo_promedio);
	fprintf(stderr, "De las %d lineas hay:\n" , r->lineas_count);
	fprintf(stderr, "\tReadys %d\n" , r->lineas_readys_count);
	fprintf(stderr, "\tWaiting %d\n" , r->lineas_waiting_count);
	fprintf(stderr, "\tMuertas%d\n" , r->lineas_muertas_count);
}

void destroy_stats(Stats* s)
{

}