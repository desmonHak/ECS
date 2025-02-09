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

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define System(name) name ##System_f

typedef void* RenderSystem;
typedef void* HealthSystem;
typedef void* SpwaningSystem;
typedef void* Entity;

typedef enum type_data_enum {
        System_func,
        uint8_t_,
        uint16_t_,
        uint32_t_,
        uint64_t_,
        Unknowned    /* se usa para indicar cualquier estado
                        que no sea los anterior mencionados*/
} type_data_enum;

typedef struct type_data {
    type_data_enum type_data;
    union data
    {
        uint8_t  ui8;
        uint16_t ui16;
        uint32_t ui32;
        uint64_t ui64;
        void*    func;
    } data;
} type_data, System;

typedef struct Bullet {
    type_data Render;
    type_data position;
    type_data Spawn;
    uint16_t final_;
} Bullet;

typedef struct Player {
    type_data Render;
    type_data position;
    type_data Health;
    uint16_t final_;
} Player;

typedef struct Enemy {
    type_data Render;
    type_data position;
    type_data Spawn;
    type_data Health;
    uint16_t final_;
} Enemy;

typedef struct advanced_data {
    type_data *ptr;
    size_t     offset;
} advanced_data;

type_data get_type_data(advanced_data* data) {
    type_data data_ret = {
        .data = 0,
        .type_data = (type_data_enum){0}
    };
    puts("get_type_data");
    void* ptr = NULL;
    for (
        ptr = data->ptr; 
        *((uint16_t*)(ptr + data->offset)) != 0xffff; 
        data->offset += sizeof(type_data)
    ) {
        type_data* data_get = (type_data*)(ptr + data->offset);
        switch (data_get->type_data)
        {
        case System_func: 
            data_ret = (type_data){.data = data_get->data, .type_data = data_get->type_data};
            printf("System_func %p\n", data_get->data.func); 
            break;
        case uint8_t_:    
            data_ret = (type_data){.data = data_get->data, .type_data = data_get->type_data};
            printf("uint8_t %hhx\n", data_get->data.ui8); 
            break;
        case uint16_t_:   
            data_ret = (type_data){.data = data_get->data, .type_data = data_get->type_data};
            printf("uint16_t %hx\n", data_get->data.ui16); 
            break;
        case uint32_t_:   
            data_ret = (type_data){.data = data_get->data, .type_data = data_get->type_data};
            printf("uint32_t %x\n", data_get->data.ui32); 
            break;
        case uint64_t_:   
            data_ret = (type_data){.data = data_get->data, .type_data = data_get->type_data};
            printf("uint64_t %llx\n", data_get->data.ui64); 
            break;
        default:          
            /* en caso de un miembro ser 0xffffffffffffffff se finaliza el programa*/
            if (data_get->data.func == (void*)-1) return (type_data){.data = 0, .type_data = 0};
            printf("desconocido %p\n", data_get->data.func); 
            break;
        }
    }
    return data_ret;
}

void System(Render)(void* data) {
    printf("RenderSystem\n");

    
    advanced_data data_init =  {
        .offset = 0,
        .ptr = data
    };
    type_data dato = get_type_data(&data_init);
}
void System(Spwaning)(void* data) {
    printf("SpwaningSystem\n");
}
void System(Health)(void* data) {
    printf("HealthSystem\n");
}

Enemy* createEnemy(uint32_t position) {
    Enemy* enemy            = (Enemy*)malloc(sizeof(Enemy));
    enemy->Render.type_data = System_func;
    enemy->Render.data.func = System(Render);
    enemy->Health.type_data = System_func;
    enemy->Health.data.func = System(Health);
    enemy->Spawn.type_data  = System_func;
    enemy->Spawn.data.func  = System(Spwaning);

    enemy->position.type_data = uint32_t_;
    enemy->position.data.ui32 = position;

    enemy->final_ = 0xffff;

    return enemy;
}

Entity createEntity(Entity* self, uint16_t number_data){
    /*
     * Crea una entidad, 
     * 
     * Se espera que self, sea una referencia donde almacenar 
     * la entidad creada si no lo fue ya.
     * 
     * Se espera que number_data sea el numero de datos que va a almacenar la entidad.
     * 
     * Importante: todas las entidades del mismo tipo deben darse el orden de datos
     * de la misma manera. En caso de que mismas entidades tengan los datos en distintos
     * lugares, no se podan situar de forma correcta al 100%.
     * 
    */
    if (self == NULL) return NULL;
    
    Entity _self = *self;
    if (_self == NULL) {
        _self = malloc((sizeof(type_data) * number_data) + sizeof(size_t));
        if (_self == NULL) {
            _self = NULL;
            return _self;
        }
        memset(_self, Unknowned, sizeof(type_data) * number_data + sizeof(size_t));
        *(size_t*)((void*)_self + ((sizeof(type_data) * number_data) + sizeof(size_t))) = -1;
    }

    return _self;
}

void dump_entidad(void* self, uint16_t number_data) {
    size_t i;
    for (i = 0; i < (sizeof(type_data) * number_data) + sizeof(size_t); i++) {
        printf("0x%02x%s", *((uint8_t*)self + i), (!(i % 8)) ? "\n" : " ");
    }
    putchar('\n');
}

int main(int argc, char* argv[]) {
    Enemy* enemig = createEnemy(0x1234);
    dump_entidad(enemig, 4);
    ((void (*)(void*))(enemig->Render.data.func))(enemig);

    type_data* entidad = NULL;
    entidad = createEntity((Entity *)&entidad, 3);
    printf("entidad %p size de tipos %zubytes\n", entidad, sizeof(type_data));
    
    if (entidad == NULL) {
        printf("entidad es NULL\n");
        return 0;
    }
    entidad[0] = (type_data){
            .type_data = System_func, 
            .data = {
                .func = System(Render)
        }
    };
    entidad[1] = (type_data){
        .type_data = uint32_t_, 
        .data = {
            .ui32 = 0xabcdef12
        }
    };

    entidad[2] = (type_data){
        .type_data = uint16_t_, 
        .data = {
            .ui16 = 0x1234
        }
    };
    dump_entidad(entidad, 3);
    ((void (*)(void*))(entidad[0].data.func))(entidad);

    puts("Adios");
    return 0;
}