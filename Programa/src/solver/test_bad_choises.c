#include "../common/city.h"
#include "../common/posicion.h"
#include "../common/linea.h"
#include "../common/telar.h"
#include "../common/solucion.h"
#include "../common/master.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
	/* Leemos la ciudad del input */
	Layout* layout = city_layout_read(stdin);

	/* Lo imprimimos para darle el estado inicial al watcher / judge */
	city_layout_print(layout);

	/* TODO RESOLVER PROBLEMA */
	Master* m = create_master(layout);
	ordenar_colores(m->t);
	juntar_lineas_por_color(m);
	set_lines_goals(m->t);
	print_telar(m->t);
	
	tejer(m);
	clean_dead_lines(m);
	tejer_inverso(m);
	clean_dead_lines(m);
	tejer(m);

	m->stats = update_stats(m->t, m->l, m->stats);
	print_stats(m->stats);

	solve_deads_lines(m,2);

	m->stats = update_stats(m->t, m->l, m->stats);
	print_stats(m->stats);
	Linea* l1 = m->t->lineas[12];
	Linea* l2 = m->t->lineas[l1->goal];
	while(l1->largo > 1)
	{
		retroceder_avanzar_lineas(m, l1, l2);
	}
	//int i = 0;
	//char r = solver_alargar_conection0(m);
	//while(r != 'F')
	//{
		//r = solver_alargar_conection0(m);
	//}
	//m->stats = update_stats(m->t, m->l, m->stats);
	//print_stats(m->stats);

	//deal_with_dead_lines(m);
	//fprintf(stderr, "----------------------------------\n" );
	//print_linea(m->t->lineas[12]);
	//limpiar_linea(m,m->t->lineas[12]);
	//limpiar_linea(m,m->t->lineas[13]);
	//limpiar_linea(m,m->t->lineas[38]);
	//limpiar_linea(m,m->t->lineas[35]);

	//fprintf(stderr, "---------------Limpiada----------------\n" );
	////print_linea(m->t->lineas[13]);
	////reset_goal_linea(m->t, m->t->lineas[13]);
	////fprintf(stderr, "---------------Set goal----------------\n" );
	////print_linea(m->t->lineas[13]);
	////conectar_linea(m,m->t->lineas[13]);

	//reset_goal_linea(m->t, m->t->lineas[35]);
	//fprintf(stderr, "---------------Set goal----------------\n" );
	//print_linea(m->t->lineas[35]);
	//conectar_linea(m,m->t->lineas[35]);

	////conectar_linea(m,m->t->lineas[3]);

	//print_linea(m->t->lineas[3]);
	//print_linea(m->t->lineas[3]);
	//calcular_opciones_linea(m,m->t->lineas[0]);
	//reset_goal_linea(m->t,m->t->lineas[0]);
	//print_linea(m->t->lineas[0]);
	//conectar_linea(m,m->t->lineas[0]);
	//set_lines_goals(m->t);
	//tejer(m);

	//conectar_linea(m,m->t->lineas[2]);
	/* TODO IMPRIMIR DECISIONES */
	//print_solucion_output_parcial(m->s);
	//print_solucion_desde_nodo(prueba->actual);
	print_solucion_final_desde_telar(m->t);
	/* Indicamos al watcher y al judge que ya terminamos */	
	printf("END\n");

	/* Liberamos memoria */
	city_layout_destroy(layout);

	return 0;
}
