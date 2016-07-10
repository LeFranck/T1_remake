#include "nodo_backtracking.h"
#include <stdlib.h>
#include <stdio.h>

NodoBacktracking* init_nodo_backtracking()
{
	NodoBacktracking* r = malloc(sizeof(NodoBacktracking));
	r->opciones = malloc(sizeof(char)*8);
	r->validas_count = 0;
	r->meta = 'F';
	int i = 0;
	for(i = 0; i < 8 ; i++)
	{
		r->opciones[i] = 'F';
	}
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
	if(actual)
	{
		if( actual->z_index == 0 && actual->c1_index == 0 && actual->c2_index == 0)
		{
			if(actual->prev){
				print_solucion_desde_nodo(actual->prev);
			}
		}else{
			if(actual->prev)
			{
				printf("LINK %zu %hhu %hhu\n", actual->z_index, actual->c1_index, actual->c2_index);
				print_solucion_desde_nodo(actual->prev);
			}else{
				printf("LINK %zu %hhu %hhu\n", actual->z_index, actual->c1_index, actual->c2_index);
			}
		}
	}
}

void destroy_nodo_backtracking(NodoBacktracking* n)
{

}