#include "solucion.h"
Solucion* create_solucion(int c_m)
{
	Solucion* r = malloc(sizeof(Solucion));
	r->conexiones = malloc(sizeof(char*)*c_m);
	r->conexiones_max = c_m;
	r->cabeza = 0;
	r->ready = 'F';
	return r;
}

void destroy_solucion(Solucion* s)
{
	free(s->conexiones);
	free(s);
}
