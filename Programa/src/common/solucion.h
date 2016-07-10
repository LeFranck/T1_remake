#ifndef T1_LIB_SOLUCION_H
#define T1_LIB_SOLUCION_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

struct solucion
{
	//char** conexiones;
	size_t* z_index;
	uint8_t* c1_index;
	uint8_t* c2_index;
	int conexiones_max;
	int cabeza;
	char ready;
};

typedef struct solucion Solucion;

Solucion* create_solucion(int c_m);

void agregar_conexion_a_solucion(Solucion* s, size_t i0, uint8_t i1, uint8_t i2);

void print_solucion_output(Solucion* s);

void print_solucion_output_parcial(Solucion* s);

void destroy_solucion(Solucion* s);

#endif
