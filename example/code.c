/**
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

// Definición de entidades mediante la macro `build_entity`
// Cada entidad tiene un conjunto de componentes que definen su comportamiento y estado
build_entity( Bullet,    // nombre de la entidad
    type_data Render;    // Componente para renderizar la entidad
    type_data position;  // Componente para la posición de la entidad
    type_data Spawn;
);

build_entity( Player,   // nombre de la entidad
    type_data Render;   // Componente para renderizar al jugador
    type_data position; // Componente para la posición del jugador
    type_data Health;   // Componente para la salud del jugador
);

build_entity( Enemy,    // nombre de la entidad
    type_data Render;   // Componente para renderizar al enemigo
    type_data position; // Componente para la posición del enemigo
    type_data Spawn;    // Componente para el sistema de spawn del enemigo
    type_data Health;   // Componente para la salud del enemigo
);

/*
 * Sistemas del ECS: funciones que operan sobre los componentes de las entidades.
 */
// Sistema que maneja el componente Render
void System(Render)(void* data) {
    printf("RenderSystem\n");

    // Inicializa los datos avanzados necesarios para procesar los componentes
    advanced_data data_init =  {
        .offset = 0,
        .ptr = data
    };

    // Obtiene los datos del componente y los procesa
    Component_t dato = get_type_data(&data_init);
}

// Sistema que maneja el componente Spawn
void System(Spawn)(void* data) {
    printf("SpwaningSystem\n");
}

// Sistema que maneja el componente Health
void System(Health)(void* data) {
    printf("HealthSystem\n");
}

/*
 * Función para crear una entidad Enemy.
 */
Entity(Enemy)* createEnemy(uint32_t position) {
    Entity(Enemy)* enemy            = (Entity(Enemy)*)malloc(sizeof(Entity(Enemy)));

     
    /* 
     * Relaciona los sistemas con los componentes de la entidad Enemy.
     * los enemigos tienen un sistema de renderizado, de salud y un sistema de spawing
     */
    make_entity(enemy,
        relate_es(enemy, Render); // Relaciona el sistema Render con el componente correspondiente
        relate_es(enemy, Health); // Relaciona el sistema Health con el componente correspondiente
        relate_es(enemy, Spawn);  // Relaciona el sistema Spawn con el componente correspondiente
        /* Aquí se pueden definir más componentes específicos de Enemy */
    );

    // Inicializa el componente position con un valor numérico dado por `position`
    enemy->position.type_data    = Number;
    enemy->position.data.Number = position;

    // Devuelve un puntero a la entidad creada
    return enemy;
}

int main(int argc, char *argv[]) {
    // Inicializa herramientas de depuración
    __constructor_debug_c__();

    // Crea una entidad Enemy y con la posición 0x1234
    Entity(Enemy)* enemig = createEnemy(0x1234);
    dump_entidad(enemig, 4);

    /* Llama al sistema Render asociado a la entidad Enemy */
    call_system(enemig, -> Render);
    
    /*
     * Crea una entidad genérica dinámica con 2 componentes.
     */
    Component_t* entidad = NULL;
    entidad = createEntity((Entity *)&entidad, 2);
    printf("entidad %p size de tipos %zubytes\n", entidad, sizeof(type_data));
    
    if (entidad == NULL) {
        printf("Entidad es NULL\n");
        return 0;
    }

    printf("Memoria de la entidad inicia en : %p\n", entidad);
    printf("Memoria de la entidad finaliza en : %p\n", entidad  + sizeof(Component_t)*2);

    // Asigna valores a los dos componentes iniciales de la entidad genérica
    assign_component(entidad, 0, create_data_component(System_func, System(Render)));
    assign_component(entidad, 1, create_data_component(Number,      0xabcdef12));

    dump_entidad(entidad, 2);

    /*
     * Redimensiona dinámicamente la memoria de la entidad y asigna nuevos valores.
     * apartir de aqui, para añadir mas componentes a la entidad, es necesario redimensionarla:
     */
    puts("Reasignando la memoria de la entidad");
    reallocEntity(&entidad, 2, 5); // Redimensiona a 5 componentes

    printf("Memoria de la entidad inicia en : %p\n", entidad);
    printf("Memoria de la entidad finaliza en : %p\n", entidad  + sizeof(Component_t)*4);

    /* Asigna a esta entidad un puntero a otra entidad (enemigo creado previamente) */
    assign_component(entidad, 2, create_data_component(Floating,    12.2324));
    assign_component(entidad, 3, create_data_component(String,      "hola mundo"));
    assign_component(entidad, 4, create_data_component(Object,      enemig)); /* asiganr a esta entidad, la entidad enemigo creada */

    dump_entidad(entidad, 5);

    /* Llama al sistema asociado al primer componente asignado dinámicamente */
    puts("llamando a un systema de un compoponente asignado dinamicamente:");
    call_system(entidad, [0]);

    /* Imprime valores específicos de los componentes asignados dinámicamente */
    printf("entidad[3].data.String      -> %s\n",  entidad[3].data.String);
    printf("entidad[2].data.Floating    -> %Lf\n", entidad[2].data.Floating);
    printf("entidad[1].data.Number      -> %x\n",  entidad[1].data.Number);
    printf("entidad[0].data.System_func -> %p\n",  entidad[0].data.System_func);

    /*
     * Crea otra entidad genérica dinámica con más componentes.
     */
    Component_t* entidad2 = NULL;
    entidad2 = createEntity((Entity)&entidad2, 11);
    printf("(createEntity)entidad2 = %p\n", entidad2);
    //entidad2 = reallocEntity((Entity *)&entidad2, 11, 500);
    //entidad2 = reallocEntity((Entity *)&entidad2, 50, 60);
    printf("(reallocEntity)entidad2 = %p\n", entidad2);

     /* Redimensiona automáticamente antes de agregar nuevos componentes */
    for (int i = 12; i-1 < 50; i++) {
        // fix error: es necesario redimensionar la entidad antes de agregar componentes via reallocEntity y
        // obtener el puntero obtenido de reallocEntity, en caso de que no se haya obtenido el puntero de reallocEntity,
        // la funcion fallara.
        entidad2 = reallocEntity((Entity)&entidad2, i - 1, i); // auto redimensionar la entidad
        /* ¿ los valores estallan por ser el limitador 0xffffffffffffffff ?  (aparentemente no)*/
        assign_component(entidad2,  i-1, create_data_component(Number, 0xffffffffffffffff));
        printf("entidad2[%d].data.Number      -> %llx\n", i-1, entidad2[i-1].data.Number);
        dump_entidad(entidad2,  i-1);
    }

    /* Libera las entidades creadas dinámicamente */
    freeEntity(entidad);
    freeEntity(entidad2);

    puts("Adios");
    return 0;
}