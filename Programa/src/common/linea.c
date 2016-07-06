#include "linea.h"
#include "posicion.h"
#include <stdio.h>

Linea* create_linea(Color c, int number, char alimentada, Posicion* inicio)
{
	Linea* r = malloc(sizeof(Linea));
	r->color = c;
	r->isReady = 'F';
	r->alimentada = alimentada;
	r->deadEnd = 'F';
	r->number = number;
	r->largo = 0;
	r->inicio = inicio;
	r->cabeza = inicio;
	return r;
}

void set_goal_linea(Linea* l, int l_comp_count, int index_en_telar)
{
	int k = 0;
	int j = 0;
	int min = 1000000;
	int index = 0;
	for(k = 0; k < l_comp_count; k++){
		for(j = 0; j < l_comp_count; j++)
		{
			double dist_j = l->distancia_otras_lineas[j];
			if(dist_j < min && dist_j != -1.0)
			{
				index = j + index_en_telar;
				min = dist_j;
			}
		}
		l->goal = index;
		index = 0;
		min = 1000000;
	}	
}

void actualizar_linea(Linea* l, Posicion* nueva, char isReady, double direccion)
{
	//Nodo Actual en linea
	if(l->actual)
	{
		l->actual->opciones[nueva->b] = 'T';
		l->actual->taken_count++;
		NodoBacktracking* nuevo = create_nodo_backtracking(l->number);
		push_nodo_backtracking(l->actual , nuevo);
	}
	//linea
	if(posiciones_distintas(nueva, l->cabeza)=='T')
	{
		l->largo++;
		l->fin_solucion++;
	}
	l->isReady = isReady;
	l->direccion = direccion;
	l->cabeza = nueva;
}

int get_building_objetivo(Linea* l)
{
	int r = 0;
	switch(l->actual->building_count) 
	{
		case 8 :
			r = building8_get_building(l->direccion);
			break;
		case 6 :
			r = building6_get_building(l->direccion);
			break;
		case 4 :
			r = building4_get_building(l->direccion);
			break;
		case 2 :
			r = building2_get_building(l->direccion);
			break;
		default :
		printf("WTF\n" );
	}
	return r;
}

int movimiento_estiloso(Linea* l, int obj, int A, int B, int A_, int B_)
{
	int i = 0;
	int a = A;
	int b = B;
	int a_ = A_;
	int b_ = B_;
	int count = l->actual->building_count;
	for(i = 0; i < count; i++)
	{
		if(i==0)
		{
			if(l->actual->opciones[obj]=='V')
			{
				return obj;
			}
		}else if(i%2 == 0)
		{
			if(l->actual->opciones[(obj+a)%count]=='V')
			{
				return (obj+a)%count;
			}
			a = (a +a_)%count ;
		}else{
			if(l->actual->opciones[(obj+b)%count]=='V')
			{
				return (obj+b)%count;
			}
			b = (b + b_)%count;
		}
	}
	return -1;
}

int movimiento_estilo_a(Linea* l, int obj)
{
	return movimiento_estiloso(l,obj,1,-1,1,-1);
}

int movimiento_estilo_b(Linea* l, int obj)
{
	return movimiento_estiloso(l,obj,-1,1,-1,1);
}

int movimiento_estilo_c(Linea* l, int obj)
{
	return movimiento_estiloso(l,obj,4,-4,-1,+1);
}

int movimiento_estilo_d(Linea* l, int obj)
{
	return movimiento_estiloso(l,obj,1,-1,2,-2);
}

int building8_get_building(double direccion)
{
	int r = 0;
	if(direccion >= -202.5 && direccion <= -157.5)
	{
		r = 4;
	}else if(direccion >= -157.5 && direccion <= -112.5)
	{
		r = 3;
	}else if(direccion >= -112.5 && direccion <= -67.5)
	{
		r = 2;
	}else if(direccion >= -67.5 && direccion <= -22.5)
	{
		r = 1;
	}else if(direccion >= -22.5 && direccion <= 22.5)
	{
		r = 0;
	}else if(direccion >= 22.5 && direccion <= 67.5)
	{
		r = 7;
	}else if(direccion >= 67.5 && direccion <= 112.5)
	{
		r = 6;
	}else if(direccion >= 112.5 && direccion <= 157.5)
	{
		r = 5;
	}else if(direccion >= 157.5 && direccion <= 202.5)
	{
		r = 4;
	}else{
		printf("WTF\n" );
	}
	return r;
}

int building6_get_building(double direccion)
{
	return building8_get_building(direccion);
}

int building4_get_building(double direccion)
{
	int r = 0;
	if(direccion >= -180.0 && direccion <= -90.0)
	{
		r = 1;
	}else if(direccion >= -90.0 && direccion <= 0.0)
	{
		r = 0;
	}else if(direccion >= 0.0 && direccion <= 90.0)
	{
		r = 4;
	}else if(direccion >= 90 && direccion <= 180)
	{
		r = 2;
	}else{
		printf("WTF\n" );
	}
	return r;
}

int building2_get_building(double direccion)
{
	return building4_get_building(direccion);
}

void print_linea(Linea* l)
{
	fprintf(stderr,"\n");
	fprintf(stderr,"linea number: %d\n",l->number);
	print_posicion(l->inicio);
	fprintf(stderr,"\tgoal: %d\n",l->goal);
	//fprintf(stderr,"\tdirection x: %d\n",l->direction_x);
	//fprintf(stderr,"\tdirection y: %d\n",l->direction_y);
	fprintf(stderr,"\tcolor: %d",l->color);
	fprintf(stderr,"\talimentada: %c",l->alimentada);
	fprintf(stderr,"\n");

}

void destrot_linea(Linea* l)
{
	free(l);
}
