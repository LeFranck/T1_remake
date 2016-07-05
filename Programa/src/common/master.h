#ifndef T1_LIB_MASTER_H
#define T1_LIB_MASTER_H
#include "city.h"
#include "telar.h"
#include "stats.h"
#include "solucion.h"


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

void destroy_master(Master* m);

#endif
