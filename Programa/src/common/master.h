#ifndef T1_LIB_MASTER_H
#define T1_LIB_MASTER_H
#include "city.h"
#include "telar.h"
#include "stats.h"
#include "solucion.h"
#include "linea.h"
#include <stdio.h>

struct master
{
	Layout* l;
	Telar* t;
	Solucion* s;
	Stats* stats;
};

typedef struct master Master;

//------------------------------------		LISTOS	 	-----------------------------------------//
//Crea el telar a partir del layout y una solucion que tenga conexiones_maximas tuplas
Master* create_master(Layout* l);

//Ordena el arreglo de lineas en telar dejando todas las lineas de un color juntas
//Este orden tiene a los colores de menor cantidad a mayor cantidad
void juntar_lineas_por_color(Master* m);

void destroy_master(Master* m);

//------------------------------------		LISTOS	 	-----------------------------------------//


//------------------------------------		FALTA	 	-----------------------------------------//

void solve_puzzle(Master* m, char condicion);

void Solve(Master* m);

void tejer(Master* m);

void conectar_linea(Master* m, Linea* a);

void mover_linea(Master* m,Linea* l);

void calcular_opciones_linea(Master* m, Linea* l);

void avanzar_linea(Master* m, Linea* l);

void conectar_meta(Master* m, Linea* l);

void conectar_a_blanco(Master* m, Linea* l);

int chose_white_move(Master* m, Linea* l, int seed);

void conectar_linea_a_edificio(Master* m, Linea* l, int b);

void limpiar_linea(Master* m, Linea* l);

void desconectar_linea(Master* m, Linea* l);

//------------------------------------		FALTA	 	-----------------------------------------//


//------------------------------------		VACIOS	 	-----------------------------------------//

void deal_with_dead_lines0(Master* m);

void deal_with_dead_lines1(Master* m, int larga_number);

void solve_deads_lines(Master* m, int IA);

void IA_dead_lines0(Master* m);

void IA_dead_lines1(Master* m);

void IA_dead_lines2(Master* m);

void solver_alargar_conection(Master* m);

int buscar_candidato_a_desconectar(Master* m);

void desenchufar_linea(Master* m , int linea);

void dummy_conection(Master* m, int linea);

void Optimizar_largos(Master* m);
void AntiMonopolio(Master* m);
void descocer(Master* m);
void retroceder_linea(Master* m, Linea* l);


//------------------------------------		VACIOS	 	-----------------------------------------//

#endif
