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
	//solve_puzzle(m, 'l');
	print_telar(m->t);
	Linea* l1 = m->t->lineas[16];
	Linea* l2 = m->t->lineas[17];
	conectar_linea(m,l1);
	if(m->t->lineas[l2->goal]->isReady == 'T' || m->t->lineas[l2->goal]->dead == 'T')
	{
		reset_goal_linea(m->t,l2);
	}
	conectar_linea(m,l2);
	print_telar(m->t);

	//tejer(m);

	/* TODO IMPRIMIR DECISIONES */

	//print_solucion_output(m->s);
	/* Indicamos al watcher y al judge que ya terminamos */	
	printf("END\n");

	/* Liberamos memoria */
	city_layout_destroy(layout);

	return 0;
}
