#include "solucion.h"

Solucion* create_solucion(int c_m)
{
	Solucion* r = malloc(sizeof(Solucion));
	//r->conexiones = malloc(sizeof(char*)*c_m);
	r->z_index = malloc(sizeof(size_t)*c_m);
	r->c1_index = malloc(sizeof(uint8_t)*c_m);
	r->c2_index = malloc(sizeof(uint8_t)*c_m);
	r->conexiones_max = c_m;
	r->cabeza = 0;
	r->ready = 'F';
	return r;
}

void agregar_conexion_a_solucion(Solucion* s, size_t i0, uint8_t i1, uint8_t i2)
{
	s->z_index[s->cabeza] = i0;
	s->c1_index[s->cabeza] = i1;
	s->c2_index[s->cabeza] = i2;
	s->cabeza++;
	if(s->cabeza == s->conexiones_max)	
	{
		s->ready= 'T';
	}
	//fprintf(stderr,"S->Cabeza: %d \t S->C_M: %d \n ",s->cabeza, s->conexiones_max);
	//fprintf(stderr,"LINK %zu %hhu %hhu\n", i0, i1, i2);
	//printf("LINK %zu %hhu %hhu\n", i0, i1, i2);
}

void print_solucion_output(Solucion* s)
{
	int i = 0;
	for(i = 0; i < s->conexiones_max; i++)
	{
    	printf("LINK %zu %hhu %hhu\n", s->z_index[i], s->c1_index[i], s->c2_index[i]);
    }
}

void print_solucion_output_parcial(Solucion* s)
{
	int i = 0;
	fprintf(stderr,"Tenemos guardadas %d soluciones\n", s->cabeza);
	for(i = 0; i < s->cabeza; i++)
	{
    	printf("LINK %zu %hhu %hhu\n", s->z_index[i], s->c1_index[i], s->c2_index[i]);
    }
}

void destroy_solucion(Solucion* s)
{
	//free(s->conexiones);
	free(s);
}
