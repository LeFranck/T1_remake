#include "nodo_backtracking.h"
#include <stdlib.h>

NodoBacktracking* init_nodo_backtracking()
{
	NodoBacktracking* r = malloc(sizeof(NodoBacktracking));
	return r;
}

NodoBacktracking* create_nodo_backtracking(int line_number)
{
	NodoBacktracking* r = init_nodo_backtracking();
	return r;
}

NodoBacktracking* push_nodo_backtracking(NodoBacktracking* prev, NodoBacktracking* actual)
{
	return prev;
}

NodoBacktracking* pop_nodo_backtracking(NodoBacktracking* actual)
{
	return actual;
}

void destroy_nodo_backtracking(NodoBacktracking* n)
{

}