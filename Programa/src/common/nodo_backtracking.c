#include "nodo_backtracking.h"
#include <stdlib.h>

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

void push_nodo_backtracking(NodoBacktracking* actual, NodoBacktracking* nuevo)
{
	nuevo->prev = actual;
}

//dudoso
NodoBacktracking* pop_nodo_backtracking(NodoBacktracking* actual)
{
	NodoBacktracking* n = actual->prev;
	free(actual->opciones);
	free(actual);
	return n;
}

void destroy_nodo_backtracking(NodoBacktracking* n)
{

}