#ifndef T1_LIB_LINEA_H
#define T1_LIB_LINEA_H
#include <stdlib.h>
#include "posicion.h"
#include "color.h"
#include "nodo_backtracking.h"

struct linea
{
	//Caracteristicas de la linea
	Color color;
	char isReady;
	char alimentada;
	char deadEnd;
	char dead;
	int number;
	int largo;

	//Sirve para casos especiales
	int randomnes;

	//IA de la Linea
	char seed_mov;
	char seed_back;

	//meta de la linea, goal es el numero de la linea meta
	int goal;
	//double direccion;
	int direccion_x;
	int direccion_y;

	//Se usan para calcular la meta
	double* distancia_otras_lineas;
	int* lineas_compatibles;
	int* posibles_metas;
	int lineas_compatibles_count;

	//Data asociada con la linea
	int inicio_solucion;
	int fin_solucion;

	Posicion* inicio;
	Posicion* cabeza;

	NodoBacktracking* actual;
	int* zonas_visitadas;
};

typedef struct linea Linea;

//------------------------------------		LISTOS	 	-----------------------------------------//

Linea* create_linea(Color c, int number, char alimentada, Posicion* inicio);

void set_goal_linea(Linea* l, int l_comp_count, int index_en_telar);

void actualizar_linea(Linea* l, Posicion* nueva, char isReady, int building_tomado);

int get_building_objetivo(Linea* l, Linea* meta, int vecino0_x, int vecino0_y);

int movimiento_estiloso(Linea* l, int obj, int a, int b, int a_, int b_);

int movimiento_estilo_a(Linea* l, int obj);

int movimiento_estilo_b(Linea* l, int obj);

int movimiento_estilo_c(Linea* l, int obj);

int movimiento_estilo_d(Linea* l, int obj);

//NUEVOS
int building8_get_building(int direccion_x, int direccion_y);

int building4_get_building_8sides(int direccion_x, int direccion_y, int dif_x, int dif_y, int randomnes);

int building4_get_building_4sides(int direccion_x, int direccion_y, int randomnes);

void print_linea(Linea* l);

void destroy_linea(Linea* l);

//------------------------------------		FALTA	 	-----------------------------------------//


//------------------------------------		VACIOS	 	-----------------------------------------//




#endif
