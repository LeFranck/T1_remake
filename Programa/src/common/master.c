#include "master.h"
#include "telar.h"
#include "solucion.h"
#include "stats.h"
#include "nodo_backtracking.h"
#include <stdio.h>
#include <stdlib.h>

//------------------------------------		LISTOS	 	-----------------------------------------//
Master* create_master(Layout* l)
{
	Master* m = malloc(sizeof(Master));
	m->l = l;
	m->t = create_telar(l);
	m->s = create_solucion(m->t->c_m);
	m->stats = create_stats(m->t);
	m->stats->conexiones_max = m->s->conexiones_max; 
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

void avanzar_linea(Master* m, Linea* l)
{
	if(l->actual->meta == 'T')
	{
		conectar_meta(m,l);
		//fprintf(stderr,"Conecte a meta\n");
	}else{
		conectar_a_blanco(m,l);
		////fprintf(stderr,"Conecte a blanco\n");
	}

	if(l->randomnes == 0){
		l->randomnes = 91;
	}else{
		l->randomnes = 0;
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
	actualizar_linea(l, m->t->lineas[l->goal]->cabeza,'T',m->t->lineas[l->goal]->cabeza->b);

	actualizar_linea(m->t->lineas[l->goal],m->t->lineas[l->goal]->cabeza, 'T' ,m->t->lineas[l->goal]->cabeza->b);
	//m->t->lineas[l->goal]->isReady = 'T';
}

void conectar_a_blanco(Master* m, Linea* l)
{
	//Problemas seed 3 y 4
	int b = chose_white_move(m, l, 2);
	//int b = chose_white_move(m, l, 1);
	////fprintf(stderr,"Conectareeeee linea %d al edificio %d\n",l->number,b);
	conectar_linea_a_edificio(m,l,b);
	////fprintf(stderr,"Conecte linea %d al edificio %d\n",l->number,b);
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
	//double dir = direccion_desde(nueva, m->t->lineas[l->goal]->cabeza);
	actualizar_linea(l, nueva,'F',b);
}

int chose_white_move(Master* m , Linea* l, int seed)
{
	//La diferencia de X es pasada para evitar los bugs del arctan
	//int diff_x = dist_x(l->cabeza, m->t->lineas[l->goal]->cabeza);
	//int building_obj = get_building_objetivo(l, diff_x);

	//Este int servira para direfenciar runbow de landprotector
	Client* vecino0 =  m->l->zones[l->cabeza->z]->buildings[0]->linked[0];
	int vecino0_x = vecino0->zone->x;
	int vecino0_y = vecino0->zone->y;
	int building_obj = get_building_objetivo(l, m->t->lineas[l->goal], vecino0_x, vecino0_y);

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
	//SOLUCION DE LA LINEA
	l->actual->z_index = z->index;
	l->actual->c1_index = c1->index;
	l->actual->c2_index = c2->index;
	m->stats->conexiones_hechas++;
	//agregar_conexion_a_solucion(m->s,z->index,c1->index,c2->index);
}

void conectar_linea(Master* m, Linea* l)
{
	if(l->isReady!='T')
	{
		l->actual = malloc(sizeof(NodoBacktracking));
		l->actual = create_nodo_backtracking(l->number);
	}
	if(l->isReady!='T' && l->dead!='T'){
		//CADA LINEA SABE DONDE PARTE SU SOLUCION
		//l->inicio_solucion = m->s->cabeza;
		while(l->isReady!='T' && l->dead!='T')
		{
			////fprintf(stderr, "Movere la linea %d que esta en estado %c\n",l->number, l->isReady );
			mover_linea(m,l);
			if(l->isReady!='T' && l->dead!='T'){
				set_dintacias_otras_lineas_desde_linea(m->t, l->color, l->number);
				reset_goal_linea(m->t, l);
			}
		}
		if(l->isReady=='T')
		{
			//R de ready
			m->t->estados_de_lineas[l->number] = 'R';
			m->t->estados_de_lineas[l->goal] = 'R';
		}else{
			//D de dead
			m->t->estados_de_lineas[l->number] = 'D';
		}
	}
	//Podriamos eliminar el nodo backtracking de cada linea
}

void retroceder_linea(Master* m, Linea* l)
{
	if(l->largo > 0)
	{
		int z = l->actual->prev->z_index;
		int b1 = l->actual->prev->c1_index;
		int b2 = l->actual->prev->c2_index;
		//int x = m->l->zones[z]->x;
		//int y = m->l->zones[z]->y;
		////fprintf(stderr,"Retrocedere con %d movimientos y [x,y,b1,b2] = [%d,%d,%d,%d] \n", l->largo,x,y,b1,b2);
		Client* c1 = m->l->zones[z]->buildings[b1];
		Client* c2 = m->l->zones[z]->buildings[b2];
		//Lavo los inter-zonas
		c2->linked[0]->color = 0;
		c2->color = 0;
		//Desconecto el link
		c1->linked[1] = NULL;
		c1->link_count--;
		c2->linked[1] = NULL;
		c2->link_count--;
		l->largo--;
		m->stats->conexiones_hechas--;
		//Cambio la cabeza de la linea
		l->cabeza->z = z;
		l->cabeza->b = b1;
		l->cabeza->x = m->l->zones[z]->x;
		l->cabeza->y = m->l->zones[z]->y;
		//Actualizo en nodo_op
		NodoBacktracking* nod = malloc(sizeof(NodoBacktracking));
		//MIEDO 
		//nod->prev = pop_nodo_backtracking(l->actual);
		nod = pop_nodo_backtracking(l->actual);
		//free(l->actual);
		l->actual = nod;
		l->actual->opciones[b2] = 'P';
	}else{
		////fprintf(stderr,"Estoy tratando de retroceder una linea sin movs \n");
		////fprintf(stderr,"Tiene %d movimientos \n", l->largo);
		////fprintf(stderr,"DEAD LINE \n");
		l->actual->z_index = 0;
		l->actual->c1_index = 0;
		l->actual->c2_index = 0;
		l->dead = 'T';
	}
}

void tejer(Master* m)
{
	int i = 0;
	for(i = 0; i < m->t->lineas_count; i++)
	{
		//Si la meta de la linea esta ok
		if(m->t->lineas[m->t->lineas[i]->goal]->isReady == 'T' || m->t->lineas[m->t->lineas[i]->goal]->dead == 'T')
		{
			//fprintf(stderr,"La meta de la linea %d esta ok \n",i);
			//Si la linea no esta ok
			if(m->t->lineas[i]->isReady != 'T' && m->t->lineas[i]->dead != 'T')
			{
				//Cambio la meta de la linea
				////fprintf(stderr,"Linea %d, meta: %d tomada\nPosicion meta ",i, m->t->lineas[i]->goal );
				//print_posicion(m->t->lineas[m->t->lineas[i]->goal]->cabeza);
				////fprintf(stderr,"RESET GOAL xq linea is%c\n",m->t->lineas[i]->isReady);
				reset_goal_linea(m->t,m->t->lineas[i]);
				////fprintf(stderr,"Nueva posicion meta\n" );
				print_posicion(m->t->lineas[m->t->lineas[i]->goal]->cabeza);
			}
		}
		////fprintf(stderr,"\nLinea %d, meta: %d\n",i, m->t->lineas[i]->goal);
		print_posicion(m->t->lineas[m->t->lineas[i]->goal]->cabeza);
		conectar_linea(m, m->t->lineas[i]);
		//fprintf(stderr,"\n\n");
	}
}

void mover_linea(Master* m,Linea* l)
{
	calcular_opciones_linea(m,l);
	if(l->deadEnd == 'T')
	{
		//fprintf(stderr,"Retrocedere Linea %d ya que llego a un deadEnd\n\n\n",l->number);
		//l->dead = 'T';
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
	l->actual->zone_sides = z->sides;
	for(i = 0; i< b_c; i++)
	{
		if(!city_client_is_taken(z->buildings[i]))
		{
			if(city_client_is_blank(z->buildings[i]))
			{
				l->actual->opciones[i] = 'T';
				validas++;
				//fprintf(stderr, "HITO %d\n\n",i);
			}
			//Tengo a la meta en mi zona
			if(z->buildings[i]->color == l->color && l->cabeza->b != i)
			{
				//fprintf(stderr,"opcion %d Mismo color y distinto edifico que %d\n",i,l->cabeza->b);
				l->actual->opciones[i] = 'T';
				validas++;
				l->actual->meta = 'T';
			}
		}else{
			l->actual->opciones[i] = 'F';
		}
	}
	if(validas == 0)
	{
		l->deadEnd ='T';
	}else{
		l->actual->validas_count = validas;
	}
	//DEBUG DEL DEADEND
	//fprintf(stderr,"Opciones linea %d: \n",l->number);
	for(i = 0; i < 8; i++)
	{
		//fprintf(stderr,"\t");
		//fprintf(stderr,"- [%d,%c]",i,l->actual->opciones[i]);
	}
	//fprintf(stderr,"\n\n");
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
		////fprintf(stderr,"Tejido completo \n ");
		if(m->s->ready != 'T')
		{
			descocer(m);
		}
	}
}

void solve_deads_lines(Master* m, int IA)
{
	if(IA == 0)
	{
		IA_dead_lines0(m);
	}else if(IA == 1)
	{
		IA_dead_lines1(m);
	}else if(IA == 2){
		IA_dead_lines2(m);
	}else{
		fprintf(stderr, "NO SEED\n");
	}
}

void deal_with_dead_lines0(Master* m)
{
	if(m->stats->lineas_muertas_count != 0)
	{
		Linea* larga = m->t->lineas[m->stats->linea_mas_larga];
		Linea* larga_contraparte = m->t->lineas[larga->goal];
		limpiar_linea(m,larga_contraparte);
		limpiar_linea(m,larga);
	}
	int i =0;
	for(i = 0; i < m->t->lineas_count; i++)
	{
		Linea* linea = m->t->lineas[i];
		if(linea->dead == 'T')
		{
			limpiar_linea(m,linea);
		}
	}
	for(i = 0; i < m->t->lineas_count; i++)
	{
		Linea* linea = m->t->lineas[i];
		if(m->t->estados_de_lineas[i] == 'W')
		{
			reset_goal_linea(m->t, linea);
		}
	}

	tejer(m);
}

//Limpia la larga_number
//Conecta las deads, conecta la que limpio al inicio
void deal_with_dead_lines1(Master* m, int larga_number)
{
	Linea* larga = m->t->lineas[larga_number];
	Linea* larga_contraparte = m->t->lineas[larga->goal];
	limpiar_linea(m,larga_contraparte);
	limpiar_linea(m,larga);

	int i =0;
	int j = 0;
	int* muertas = malloc(sizeof(int)*m->stats->lineas_muertas_count);
	for(i = 0; i < m->t->lineas_count; i++)
	{
		Linea* linea = m->t->lineas[i];
		if(linea->dead == 'T')
		{
			limpiar_linea(m,linea);
			muertas[j] = i;
			j++;
		}
	}
	for(i = 0; i < m->t->lineas_count; i++)
	{
		Linea* linea = m->t->lineas[i];
		if(m->t->estados_de_lineas[i] == 'W')
		{
			reset_goal_linea(m->t, linea);
		}
	}
	for(i = 0; i < m->stats->lineas_muertas_count; i++)
	{
		reset_goal_linea(m->t,m->t->lineas[muertas[i]]);
		conectar_linea(m,m->t->lineas[muertas[i]]);
	}
	reset_goal_linea(m->t, larga_contraparte);
	conectar_linea(m,larga_contraparte);
}

void IA_dead_lines0(Master* m)
{
	int i = 0;
	while(m->stats->lineas_muertas_count != 0 )
	{
		deal_with_dead_lines0(m);
		m->stats = update_stats(m->t, m->l, m->stats);
		fprintf(stderr, "%d\n",i );
		i++;
		if(i == 100){m->stats->lineas_muertas_count = 0;}
	}
}

void IA_dead_lines1(Master* m)
{
	int i = 0;
	while(m->stats->lineas_muertas_count != 0 )
	{
		deal_with_dead_lines1(m, m->stats->linea_mas_larga);
		m->stats = update_stats(m->t, m->l, m->stats);
		fprintf(stderr, "%d\n",i );
		i++;
		if(i == 1000){m->stats->lineas_muertas_count = 0;}
	}
}

//Random 
void IA_dead_lines2(Master* m)
{
	int i = 0;
	while(m->stats->lineas_muertas_count != 0 )
	{
		time_t t;
   		srand((unsigned) time(&t));
		int random =  rand() % m->t->lineas_count ;
   		int number = ((random + i )%m->t->lineas_count);
   		while(m->t->lineas[number]->largo <= 1)
   		{
   			random = rand() % m->t->lineas_count ;
   			number = ((random + i )%m->t->lineas_count);
   		}
		deal_with_dead_lines1(m, number);
		m->stats = update_stats(m->t, m->l, m->stats);
		//fprintf(stderr, "itareacion %d\t number: %d\n",i,number );
		i++;
		if(i == 20000){
			fprintf(stderr, "20000 Desconecciones :O\n" );
		}
	}
	fprintf(stderr, "\n%d Desconecciones :O\n",i );	
}

char solver_alargar_conection0(Master* m)
{
	int linea_candidata = buscar_candidato_a_desconectar0(m);
	if(linea_candidata == -1)
	{
		return 'F';
	}else{
		desenchufar_linea0(m,linea_candidata);
		dummy_conection0(m,linea_candidata);
		conectar_linea_alargar0(m, m->t->lineas[linea_candidata]);
		return 'T';
	}
}

void solver_alargar_conexiones(Master* m)
{
	int k = 0;
	while(m->stats->conexiones_hechas != m->stats->conexiones_max)
	{
		int i = 0;
		//fprintf(stderr, "\n %d Desconecciones\n",k );
		for( i = 0; i < m->t->lineas_count; i++)
		{
			if(m->stats->lineas_candidatas[i]== 'T')
			{
				if(linea_es_candidata(m,m->t->lineas[i])=='T')
				{
					desenchufar_linea0(m,i);
					dummy_conection0(m,i);
					conectar_linea_alargar0(m,m->t->lineas[i]);
					k++;
				}
			}
		}
		//fprintf(stderr, "\n %d Desconecciones\n",k );
		for(i = 0; i < m->t->lineas_count; i++)
		{
			if(m->stats->lineas_candidatas[i]== 'T')
			{
				if(m->t->lineas[i]->largo > 3){
					retroceder_avanzar_lineas(m,m->t->lineas[i],m->t->lineas[m->t->lineas[i]->goal]);
				}else{
					m->stats->lineas_candidatas[i] = 'F';					
				}
				//if(m->t->lineas[i]->largo <= 0)
				//{
				//}
			}
		}
		if(k == 12)
		{
			fprintf(stderr, "\n 10000 Desconecciones :O\n" );
			break;
		}
	}
	fprintf(stderr, "\n %d Desconecciones\n",k );
}

char linea_es_candidata(Master* m, Linea* l)
{
	int linea = l->number;
	char r = 'F';
	int j = 0;
	if(m->stats->lineas_candidatas[linea] == 'T')
	{
		int z_cabeza = m->t->lineas[linea]->cabeza->z;
		for(j = 0; j < m->l->zones[z_cabeza]->building_count ; j++)
		{
			if(!city_client_is_taken(m->l->zones[z_cabeza]->buildings[j]))
			{
				r = 'T';
			}
		}
	}
	return r;
}

void conectar_linea_alargar0(Master* m, Linea* l)
{
	if(l->isReady!='T' && l->dead!='T'){
			//CADA LINEA SABE DONDE PARTE SU SOLUCION
			//l->inicio_solucion = m->s->cabeza;
			while(l->isReady!='T' && l->dead!='T')
			{
				////fprintf(stderr, "Movere la linea %d que esta en estado %c\n",l->number, l->isReady );
				mover_linea(m,l);
				if(l->isReady!='T' && l->dead!='T'){
					set_dintacias_otras_lineas_desde_linea(m->t, l->color, l->number);
					reset_goal_linea(m->t, l);
				}
			}
			if(l->isReady=='T')
			{
				//R de ready
				m->t->estados_de_lineas[l->number] = 'R';
				m->t->estados_de_lineas[l->goal] = 'R';
			}else{
				//D de dead
				m->t->estados_de_lineas[l->number] = 'D';
			}
		}
}

int buscar_candidato_a_desconectar0(Master* m)
{
	int i = 0;
	int j = 0;
	int largo_candidato = 10000000;
	int index_candidato = -1;
	for(i = 0; i < m->t->lineas_count; i++)
	{
		if(m->t->lineas[i]->largo > 0)
		{
			int z_cabeza = m->t->lineas[i]->cabeza->z;
			for(j = 0; j < m->l->zones[z_cabeza]->building_count ; j++)
			{
				if(!city_client_is_taken(m->l->zones[z_cabeza]->buildings[j]))
				{
					if(m->t->lineas[i]->largo < largo_candidato)
					{
						largo_candidato = m->t->lineas[i]->largo;
						index_candidato = i;
					}
				}
			}
		}
	}
	return index_candidato;
}

void desenchufar_linea0(Master* m , int linea)
{
	Linea* l = m->t->lineas[linea];
	Linea* goal = m->t->lineas[l->goal];
	Client* c1 = m->l->zones[l->cabeza->z]->buildings[l->cabeza->b];
	Client* c2 = c1->linked[1];
	Posicion* cabeza_previa = create_posicion(l->cabeza->z, c2->index,l->cabeza->x,l->cabeza->y);
	l->cabeza = cabeza_previa;
	c2->linked[1] = NULL;
	c2->link_count--;
	c1->linked[1] = NULL;
	c1->link_count--;
	m->stats->conexiones_hechas--;
	NodoBacktracking* n = pop_nodo_backtracking(l->actual);
	n->z_index = 0;
	n->c1_index = 0;
	n->c2_index = 0;
	int i = 0;
	for(i = 0; i < 8 ; i++){
		n->opciones[i] = 'F';
	}
	l->actual = n;
	l->isReady = 'F';
	l->dead = 'F';
	l->deadEnd = 'F';
	l->actual->meta = 'F';
	m->t->estados_de_lineas[l->number] = 'W';
	goal->isReady = 'F';
	goal->dead = 'F';
	goal->deadEnd = 'F';
	m->t->estados_de_lineas[goal->number] = 'W';
}

void dummy_conection0(Master* m, int linea)
{
	Linea* l = m->t->lineas[linea];
	int z_cabeza = l->cabeza->z;
	int b = -1;
	int j = 0;
	for(j = 0; j < m->l->zones[z_cabeza]->building_count ; j++)
	{
		if(city_client_is_blank(m->l->zones[z_cabeza]->buildings[j]))
		{
			b = j;
		}
	}
	conectar_linea_a_edificio(m,l,b);
	////fprintf(stderr,"Conecte linea %d al edificio %d\n",l->number,b);
	//Expandir color
	Zone* z = m->l->zones[l->cabeza->z];
	Client* c1 = z->buildings[l->cabeza->b];
	Client* c_lejano = c1->linked[1]->linked[0];
	c1->linked[1]->color = l->color;
	c_lejano->color = l->color;
	l->largo++;
	int z_ = c_lejano->zone->index;
	int b_ = c_lejano->index;
	int x_ = c_lejano->zone->x;
	int y_ = c_lejano->zone->y;
	Posicion* nueva = create_posicion(z_,b_,x_,y_);
	//double dir = direccion_desde(nueva, m->t->lineas[l->goal]->cabeza);
	actualizar_linea(l, nueva,'F',b);
}

void limpiar_linea(Master* m , Linea* l)
{
	if(l->largo == 0)
	{
		l->isReady = 'F';
		l->dead = 'F';
		l->deadEnd = 'F';
		m->t->estados_de_lineas[l->number] = 'W';
	}else{
		if(l->isReady == 'T')
		{
			Client* c1 = m->l->zones[l->cabeza->z]->buildings[l->cabeza->b];
			Client* c2 = c1->linked[1];
			Posicion* cabeza_previa = create_posicion(l->cabeza->z, c2->index,l->cabeza->x,l->cabeza->y);
			l->cabeza = cabeza_previa;
			c2->linked[1] = NULL;
			c2->link_count--;
			c1->linked[1] = NULL;
			c1->link_count--;
			m->stats->conexiones_hechas--;
			NodoBacktracking* n = pop_nodo_backtracking(l->actual);
			n->z_index = 0;
			n->c1_index = 0;
			n->c2_index = 0;
			int i = 0;
			for(i = 0; i < 8 ; i++){
				n->opciones[i] = 'F';
			}
			l->actual = n;
			l->isReady = 'F';
			l->dead = 'F';
			l->deadEnd = 'F';
			m->t->estados_de_lineas[l->number] = 'W';
			l->largo--;
			if(l->largo > 0){
				while(l->largo > 0 )
				{
					retroceder_linea(m,l);
				}
				l->actual->z_index = 0;
				l->actual->c1_index = 0;
				l->actual->c2_index = 0;
			}

		}else{
			if(l->largo > 0)
			{
				while(l->largo > 0 )
				{
					retroceder_linea(m,l);
				}
				l->actual->z_index = 0;
				l->actual->c1_index = 0;
				l->actual->c2_index = 0;
			}
		l->isReady = 'F';
		l->dead = 'F';
		l->deadEnd = 'F';
		m->t->estados_de_lineas[l->number] = 'W';
		}
	}
}
//------------------------------------Falta Completar -----------------------------------------//
void descocer(Master* m)
{

}
//------------------------------------		Vacios		-----------------------------------------//
void Optimizar_largos(Master* m){}
void AntiMonopolio(Master* m){}
//------------------------------------		Vacios 		-----------------------------------------//

//L1 retrocedera y l2 avanzara
void retroceder_avanzar_lineas(Master* m, Linea* l1, Linea* l2)
{
	l1->actual = pop_nodo_backtracking(l1->actual);
	if(l2->largo == 0)
	{
		l2->actual = create_nodo_backtracking(l2->number);
	}
	//Doy el mov a l2
	l2->actual-> z_index = l1->actual-> z_index;
	l2->actual-> c1_index = l1->actual-> c2_index;
	l2->actual-> c2_index = l1->actual-> c1_index;	
	
	//Le quito el mov a l1
	l1->actual-> z_index = 0;
	l1->actual-> c1_index = 0;
	l1->actual-> c2_index = 0;

	NodoBacktracking* n = create_nodo_backtracking(l2->number);
	l2->actual = push_nodo_backtracking(l2->actual, n);

	NodoBacktracking* prev = l1->actual->prev;
	int x =  m->l->zones[prev->z_index]->x;
	int y =  m->l->zones[prev->z_index]->y;
	Posicion* cabeza_nueva = create_posicion(prev->z_index, prev->c2_index,x,y);
	l2->cabeza = cabeza_nueva;
	l1->cabeza = cabeza_nueva;
	l2->largo++;
	l1->largo--;
}

void tejer_debug(Master* m, int l1, int l2)
{
	int i = 0;
	for(i = 0; i < m->t->lineas_count; i++)
	{
		//Si la meta de la linea esta ok
		if(m->t->lineas[m->t->lineas[i]->goal]->isReady == 'T' || m->t->lineas[m->t->lineas[i]->goal]->dead == 'T')
		{
			//fprintf(stderr,"La meta de la linea %d esta ok \n",i);
			//Si la linea no esta ok
			if(m->t->lineas[i]->isReady != 'T' && m->t->lineas[i]->dead != 'T')
			{
				//Cambio la meta de la linea
				////fprintf(stderr,"Linea %d, meta: %d tomada\nPosicion meta ",i, m->t->lineas[i]->goal );
				//print_posicion(m->t->lineas[m->t->lineas[i]->goal]->cabeza);
				////fprintf(stderr,"RESET GOAL xq linea is%c\n",m->t->lineas[i]->isReady);
				reset_goal_linea(m->t,m->t->lineas[i]);
				////fprintf(stderr,"Nueva posicion meta\n" );
				print_posicion(m->t->lineas[m->t->lineas[i]->goal]->cabeza);
			}
		}
		////fprintf(stderr,"\nLinea %d, meta: %d\n",i, m->t->lineas[i]->goal);
		print_posicion(m->t->lineas[m->t->lineas[i]->goal]->cabeza);
		if(i != l1 && i != l2){
			conectar_linea(m, m->t->lineas[i]);			
		}
		//fprintf(stderr,"\n\n");
	}
}

void tejer_inverso(Master* m)
{
	int i = 0;
	for(i = m->t->lineas_count-1; i > 0; i--)
	{
		//Si la meta de la linea esta ok
		if(m->t->lineas[m->t->lineas[i]->goal]->isReady == 'T' || m->t->lineas[m->t->lineas[i]->goal]->dead == 'T')
		{
			//fprintf(stderr,"La meta de la linea %d esta ok \n",i);
			//Si la linea no esta ok
			if(m->t->lineas[i]->isReady != 'T' && m->t->lineas[i]->dead != 'T')
			{
				//Cambio la meta de la linea
				////fprintf(stderr,"Linea %d, meta: %d tomada\nPosicion meta ",i, m->t->lineas[i]->goal );
				//print_posicion(m->t->lineas[m->t->lineas[i]->goal]->cabeza);
				////fprintf(stderr,"RESET GOAL xq linea is%c\n",m->t->lineas[i]->isReady);
				reset_goal_linea(m->t,m->t->lineas[i]);
				////fprintf(stderr,"Nueva posicion meta\n" );
				print_posicion(m->t->lineas[m->t->lineas[i]->goal]->cabeza);
			}
		}
		////fprintf(stderr,"\nLinea %d, meta: %d\n",i, m->t->lineas[i]->goal);
		conectar_linea(m, m->t->lineas[i]);			
		//fprintf(stderr,"\n\n");
	}
}

void clean_dead_lines(Master* m)
{
	int i =0;
	for(i = 0; i < m->t->lineas_count; i++)
	{
		Linea* linea = m->t->lineas[i];
		if(linea->dead == 'T')
		{
			limpiar_linea(m,linea);
		}
	}
	for(i = 0; i < m->t->lineas_count; i++)
	{
		Linea* linea = m->t->lineas[i];
		if(m->t->estados_de_lineas[i] == 'W')
		{
			reset_goal_linea(m->t, linea);
		}
	}
}