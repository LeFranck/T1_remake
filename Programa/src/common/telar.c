#include "telar.h"
#include "linea.h"
#include "color.h"
#include <stdio.h>


Telar* init_telar(int l_c, int c_c, int c_m, char* colores, int* cantidad_por_color)
{
	Telar* r = malloc(sizeof(Telar));
	r->lineas = malloc(sizeof(Linea*)*l_c);
	r->colores = malloc(sizeof(Color)*c_c);
	r->lineas_por_color = malloc(sizeof(int)*c_c);
	r->index_en_lineas_por_color = malloc(sizeof(int)*c_c);
	r->estados_de_lineas = malloc(sizeof(char)*l_c);
	r->lineas_count = l_c;
	r->colores_count = c_c;
	r->c_m = c_m;
	int i = 0;
	int j = 0;
	for(i = 0; i < l_c; i++)
	{
		r->estados_de_lineas[i] = 'W';
	}
	for(i = 0; i < 7; i++)
	{
		if(colores[i]=='T')
		{
			r->colores[j]= i + 1;
			r->lineas_por_color[j] = cantidad_por_color[i];
			j++;
		}
	}

	return r;
}

Telar* clon_telar(Telar* t)
{
	Telar* r = malloc(sizeof(Telar));
	r->lineas = malloc(sizeof(Linea*)*t->lineas_count);
	r->colores = t->colores;
	r->lineas_por_color = t->lineas_por_color;
	r->index_en_lineas_por_color = t->index_en_lineas_por_color;
	r->estados_de_lineas = t->estados_de_lineas;
	r->lineas_count = t->lineas_count;
	r->colores_count = t->colores_count;
	r->c_m = t->c_m;
	return r;
}

Telar* create_telar(Layout* l)
{
	int bc = 0;
	int c_m = 0;
	int l_c = 0;
	int c_c = 0;
	char* colores = malloc(sizeof(char)*7);
	int* cantidad_por_color = malloc(sizeof(int)*7);
	int k = 0;
	int lineas_flotantes_count = 0;

	int i = 0;	
	int j = 0;
	for(i = 0; i < 7 ;i++)
	{
		colores[i] = 'F';
		cantidad_por_color[i] = 0;
	}
	//cuenta los edificios totales
	//cuenta la cantidad de lineas flotantes que habran
	for(i = 0; i < l->zone_count; i++)
	{
		bc = bc + l->zones[i]->building_count;
		for(j = 0; j < l->zones[i]->building_count; j++)
		{	
			if(l->zones[i]->buildings[j]->color!=0 && !city_client_is_taken(l->zones[i]->buildings[j]) && !l->zones[i]->buildings[j]->linked[0]->zone->core )
			{
				lineas_flotantes_count++;
				cantidad_por_color[l->zones[i]->buildings[j]->color-1]++;
			}
		}
	}

	c_m = bc/2;

	//Cuenta las lineas a partir de los cores
	//Agrega a los contadores las cantidades de lineas por color
	for(i = 0; i < l->core_count; i++)
	{
		l_c = l_c + l->cores[i]->buildings[0]->link_count;
		cantidad_por_color[l->cores[i]->buildings[0]->color-1]+=l->cores[i]->buildings[0]->link_count;
		if(colores[l->cores[i]->buildings[0]->color-1]!='T')
		{
			colores[l->cores[i]->buildings[0]->color-1]='T';
		}
	}

	l_c = l_c + lineas_flotantes_count;

	for(i = 0; i < 7; i++)
	{
		if(colores[i]=='T'){c_c++;}
	}
	
	//Setea las variables de telar a partir de los parametros
	Telar* r = init_telar(l_c,c_c,c_m, colores, cantidad_por_color);

	//Crea las lineas conectadas
	for(i = 0; i < l->core_count; i++)
	{
		for(j = 0; j < l->cores[i]->buildings[0]->link_count; j++)
		{
			// z, b, x e y 
			Posicion* p = create_posicion(l->cores[i]->buildings[0]->linked[j]->zone->index
											,l->cores[i]->buildings[0]->linked[j]->index,
											l->cores[i]->buildings[0]->linked[j]->zone->x,
											l->cores[i]->buildings[0]->linked[j]->zone->y);
			r->lineas[k] = create_linea(l->cores[i]->buildings[0]->linked[j]->color,k, 'T', p);
			k++;
		}
	}
	
	//Crea las lineas flotantes
	if(lineas_flotantes_count > 0)
	{
		for(i = 0; i < l->zone_count; i++)
		{
			for(j = 0; j < l->zones[i]->building_count; j++)
			{
				if(l->zones[i]->buildings[j]->color!=0 && !city_client_is_taken(l->zones[i]->buildings[j]) && !l->zones[i]->buildings[j]->linked[0]->zone->core)
				{
					// z, b, x e y 
					Posicion* p = create_posicion( i, j ,l->zones[i]->x,l->zones[i]->y);
					r->lineas[k] = create_linea(l->zones[i]->buildings[j]->color,k, 'F', p);
					k++;
				}
			}
		}
	}

	for(i = 0; i < r->lineas_count; i++)
	{
		r->lineas[i]->zonas_visitadas = malloc(sizeof(int)*l->zone_count);
		for(j = 0; j < l->zone_count; j++)
		{
			r->lineas[i]->zonas_visitadas[j] = 0;
		}
	}
	return r;
}

void ordenar_colores(Telar* t)
{
	//SORT
	int i = 0;
	int k = 0;
	int index_min = 0;
	int min = 10000000;

	for (k= 0; k < t->colores_count; k++)
	{
		for(i = k; i < t->colores_count; i++)
		{
			if(t->lineas_por_color[i]<min)
			{
				min = t->lineas_por_color[i];
				index_min = i;
			}
		}
		int aux_l = t->lineas_por_color[k];
		int aux_c = t->colores[k];
		t->colores[k] = t->colores[index_min];
		t->lineas_por_color[k] = t->lineas_por_color[index_min];
		t->lineas_por_color[index_min] = aux_l;
		t->colores[index_min] = aux_c;
		min = 1000000;
	}

}
//Setea las metas de todas las lineas al inicio del programa
void set_lines_goals(Telar* t)
{
	int i = 0;
	for(i = 0; i < t->colores_count; i++)
	{
		set_lines_goals_color(t, i);
	}
}

//Setea las metas de todas las lineas del color i
void set_lines_goals_color(Telar* t, int i)
{
	set_lines_distancia_otras_lineas(t, i);
	int k = 0;
	for(k = 0; k < t->lineas_por_color[i]; k++)
	{
		set_goal_linea(t->lineas[t->index_en_lineas_por_color[i]+k], t->lineas_por_color[i], t->index_en_lineas_por_color[i]);
	}	
}

// i es el numero en el index colores(estamos seteando la distancia de ese color)
void set_lines_distancia_otras_lineas(Telar* t, int i)
{
	int j = 0;
	int k = 0;
	for(j = 0; j < t->lineas_por_color[i]; j++)
	{
		t->lineas[t->index_en_lineas_por_color[i]+j]->distancia_otras_lineas = malloc(sizeof(double)*t->lineas_por_color[i]);
		for(k = 0; k < t->lineas_por_color[i]; k++)
		{
			//El segundo argumento del || es para ver si la linea esta ok
			if(k==j || t->estados_de_lineas[t->index_en_lineas_por_color[i]+k] != 'W')
			{
				t->lineas[t->index_en_lineas_por_color[i]+j]->distancia_otras_lineas[k] = -1.0;
			}else{
				double dist = distancia_entre(t->lineas[t->index_en_lineas_por_color[i]+j]->cabeza, t->lineas[t->index_en_lineas_por_color[i]+k]->cabeza);
				t->lineas[t->index_en_lineas_por_color[i]+j]->distancia_otras_lineas[k] = dist;
			}
		}
	}
}

void set_dintacias_otras_lineas_desde_linea(Telar* t, int color, int linea)
{
	int index_en_arreglo_de_colores = 0;
	int j = 0;
	int i = 0;
	for(j = 0; j < t->colores_count; j++)
	{
		if(t->colores[j] == color)
		{
			index_en_arreglo_de_colores = j;
		}
	}

	int index_agregado = t->index_en_lineas_por_color[index_en_arreglo_de_colores];

	for(i = 0; i < t->lineas_por_color[index_en_arreglo_de_colores]; i++)
	{
		if(index_agregado + i == linea || t->estados_de_lineas[index_agregado + i] != 'W')
		{
			t->lineas[linea]->distancia_otras_lineas[i] = -1;
		}else{
			double dist = distancia_entre(t->lineas[index_agregado + i]->cabeza, t->lineas[linea]->cabeza);
			t->lineas[linea]->distancia_otras_lineas[i] = dist;
		}
	}
}

void reset_goal_linea(Telar* t, Linea* l)
{
	int i = 0;
	int j = 0;
	int min = 1000000;
	int index = 0;
	for(i = 0; i < t->colores_count; i++){
		if(t->colores[i] == l->color)
		{
			for(j = 0; j < t->lineas_por_color[i]; j++)
			{
				double dist_j = l->distancia_otras_lineas[j];
				if(dist_j < min && dist_j != -1.0 && t->estados_de_lineas[t->index_en_lineas_por_color[i]+j] == 'W')
				{
					if(dist_j == 0)
					{
						fprintf(stderr, "WUT\n\n");
					}
					index = t->index_en_lineas_por_color[i]+j;
					min = dist_j;
				}
			}
			l->goal = index;
		}
	}			
}

void print_telar(Telar* t)
{
	int i = 0;
	fprintf(stderr,"Se imprimira el telar\n");
	fprintf(stderr,"\n");
	for(i = 0; i < t->lineas_count; i++)
	{
		print_linea(t->lineas[i]);
	}
	print_telar_colores(t);
	fprintf(stderr,"\n");
}

void print_telar_colores(Telar* t)
{
	int i = 0;
	fprintf(stderr,"Se imprimiran los colores del telar\n");
	fprintf(stderr,"\n");
	fprintf(stderr,"Colores: \t[color, cantidad]\n ");
	for(i = 0; i < t->colores_count; i++)
	{
		fprintf(stderr,"\n");	
		fprintf(stderr," [%d,%d] ,",t->colores[i],t->lineas_por_color[i]);
	}
	fprintf(stderr,"\n");
}

void destroy_telar(Telar* t)
{

	int i = 0;
	for(i = 0; i < t->lineas_count; i++)
	{
		free(t->lineas[i]);
	}
	free(t->lineas);
	free(t->colores);
	free(t->lineas_por_color);
	free(t->index_en_lineas_por_color);
	free(t->estados_de_lineas);
	free(t);
}


void print_solucion_final_desde_telar(Telar* t)
{
	int i = 0;
	for(i = 0; i < t->lineas_count; i++)
	{
		print_linea(t->lineas[i]);
		if(t->lineas[i]->largo > 0)
		{
			fprintf(stderr, "ME CAIGO ACA :( i es %d\n" ,i);
			print_solucion_desde_nodo(t->lineas[i]->actual);
		}
	}
}