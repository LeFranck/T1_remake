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
	r->dead = 'F';
	r->number = number;
	r->largo = 0;
	r->inicio = malloc(sizeof(Posicion));
	r->cabeza = malloc(sizeof(Posicion));
	int z1 = inicio->z;
	int b1 = inicio->b;
	int x1 = inicio->x;
	int y1 = inicio->y;
	r->inicio->z =  z1;
	r->inicio->b =  b1;
	r->inicio->x =  x1;
	r->inicio->y =  y1;
	r->cabeza->z =  z1;
	r->cabeza->b =  b1;
	r->cabeza->x =  x1;
	r->cabeza->y =  y1;

	r->randomnes = 0;
	return r;
}

//Escoge en el arreglo de distancias compatibles la menor y devuelve el index 
//de esa linea en el telar general
void set_goal_linea(Linea* l, int lineas_compatibles_count, int index_en_telar)
{
	int k = 0;
	int j = 0;
	int min = 1000000;
	int index = 0;
	l->lineas_compatibles_count = lineas_compatibles_count;
	for(k = 0; k < lineas_compatibles_count; k++){
		for(j = 0; j < lineas_compatibles_count; j++)
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

void actualizar_linea(Linea* l, Posicion* nueva, char isReady, int building_tomado)
{
	if(posiciones_distintas(nueva, l->cabeza)=='T')
	{
		//Pase por ese bulding
		l->actual->opciones[building_tomado] = 'P';
		l->actual->taken_count++;
		NodoBacktracking* nuevo = create_nodo_backtracking(l->number);
		l->actual = push_nodo_backtracking(l->actual , nuevo);
		l->largo++;
		l->fin_solucion++;

		//eSTABA fuera del if
		l->cabeza = nueva;
	}
	l->isReady = isReady;
	//fprintf(stderr, "Linea %d ha sido actualizada.\n",l->number );
	//fprintf(stderr, "Linea %d estado %c \n",l->number,l->isReady );
}

//Movimiento a partir del building obj
int movimiento_estiloso(Linea* l, int OBJ, int A, int B, int A_, int B_)
{
	int i = 0;
	int a = A;
	int b = B;
	int a_ = A_;
	int b_ = B_;
	int count = l->actual->building_count;
	int obj = OBJ + count;
	while(1 == 1){
		if(i==0)
		{
			if(l->actual->opciones[obj%count]=='T')
			{
				return obj%count;
			}
		}else if(i%2 == 0)
		{
			if(l->actual->opciones[(obj+a)%count]=='T')
			{
				return (obj+a)%count;
			}
			a = a +a_;
			//a = (a +a_)%count ;
		}else{
			if(l->actual->opciones[(obj+b)%count]=='T')
			{
				return (obj+b)%count;
			}
			b = b + b_;
			//b = (b + b_)%count;
		}
		i++;
	}

	//for(i = 0; i < count; i++)
	//{
		//if(i==0)
		//{
			//if(l->actual->opciones[obj%count]=='T')
			//{
				//return obj%count;
			//}
		//}else if(i%2 == 0)
		//{
			//if(l->actual->opciones[(obj+a)%count]=='T')
			//{
				//return (obj+a)%count;
			//}
			//a = a +a_;
			////a = (a +a_)%count ;
		//}else{
			//if(l->actual->opciones[(obj+b)%count]=='T')
			//{
				//return (obj+b)%count;
			//}
			//b = b + b_;
			////b = (b + b_)%count;
		//}
	//}
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

//Encargados de obtener el building objetivo de una linea
//Junto con la dirección de esta(La direccion se usara aca y nada mas)
int get_building_objetivo(Linea* l, Linea* meta, int vecino0_x, int vecino0_y)
{
	int r = -1;
	Posicion* a = l->cabeza;
	Posicion* b = meta->cabeza;
	int dir_x = direccion_x(a,b);
	int dir_y = direccion_y(a,b);
	int zone_sides = l->actual->zone_sides;
	int b_c = l->actual->building_count;
	//bulding count puede ser 8, 4 o 2
	if(b_c == 8)
	{
		r = building8_get_building(dir_x, dir_y);
	}else if(b_c == 4){
		if(zone_sides == 8){
			int dif_x = a->x - vecino0_x;
			int dif_y = a->y - vecino0_y;
			r = building4_get_building_8sides(dir_x, dir_y, dif_x, dif_y, l->randomnes);
		}else{
			r = building4_get_building_4sides(dir_x, dir_y, l->randomnes);
		}
	}else{
		if(l->cabeza->b == 0){
			r = 1;
		}else{
			r = 0;
		}
	}
	return r;
}

int building8_get_building(int direccion_x, int direccion_y)
{
	int r = -1;
	if(direccion_x == 1 && direccion_y == 0){
		r = 0;
	}else if(direccion_x == 1 && direccion_y == -1){
		r = 1;
	}else if(direccion_x == 0 && direccion_y == -1){
		r = 2;
	}else if(direccion_x == -1 && direccion_y == -1){
		r = 3;
	}else if(direccion_x == -1 && direccion_y == 0){
		r = 4;
	}else if(direccion_x == -1 && direccion_y == 1){
		r = 5;
	}else if(direccion_x == 0 && direccion_y == 1){
		r = 6;
	}else if(direccion_x == 1 && direccion_y == 1){
		r = 7;
	}else{
		fprintf(stderr,"\n\n\nWTF b8\n\n\n");
	}
	return r;
}

int building4_get_building_8sides(int direccion_x, int direccion_y, int dif_x, int dif_y, int randomnes)
{
	int r = -1;
	if(dif_x != 0 && dif_y != 0){
		//Caso rombo
		if(direccion_x == 1 && direccion_y == 0){
			if(randomnes == 0){
				r = 0;
			}else{
				r = 3;
			}
		}else if(direccion_x == 1 && direccion_y == -1){
			r = 0;
		}else if(direccion_x == 0 && direccion_y == -1){
			if(randomnes == 0){
				r = 0;
			}else{
				r = 1;
			}
		}else if(direccion_x == -1 && direccion_y == -1){
			r = 1;
		}else if(direccion_x == -1 && direccion_y == 0){
			if(randomnes == 0){
				r = 1;
			}else{
				r = 2;
			}
		}else if(direccion_x == -1 && direccion_y == 1){
			r = 2;
		}else if(direccion_x == 0 && direccion_y == 1){
			if(randomnes == 0){
				r = 2;
			}else{
				r = 3;
			}
		}else if(direccion_x == 1 && direccion_y == 1){
			r = 3;
		}else{
			fprintf(stderr,"\n\n\nWTF b4 rombo\n\n\n");
		}
	}else{
		//Caso cuadrado
		if(direccion_x == 1 && direccion_y == 0){
			r = 0;
		}else if(direccion_x == 1 && direccion_y == -1){
			if(randomnes == 0){
				r = 0;
			}else{
				r = 1;
			}
		}else if(direccion_x == 0 && direccion_y == -1){
			r = 1;
		}else if(direccion_x == -1 && direccion_y == -1){
			if(randomnes == 0){
				r = 1;
			}else{
				r = 2;
			}
		}else if(direccion_x == -1 && direccion_y == 0){
			r = 2;
		}else if(direccion_x == -1 && direccion_y == 1){
			if(randomnes == 0){
				r = 2;
			}else{
				r = 3;
			}
		}else if(direccion_x == 0 && direccion_y == 1){
			r = 3;
		}else if(direccion_x == 1 && direccion_y == 1){
			if(randomnes == 0){
				r = 0;
			}else{
				r = 3;
			}
		}else{
			fprintf(stderr,"\n\n\nWTF b4 cuadrado\n\n\n");
		}
	}
	return r;
}

int building4_get_building_4sides(int direccion_x, int direccion_y, int randomnes)
{
	int r = -1;
	if(direccion_x == 1 && direccion_y == 0){
		if(randomnes == 0){
			r = 0;
		}else{
			r = 3;
		}
	}else if(direccion_x == 1 && direccion_y == -1){
		r = 0;
	}else if(direccion_x == 0 && direccion_y == -1){
		if(randomnes == 0){
			r = 0;
		}else{
			r = 1;
		}
	}else if(direccion_x == -1 && direccion_y == -1){
		r = 1;
	}else if(direccion_x == -1 && direccion_y == 0){
		if(randomnes == 0){
			r = 1;
		}else{
			r = 2;
		}
	}else if(direccion_x == -1 && direccion_y == 1){
		r = 2;
	}else if(direccion_x == 0 && direccion_y == 1){
		if(randomnes == 0){
			r = 2;
		}else{
			r = 3;
		}
	}else if(direccion_x == 1 && direccion_y == 1){
		r = 3;
	}else{
		fprintf(stderr,"\n\n\nWTF b4 sides 4\n\n\n");
	}
	return r;
}

void print_linea(Linea* l)
{
	//Actual debuggind landprotector
	if(l->color == 4){
		fprintf(stderr,"\n");
		fprintf(stderr,"linea number: %d\n",l->number);
		fprintf(stderr,"Inicio\n");
		print_posicion(l->inicio);
		fprintf(stderr,"Cabeza\n");
		print_posicion(l->cabeza);
		fprintf(stderr,"\tIs Ready: %c\n",l->isReady);
		fprintf(stderr,"\tIs Dead: %c\n",l->dead);
		fprintf(stderr,"\tIs DeadEnd: %c\n",l->deadEnd);
		fprintf(stderr,"\tgoal: %d\n",l->goal);
		fprintf(stderr,"\tlargo: %d\n",l->largo);
		fprintf(stderr,"\tcolor: %d",l->color);
		fprintf(stderr,"\talimentada: %c",l->alimentada);
		fprintf(stderr,"\n");
		int i = 0;
		for(i = 0; i < l->lineas_compatibles_count; i++)
		{
			fprintf(stderr, ", %f", l->distancia_otras_lineas[i]);
		}
		fprintf(stderr, "]\n" );
		debug_print_solucion_desde_nodo(l->actual,0);
	}
}

void destrot_linea(Linea* l)
{
	free(l);
}
