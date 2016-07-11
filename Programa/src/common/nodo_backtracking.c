#include "nodo_backtracking.h"
#include <stdlib.h>
#include <stdio.h>

NodoBacktracking* init_nodo_backtracking()
{
	NodoBacktracking* r = malloc(sizeof(NodoBacktracking));
	r->opciones = malloc(sizeof(char)*8);
	r->validas_count = 0;
	r->meta = 'F';
	r->z_index = 0;
	r->c1_index = 0;
	r->c2_index = 0;
	int i = 0;
	for(i = 0; i < 8 ; i++)
	{
		r->opciones[i] = 'F';
	}
	r->has_prev = 0;
	return r;
}

NodoBacktracking* create_nodo_backtracking(int line_number)
{
	NodoBacktracking* r = init_nodo_backtracking();
	r->line_number = line_number;
	return r;
}

NodoBacktracking* push_nodo_backtracking(NodoBacktracking* actual, NodoBacktracking* nuevo)
{
	nuevo->prev = actual;
	nuevo->has_prev = 1;
	return nuevo;
}

//dudoso
NodoBacktracking* pop_nodo_backtracking(NodoBacktracking* actual)
{
	NodoBacktracking* n = malloc(sizeof(NodoBacktracking));
	n = actual->prev;
	free(actual->opciones);
	free(actual);
	return n;
}

void print_solucion_desde_nodo(NodoBacktracking* actual)
{
	if( actual->z_index == 0 && actual->c1_index == 0 && actual->c2_index == 0)
	{
		if(actual->has_prev == 1){
			print_solucion_desde_nodo(actual->prev);
		}
	}else{
		if(actual->has_prev)
		{
			printf("LINK %zu %hhu %hhu\n", actual->z_index, actual->c1_index, actual->c2_index);
			print_solucion_desde_nodo(actual->prev);
		}else{
			printf("LINK %zu %hhu %hhu\n", actual->z_index, actual->c1_index, actual->c2_index);
		}
	}
}

void debug_print_solucion_desde_nodo(NodoBacktracking* actual, int i )
{
	//fprintf(stderr, "impresion %d\n", i);
	//fprintf(stderr,"LINK %zu %hhu %hhu\n", actual->z_index, actual->c1_index, actual->c2_index);
	if(actual->has_prev == 1){
		debug_print_solucion_desde_nodo(actual->prev,i+1);
	}
}


void destroy_nodo_backtracking(NodoBacktracking* n)
{

}