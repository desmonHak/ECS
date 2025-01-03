#ifndef ECS_C
#define ECS_C

#include "ecs.h"

Component_t get_type_data(advanzed_data* data) {
    type_data data_ret = {
        .data = 0,
        .type_data = (type_data_emum){0}
    };
    debug_ecs("get_type_data");
    void* ptr = NULL;
    for (
        ptr = data->ptr; 
        *((uint16_t*)(ptr + data->offset)) != 0xffff; 
        data->offset += sizeof(Component_t)
    ) {
        type_data* data_get = (Component_t*)(ptr + data->offset);
        switch (data_get->type_data)
        {
        case System_func: 
            data_ret = (type_data){.data = data_get->data, .type_data = data_get->type_data};
            debug_ecs("System_func (...)(*0x%p)(...,)\n", data_get->data.System_func); 
            break;
        case Unkownod:    
            data_ret = (type_data){.data = data_get->data, .type_data = data_get->type_data};
            debug_ecs("unkownod 0x%p\n", data_get->data.Unkownod); 
            break;
        case String:   
            data_ret = (type_data){.data = data_get->data, .type_data = data_get->type_data};
            debug_ecs("string %s\n", data_get->data.String); 
            break;
        case Floating:   
            data_ret = (type_data){.data = data_get->data, .type_data = data_get->type_data};
            debug_ecs("floating %Lf\n", data_get->data.Floating);
            break;
        case Number:   
            data_ret = (type_data){.data = data_get->data, .type_data = data_get->type_data};
            debug_ecs("number 0x%llx\n", data_get->data.Number); 
            break;
        default:          
            /* en caso de un miembro ser 0xffffffffffffffff se finaliza el programa */
            if (data_get->data.System_func == (void*)-1) return (type_data){.data = 0, .type_data = 0};
            debug_ecs("desconocido %p\n", data_get->data.System_func); 
            break;
        }
    }
    return data_ret;
}

void dump_entidad(void* self, uint16_t number_data) {
    size_t i;
    for (i = 0; i < (sizeof(type_data) * number_data) + sizeof(size_t); i++) {
        printf("0x%02x%s", *((uint8_t*)self + i), (!(i % 8)) ? "\n" : " ");
    }
    printf("\n");
}
Entity createEntity(Entity* self, uint16_t number_data){
    /*
     * Crea una entidad generica, la entidad debe convertirse en un puntero Component_t
     * el cual puede usarse como lista de valores con los componentes de la entidad.
     * 
     * Se espera que self, sea una referencia donde almacenar 
     * la entidad creada si no lo fue ya.
     * 
     * Se espera que number_data sea el numero de datos que va a lamcenar la entidad.
     * 
     * Importante: todas las entidades del mismo tipo deben darse el orden de datos
     * de la misma manera. En caso de que mismas entidades tengan los datos en distintos
     * lugares, no se podan situar de forma correcta al 100%.
     * 
     */
    if (self == NULL) return NULL;
    
    Entity _self = *self;
    if (_self == NULL) {
        _self = malloc((sizeof(Component_t) * number_data));
        if (_self == NULL) {
            _self = NULL;
            return _self;
        }
        // poner la memoria como valores desconocidos
        memset(_self, Unkownod, sizeof(Component_t) * number_data );

        // indicar el final de la entidad
        *(uint16_t*)((void*)_self + ((sizeof(Component_t) * number_data) )) = 0xffff;
    }

    return _self;
}

#endif