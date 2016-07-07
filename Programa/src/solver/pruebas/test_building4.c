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
	int z1 = 11;
	int z2 = 6;
	int b1 = 0;
	int b2 = 0;
	layout->zones[z1]->buildings[b1]->color = 1;
	layout->zones[z2]->buildings[b2]->color = 1;
	Posicion* a = create_posicion(z1,b1,layout->zones[z1]->x,layout->zones[z1]->y);
	Posicion* b = create_posicion(z2,b2,layout->zones[z2]->x,layout->zones[z2]->y);
	double dir = direccion_desde(a,b);
	int r = building4_get_building(dir, dist_x(a,b));
	fprintf(stderr,"building a tomar entre (%d,%d) y (%d,%d) es: %d\n",layout->zones[z1]->x,layout->zones[z1]->y,layout->zones[z2]->x,layout->zones[z2]->y,r);
	//print_telar(m->t);

	/* TODO IMPRIMIR DECISIONES */

	/* Indicamos al watcher y al judge que ya terminamos */	
	printf("END\n");

	/* Liberamos memoria */
	city_layout_destroy(layout);
	destroy_master(m);
	return 0;
}
