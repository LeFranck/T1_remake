#ifndef T1_LIB_NODO_BACKTRACKING_H
#define T1_LIB_NODO_BACKTRACKING_H


typedef struct NodoBacktracking
{
	char* opciones;
	//Opciones:
	//V es valida, F no es valida, T es tomada, taken
	int validas_count;
	int taken_count;
	//meta nos indica si estamos en la zona de la meta
	char meta;
	char seedA;
	char seedB;
	int distancia;
	int line_number;
	int building_count;
	struct NodoBacktracking* prev;
} NodoBacktracking;

NodoBacktracking* init_nodo_backtracking();

NodoBacktracking* create_nodo_backtracking(int line_number);

void push_nodo_backtracking(NodoBacktracking* prev, NodoBacktracking* nuevo);

//me cabe la duda del retorno
NodoBacktracking* pop_nodo_backtracking(NodoBacktracking* actual);

void destroy_nodo_backtracking(NodoBacktracking* n);

#endif
