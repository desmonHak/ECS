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
#ifndef ECS_C
#define ECS_C

#include "ecs.h"

Component_t get_type_data(advanzed_data* data) {
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(Component_t, get_type_data)
            TYPE_DATA_DBG(advanzed_data*, "data = %p")
        END_TYPE_FUNC_DBG,
        data);
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
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(void, dump_entidad)
            TYPE_DATA_DBG(void*, "self = %p")
            TYPE_DATA_DBG(uint16_t, "number_data = %p")
        END_TYPE_FUNC_DBG,
        self, number_data);
    size_t i;
    for (i = 1; i-1 < (sizeof(type_data) * number_data) + sizeof(size_t); i++) {
        printf("0x%02x%s", *((uint8_t*)self + (i-1)), (!((i) % 8)) ? "\n" : " ");
    }
    printf("\n");
}
Entity* createEntity(Entity** self, uint16_t number_data){
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(Entity*, createEntity)
            TYPE_DATA_DBG(Entity**, "self = %p")
            TYPE_DATA_DBG(uint16_t, "number_data = %p")
        END_TYPE_FUNC_DBG,
        self, number_data);
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
    
    Entity *_self = *self;
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

Entity* reallocEntity(Entity** self, size_t old_number_data, size_t new_number_data){
    DEBUG_PRINT(DEBUG_LEVEL_INFO,
        INIT_TYPE_FUNC_DBG(Entity*, reallocEntity)
            TYPE_DATA_DBG(Entity**, "self = %p")
            TYPE_DATA_DBG(size_t, "old_number_data = %zu")
            TYPE_DATA_DBG(size_t, "new_number_data = %zu")
        END_TYPE_FUNC_DBG,
        self, old_number_data, new_number_data);
    /*
     * retornar una estructura con campos nulos implica que o hubo un error al hacer realloc, o no
     * se redimensiono el tamaño
     */
    if (self == NULL) return NULL;

    if (old_number_data < new_number_data) {
        unsigned char* _self = (unsigned char*)realloc(*self, sizeof(Component_t) * new_number_data);
        report_error_ecs(
            _self != NULL,  // si esto no ocurrio
                return NULL; // codigo que ejecutar
            , "Error: no se pudo reasignar la memoria debido a que realloc fallo\n"
        );
        DEBUG_PRINT(DEBUG_LEVEL_INFO, "Datos redimnsionado a %zu Bytes\n", new_number_data);

        // poner los valores desconocidos unicamente en la nueva memoria redimensionaada
        memset(_self + sizeof(Component_t) * old_number_data, 
            Unkownod, sizeof(Component_t) * (new_number_data - old_number_data));

        // eliminar el antiguo final de la entidad
        *(uint16_t*)((void*)_self + ((sizeof(Component_t) * old_number_data) )) = 0x0000;

        // indicar el nuevo final de la entidad
        *(uint16_t*)((void*)_self + ((sizeof(Component_t) * new_number_data) )) = 0xffff;
    } else {
        report_error_ecs(
            false,  // si es false, se solicita que ocurra este raise
                return NULL; // codigo que ejecutar
            , "Error: no se pudo reasignar la memoria, debido a que el nuevo size es menor al inicial\n"
        );
    }
}

#endif