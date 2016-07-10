#ifndef T1_LIB_NODO_BACKTRACKING_H
#define T1_LIB_NODO_BACKTRACKING_H
#include <stdint.h>
#include <stdlib.h>


typedef struct NodoBacktracking
{
	char* opciones;
	//Opciones:
	//T es valida, F no es valida, P es pase por ahi
	int validas_count;
	int taken_count;
	//meta nos indica si estamos en la zona de la meta
	char meta;
	char seedA;
	char seedB;
	int distancia;
	int line_number;
	int building_count;
	int zone_sides;

	//conexion hecha 
	size_t z_index;
	uint8_t c1_index;
	uint8_t c2_index;


	struct NodoBacktracking* prev;
} NodoBacktracking;

NodoBacktracking* init_nodo_backtracking();

NodoBacktracking* create_nodo_backtracking(int line_number);

NodoBacktracking* push_nodo_backtracking(NodoBacktracking* prev, NodoBacktracking* nuevo);

//me cabe la duda del retorno
NodoBacktracking* pop_nodo_backtracking(NodoBacktracking* actual);

void debug_print_solucion_desde_nodo(NodoBacktracking* n , int i );

void print_solucion_desde_nodo(NodoBacktracking* actual);

void destroy_nodo_backtracking(NodoBacktracking* n);

#endif
