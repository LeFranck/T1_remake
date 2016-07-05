#ifndef T1_LIB_NODO_BACKTRACKING_H
#define T1_LIB_NODO_BACKTRACKING_H


typedef struct NodoBacktracking
{
	char* opciones;
	int validas_count;
	char seedA;
	char seedB;
	int distancia;
	int line_number;
	struct NodoBacktracking* prev;
} NodoBacktracking;

NodoBacktracking* init_nodo_backtracking();

NodoBacktracking* create_nodo_backtracking(int line_number);

NodoBacktracking* push_nodo_backtracking(NodoBacktracking* prev, NodoBacktracking* actual);

NodoBacktracking* pop_nodo_backtracking(NodoBacktracking* actual);

void destroy_nodo_backtracking(NodoBacktracking* n);

#endif