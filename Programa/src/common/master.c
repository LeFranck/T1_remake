#include "master.h"
#include "telar.h"
#include "solucion.h"
#include "stats.h"
#include "nodo_backtracking.h"

Master* create_master(Layout* l)
{

	Master* m = malloc(sizeof(Master));
	m->l = l;
	m->t = create_telar(l);
	m->s = create_solucion(m->t->c_m);
	m->stats = create_stats(m->t);
	return m;
}

void solve_puzzle(Master* m, char condicion)
{

	if(condicion == 'O')
	{
		Optimizar_largos(m);
	}else if(condicion == 'M')
	{
		AntiMonopolio(m);
	}else{
		Solve(m);
	}
}

void Optimizar_largos(Master* m){}

void AntiMonopolio(Master* m){}

void Solve(Master* m)
{
	while(m->s->ready != 'T')
	{
		tejer(m);
		if(m->s->ready != 'T')
		{
			descocer(m);
		}
	}
}

void tejer(Master* m)
{
	int i = 0;
	for(i = 0; i < m->t->lineas_count; i++)
	{
		conectar_linea(m, m->t->lineas[i]);
	}
}

void descocer(Master* m)
{

}

void conectar_linea(Master* m, Linea* l)
{
	if(l->isReady!='T')
	{
		l->actual = create_nodo_backtracking(l->number);
	}
	while(l->isReady!='T' && l->dead!='T')
	{
		mover_linea(m,l);
		fprintf(stderr,"Movi linea :) \n ");
	}
	if(l->isReady=='T')
	{
		//R de ready
		m->t->estados_de_lineas[l->number] = 'R';
		m->t->estados_de_lineas[l->goal] = 'R';
	}else{
		//D de dead
		m->t->estados_de_lineas[l->number] = 'D';
		m->t->estados_de_lineas[l->number] = 'D';
	}
	//Podriamos eliminar el nodo backtracking de cada linea
}

void mover_linea(Master* m,Linea* l)
{
	calcular_opciones_linea(m,l);
	if(l->deadEnd == 'T')
	{
		retroceder_linea(m,l);
	}else{
		avanzar_linea(m,l);
	}
	fprintf(stderr,"avanze linea :) \n ");
}

void calcular_opciones_linea(Master* m, Linea* l)
{
	Zone* z = m->l->zones[l->cabeza->z];
	int i = 0;
	int validas = 0;
	int b_c = z->building_count;
	l->actual->building_count = b_c;
	for(i = 0; i< b_c; i++)
	{
		if(!city_client_is_taken(z->buildings[i]))
		{
			if(city_client_is_blank(z->buildings[i]))
			{
				l->actual->opciones[i] = 'T';
				validas++;
			}
			if(z->buildings[i]->color == l->color)
			{
				l->actual->opciones[i] = 'T';
				validas++;
				l->actual->meta = 'T';
			}
		}
	}
	l->direccion = direccion_desde(l->cabeza, m->t->lineas[l->goal]->cabeza);
	if(validas == 0)
	{
		l->deadEnd ='T';
	}else{
		l->actual->validas_count = validas;
	}
}

void retroceder_linea(Master* m, Linea* l)
{


}

void avanzar_linea(Master* m, Linea* l)
{
	if(l->actual->meta == 'T')
	{
		conectar_meta(m,l);
	}else{
		conectar_a_blanco(m,l);
	}
}

void conectar_meta(Master* m, Linea* l)
{
	//No tengo que pintar a nadie :)
	conectar_linea_a_edificio(m, l, m->t->lineas[l->goal]->cabeza->b);
	//actualizar datos en ambas lineas
	double dir = direccion_desde(l->cabeza, m->t->lineas[l->goal]->cabeza);
	double dir2 = dir * (-1.0);
	fprintf(stderr,"Calcule direccion :)\n ");
	actualizar_linea(l, m->t->lineas[l->goal]->cabeza, 'T', dir);
	fprintf(stderr,"actualize linea 1 :) \n ");
	actualizar_linea(m->t->lineas[l->goal],m->t->lineas[l->goal]->cabeza, 'T', dir2);
	fprintf(stderr,"actualize linea 2 :) \n ");

}

void conectar_a_blanco(Master* m, Linea* l)
{

}

//agregar conexion en funcion del index linea o al final???
void conectar_linea_a_edificio(Master* m, Linea* l, int b)
{
	//client link
	Zone* z = m->l->zones[l->cabeza->z];
	Client* c1 = z->buildings[l->cabeza->b];
	Client* c2 = z->buildings[b];
	c1->linked[c1->link_count] = c2;
	c1->link_count++;
	c2->linked[c2->link_count] = c1;
	c2->link_count++;
	agregar_conexion_a_solucion(m->s,z->index,c1->index,c2->index);
}



void juntar_lineas_por_color(Master* m)
{
	Telar* r = clon_telar(m->t);
	int i = 0;
	int j = 0;
	int index = 0;
	int line_count = 0;
	for(i = 0; i < r->colores_count; i++)
	{
		m->t->index_en_lineas_por_color[i] = line_count; 
		index = 0;
		for(j = 0; j < r->lineas_por_color[i]; j++)
		{
			while(m->t->lineas[index]->color != r->colores[i])
			{
				index++;
			}
			r->lineas[line_count] = m->t->lineas[index];
			r->lineas[line_count]->number = line_count;
			line_count++;
			index++;
		}
	}
	m->t = r;
}

void destroy_master(Master* m)
{
	destroy_solucion(m->s);
	destroy_stats(m->stats);
	destroy_telar(m->t);
	free(m);
}
