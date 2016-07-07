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

//Escoge en el arreglo de distancias compatibles la menor y devuelve el index 
//de esa linea en el telar general
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

void actualizar_linea(Linea* l, Posicion* nueva, char isReady, double direccion, int building_tomado)
{
	//Nodo Actual en linea
	//PROBLEMAS
	//if(l->actual)
	//{
		//l->actual->opciones[building_tomado] = 'T';
		//l->actual->taken_count++;
		//NodoBacktracking* nuevo = create_nodo_backtracking(l->number);
		//push_nodo_backtracking(l->actual , nuevo);
		//l->actual = nuevo;
	//}
	if(posiciones_distintas(nueva, l->cabeza)=='T')
	{
		//Cambio
		l->actual->opciones[building_tomado] = 'T';
		l->actual->taken_count++;
		NodoBacktracking* nuevo = create_nodo_backtracking(l->number);
		push_nodo_backtracking(l->actual , nuevo);
		l->largo++;
		l->fin_solucion++;
	}
	l->isReady = isReady;
	l->direccion = direccion;
	l->cabeza = nueva;
}

int get_building_objetivo(Linea* l, int dif_x)
{
	int r = 0;
	switch(l->actual->building_count) 
	{
		case 8 :
			r = building8_get_building(l->direccion, dif_x);
			break;
		case 6 :
			r = building6_get_building(l->direccion, dif_x);
			break;
		case 4 :
			r = building4_get_building(l->direccion, dif_x);
			break;
		case 2 :
			if(l->cabeza->b == 0){
				r = 1;
			}else{
				r = 0;
			}
			break;
		default :
		printf("WTF\n" );
	}
	return r;
}

int movimiento_estiloso(Linea* l, int OBJ, int A, int B, int A_, int B_)
{
	int i = 0;
	int a = A;
	int b = B;
	int a_ = A_;
	int b_ = B_;
	int count = l->actual->building_count;
	int obj = OBJ + count;
	for(i = 0; i < count; i++)
	{
		if(i==0)
		{
			if(l->actual->opciones[obj%count]=='V')
			{
				return obj%count;
			}
		}else if(i%2 == 0)
		{
			if(l->actual->opciones[(obj+a)%count]=='V')
			{
				return (obj+a)%count;
			}
			a = (a +a_)%count ;
		}else{
			fprintf(stderr,"(%d+%d)mod%d = %d\n",obj,b,count,(obj+b)%count);
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

//BUG DEL ANGULO si x1 > x2 da el angulo de la derecha
int building8_get_building(double direccion, int dif_x)
{
	int r = 0;
	if(direccion >= -112.5 && direccion <= -67.5)
	{
		r = 2;
	}else if(direccion >= -67.5 && direccion <= -22.5)
	{
		if(dif_x < 0)
		{
			r = 5;
		}else{
			r = 1;			
		}
	}else if(direccion >= -22.5 && direccion <= 22.5)
	{
		if(dif_x < 0)
		{
			r = 4;
		}else{
			r = 0;
		}
	}else if(direccion >= 22.5 && direccion <= 67.5)
	{
		if(dif_x < 0)
		{
			r = 3;
		}else{
			r = 7;
		}
	}else if(direccion >= 67.5 && direccion <= 112.5)
	{
		r = 6;
	}else{
		printf("WTF\n" );
	}

	fprintf(stderr,"direccion %f edificio %d\n",direccion,r);
	return r;
}

//AL parecer los de 6 son un mito
int building6_get_building(double direccion, int dif_x)
{
	return building8_get_building(direccion, dif_x);
}

int building4_get_building(double direccion, int dif_x)
{
	int r = 0;
	if(direccion >= -90.0 && direccion <= 0.0)
	{
		if(dif_x<0)
		{
			r = 2;
		}else{
			r = 0;			
		}
	}else if(direccion >= 0.0 && direccion <= 90.0)
	{
		if(dif_x <0)
		{
			r = 1;
		}else{
			r = 3;			
		}
	}else{
		printf("WTF\n" );
	}
	return r;
}

int building2_get_building(double direccion)
{
		return building4_get_building(direccion,0);
}

void print_linea(Linea* l)
{
	fprintf(stderr,"\n");
	fprintf(stderr,"linea number: %d\n",l->number);
	print_posicion(l->inicio);
	fprintf(stderr,"\tgoal: %d\n",l->goal);
	fprintf(stderr,"\tcolor: %d",l->color);
	fprintf(stderr,"\talimentada: %c",l->alimentada);
	fprintf(stderr,"\n");

}

void destrot_linea(Linea* l)
{
	free(l);
}
