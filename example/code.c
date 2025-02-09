/*
 *	Licencia Apache, Versión 2.0 con Modificación
 *	
 *	Copyright 2023 Desmon (David)
 *	
 *	Se concede permiso, de forma gratuita, a cualquier persona que obtenga una copia de 
 *	este software y archivos de documentación asociados (el "Software"), para tratar el 
 *	Software sin restricciones, incluidos, entre otros, los derechos de uso, copia, 
 *	modificación, fusión, publicación, distribución, sublicencia y/o venta de copias del 
 *	Software, y para permitir a las personas a quienes se les proporcione el Software 
 *	hacer lo mismo, sujeto a las siguientes condiciones:
 *	
 *	El anterior aviso de copyright y este aviso de permiso se incluirán en todas las 
 *	copias o partes sustanciales del Software.
 *	
 *	EL SOFTWARE SE PROPORCIONA "TAL CUAL", SIN GARANTÍA DE NINGÚN TIPO, EXPRESA O 
 *	IMPLÍCITA, INCLUYENDO PERO NO LIMITADO A LAS GARANTÍAS DE COMERCIABILIDAD, IDONEIDAD 
 *	PARA UN PROPÓSITO PARTICULAR Y NO INFRACCIÓN. EN NINGÚN CASO LOS TITULARES DEL 
 *	COPYRIGHT O LOS TITULARES DE LOS DERECHOS DE AUTOR SERÁN RESPONSABLES DE NINGÚN 
 *	RECLAMO, DAÑO U OTRA RESPONSABILIDAD, YA SEA EN UNA ACCIÓN DE CONTRATO, AGRAVIO O DE 
 *	OTRA MANERA, QUE SURJA DE, FUERA DE O EN CONEXIÓN CON EL SOFTWARE O EL USO U OTRO TIPO
 *	DE ACCIONES EN EL SOFTWARE.
 *	
 *	Además, cualquier modificación realizada por terceros se considerará propiedad del 
 *	titular original de los derechos de autor. Los titulares de derechos de autor 
 *	originales no se responsabilizan de las modificaciones realizadas por terceros.
 *	
 *	Queda explícitamente establecido que no es obligatorio especificar ni notificar los 
 *	cambios realizados entre versiones, ni revelar porciones específicas de código 
 *	modificado.
 */

#include "ecs.h"

build_entity( Bullet,
    type_data Render;
    type_data position;
    type_data Spawn;
);

build_entity( Player,
    type_data Render;
    type_data position;
    type_data Health;
);

build_entity( Enemy,
    type_data Render;
    type_data position;
    type_data Spawn;
    type_data Health;
);

/*
 * sistemas del ECS
 */
void System(Render)(void* data) {
    printf("RenderSystem\n");

    advanzed_data data_init =  {
        .offset = 0,
        .ptr = data
    };

    Component_t dato = get_type_data(&data_init);
}
void System(Spawn)(void* data) {
    printf("SpwaningSystem\n");
}
void System(Health)(void* data) {
    printf("HealthSystem\n");
}

/*
 * funcion para crear una entidad enemigo
 */
Entity(Enemy)* createEnemy(uint32_t position) {
    Entity(Enemy)* enemy            = (Entity(Enemy)*)malloc(sizeof(Entity(Enemy)));

    /* los enemigos tienen un sistema de renderizado, de salud y un sistema de spawing */
    make_entity(enemy,
        relate_es(enemy, Render);
        relate_es(enemy, Health);
        relate_es(enemy, Spawn);
        /* aqui definir mas componentes de la entidad enemigo */
    );
    enemy->position.type_data    = Number;
    enemy->position.data.Number = position;

    return enemy;
}

int main(int argc, char *argv[]) {
    __constructor_debug_c__();
    Entity(Enemy)* enemig = createEnemy(0x1234);
    dump_entidad(enemig, 4);

    /* llamar a el systema Render */
    call_system(enemig, -> Render);
    
    /*
     * crear una entidad generica de forma dinamica con 2 componentes
     */
    Component_t* entidad = NULL;
    entidad = createEntity((Entity *)&entidad, 2);
    printf("entidad %p size de tipos %zubytes\n", entidad, sizeof(type_data));
    
    if (entidad == NULL) {
        printf("componentes es NULL\n");
        return 0;
    }

    printf("Memoria de la entidad inicia en : %p\n", entidad);
    printf("Memoria de la entidad finaliza en : %p\n", entidad  + sizeof(Component_t)*2);
    assign_component(entidad, 0, create_data_component(System_func, System(Render)));
    assign_component(entidad, 1, create_data_component(Number,      0xabcdef12));

    dump_entidad(entidad, 2);

    // apartir de aqui, para añadir mas valores a la entidad, es necesario redimensionarla:
    puts("Reasignando la memoria de la entidad");
    reallocEntity(&entidad, 2, 4);
    printf("Memoria de la entidad inicia en : %p\n", entidad);
    printf("Memoria de la entidad finaliza en : %p\n", entidad  + sizeof(Component_t)*4);
    assign_component(entidad, 2, create_data_component(Floating,    12.2324));
    assign_component(entidad, 3, create_data_component(String,      "hola mundo"));

    dump_entidad(entidad, 4);

    /* llamar a un componente asignado dinamicamente:*/
    puts("llamando a un systema de un compoponente asignado dinamicamente:");
    call_system(entidad, [0]);

    printf("entidad[3].data.String      -> %s\n",  entidad[3].data.String);
    printf("entidad[2].data.Floating    -> %Lf\n", entidad[2].data.Floating);
    printf("entidad[1].data.Number      -> %x\n",  entidad[1].data.Number);
    printf("entidad[0].data.System_func -> %p\n",  entidad[0].data.System_func);


    Component_t* entidad2 = NULL;
    entidad2 = createEntity((Entity *)&entidad2, 11);
    entidad2 = reallocEntity((Entity *)&entidad2, 11, 500);
    //entidad2 = reallocEntity((Entity *)&entidad2, 50, 60);
    printf("entidad2 = %p\n", entidad2);
    for (int i = 12; i-1 < 50; i++) {
        //reallocEntity((Entity *)&entidad2, i - 1, i);
        /*los valores estallan por ser el limitador 0xffffffffffffffff*/
        assign_component(entidad2,  i-1, create_data_component(Number, 0xffffffffffffffff));
        printf("entidad2[%d].data.Number      -> %llx\n", i-1, entidad2[i-1].data.Number);
        dump_entidad(entidad2,  i-1);
    }

puts("Adios");
    return 0;
}