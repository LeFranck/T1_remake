#include "master.h"
#include "telar.h"
#include "solucion.h"
#include "stats.h"

Master* create_master(Layout* l)
{

	Master* m = malloc(sizeof(Master));
	m->l = l;
	m->t = create_telar(l);
	m->s = create_solucion(m->t->c_m);
	m->stats = create_stats(m->t);
	return m;
}

void solve_puzzle(Master* m, char condicion)
{

	if(condicion == 'O')
	{
		Optimizar_largos(m);
	}else if(condicion == 'M')
	{
		AntiMonopolio(m);
	}else{
		Solve(m);
	}
}

void Optimizar_largos(Master* m){}

void AntiMonopolio(Master* m){}

void Solve(Master* m)
{
	while(m->s->ready != 'T')
	{
		tejer(m);
		if(m->s->ready != 'T')
		{
			descocer(m);
		}
	}
}

void tejer(Master* m)
{
	int i = 0;
	for(i = 0; i < m->t->lineas_count; i++)
	{
		conectar_linea(m, m->t->lineas[i]);
	}
}

void descocer(Master* m)
{

}

void conectar_linea(Master* m, Linea* a)
{

}

void destroy_master(Master* m)
{

}
