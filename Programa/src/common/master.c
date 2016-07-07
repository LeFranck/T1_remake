#include "master.h"
#include "telar.h"
#include "solucion.h"
#include "stats.h"
#include "nodo_backtracking.h"

//------------------------------------		LISTOS	 	-----------------------------------------//
Master* create_master(Layout* l)
{
	Master* m = malloc(sizeof(Master));
	m->l = l;
	m->t = create_telar(l);
	m->s = create_solucion(m->t->c_m);
	m->stats = create_stats(m->t);
	return m;
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
//------------------------------------		LISTOS		 -----------------------------------------//




//------------------------------------		Falta Completar -----------------------------------------//
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



void Solve(Master* m)
{
	while(m->s->ready != 'T')
	{
		tejer(m);
		//fprintf(stderr,"Tejido completo \n ");
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


void conectar_linea(Master* m, Linea* l)
{
	if(l->isReady!='T')
	{
		l->actual = create_nodo_backtracking(l->number);
	}
	while(l->isReady!='T' && l->dead!='T')
	{
		mover_linea(m,l);
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
	reset_goal_linea(m->t,l);
	if(l->deadEnd == 'T')
	{
		//fprintf(stderr,"Linea %d llego a un deadEnd\n",l->number);
		retroceder_linea(m,l);
	}else{
		avanzar_linea(m,l);
	}
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
				l->actual->opciones[i] = 'V';
				validas++;
			}
			//Tengo a la meta en mi zona
			if(z->buildings[i]->color == l->color && l->cabeza->b != i)
			{
				fprintf(stderr,"opcion %d Mismo color y distintos edificios\n",i);
				l->actual->opciones[i] = 'V';
				validas++;
				l->actual->meta = 'T';
			}
		}else{
			l->actual->opciones[i] = 'F';
		}
	}
	l->direccion = direccion_desde(l->cabeza, m->t->lineas[l->goal]->cabeza);
	if(validas == 0)
	{
		l->deadEnd ='T';
	}else{
		l->actual->validas_count = validas;
	}
	//DEBUG DEL DEADEND
	//fprintf(stderr,"Opciones linea %d: \n",l->number);
	//for(i = 0; i < 8; i++)
	//{
		//fprintf(stderr,"\t");
		//fprintf(stderr,"- [%d,%c]",i,l->actual->opciones[i]);
	//}
	//fprintf(stderr,"\n\n");

}

void avanzar_linea(Master* m, Linea* l)
{
	if(l->actual->meta == 'T')
	{
		conectar_meta(m,l);
		fprintf(stderr,"Conecte a meta\n");
	}else{
		conectar_a_blanco(m,l);
		fprintf(stderr,"Conecte a blanco\n");
	}
}

void conectar_meta(Master* m, Linea* l)
{
	//No tengo que pintar a nadie :)
	//fprintf(stderr,"Conectareeeee linea %d a la meta %d\n",l->number,m->t->lineas[l->goal]->cabeza->b);
	conectar_linea_a_edificio(m, l, m->t->lineas[l->goal]->cabeza->b);
	//actualizar datos en ambas lineas
	//double dir = direccion_desde(l->cabeza, m->t->lineas[l->goal]->cabeza);
	//double dir2 = dir * (-1.0);
	actualizar_linea(l, m->t->lineas[l->goal]->cabeza,'T', -2.0 ,m->t->lineas[l->goal]->cabeza->b);
	actualizar_linea(m->t->lineas[l->goal],m->t->lineas[l->goal]->cabeza, 'T', -2.0 ,m->t->lineas[l->goal]->cabeza->b);
}

void conectar_a_blanco(Master* m, Linea* l)
{
	int b = chose_white_move(m, l, 1);
	//fprintf(stderr,"Conectareeeee linea %d al edificio %d\n",l->number,b);
	conectar_linea_a_edificio(m,l,b);
	//fprintf(stderr,"Conecte linea %d al edificio %d\n",l->number,b);
	//Expandir color
	Zone* z = m->l->zones[l->cabeza->z];
	Client* c1 = z->buildings[l->cabeza->b];
	Client* c_lejano = c1->linked[1]->linked[0];
	c1->linked[1]->color = l->color;
	c_lejano->color = l->color;

	int z_ = c_lejano->zone->index;
	int b_ = c_lejano->index;
	int x_ = c_lejano->zone->x;
	int y_ = c_lejano->zone->y;
	Posicion* nueva = create_posicion(z_,b_,x_,y_);
	double dir = direccion_desde(nueva, m->t->lineas[l->goal]->cabeza);
	actualizar_linea(l, nueva, 'F', dir, b);
}

int chose_white_move(Master* m , Linea* l, int seed)
{
	//La diferencia de X es pasada para evitar los bugs del arctan
	int diff_x = dist_x(l->cabeza, m->t->lineas[l->goal]->cabeza);
	int building_obj = get_building_objetivo(l, diff_x);
	if(seed == 1)
	{
		return movimiento_estilo_a(l,building_obj);
	}else if(seed == 2)
	{
		return movimiento_estilo_b(l,building_obj);
	}else if(seed == 3)
	{
		return movimiento_estilo_c(l,building_obj);
	}else{
		return movimiento_estilo_d(l,building_obj);
	}
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

//------------------------------------Falta Completar -----------------------------------------//




//------------------------------------		Vacios		-----------------------------------------//
void Optimizar_largos(Master* m){}
void AntiMonopolio(Master* m){}
void descocer(Master* m){}
void retroceder_linea(Master* m, Linea* l){}
//------------------------------------		Vacios 		-----------------------------------------//

