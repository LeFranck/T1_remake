#ifndef T1_LIB_MASTER_H
#define T1_LIB_MASTER_H
#include "city.h"
#include "telar.h"
#include "stats.h"
#include "solucion.h"
#include "linea.h"
#include <stdio.h>

struct master
{
	Layout* l;
	Telar* t;
	Solucion* s;
	Stats* stats;
};

typedef struct master Master;

Master* create_master(Layout* l);

void solve_puzzle(Master* m, char condicion);

void Optimizar_largos(Master* m);

void AntiMonopolio(Master* m);

void Solve(Master* m);

void tejer(Master* m);

void descocer(Master* m);

void conectar_linea(Master* m, Linea* a);

void mover_linea(Master* m,Linea* l);

void calcular_opciones_linea(Master* m, Linea* l);

void retroceder_linea(Master* m, Linea* l);

void avanzar_linea(Master* m, Linea* l);

void conectar_meta(Master* m, Linea* l);

void conectar_a_blanco(Master* m, Linea* l);

void make_white_move(Master* m, Linea* l, int seed);

void conectar_linea_a_edificio(Master* m, Linea* l, int b);

void juntar_lineas_por_color(Master* m);

void destroy_master(Master* m);

#endif
