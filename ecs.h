#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef void* Entity;

/*
 * ECS = Entidad-Componente-Sistema
 */

#define Entity(name)    name ##_Entity
#define System(name)    name ##_System
#define Component(name) name ##_Component

/*
 * uint16_t  final_: todo sistema tiene un miembro final(debe estar siempre en
 *      el final de la estructura) que permite saber el final de los componentes
 *      de una entidad y el valor siempre debe ser 0xffff.
 */
#define build_entity(name, ...) typedef struct Entity(name) {       \
                                __VA_ARGS__                         \
                                    uint16_t  final_;               \
                                } Entity(name)

/* relacionar ES (Entidad-Sistema) */
#define relate_es(now_entity, now_system)                           \
    now_entity->now_system.type_data = System_func;                 \
    now_entity->now_system.data.System_func = System(now_system)

/*
 * Las entidades deben finalizar con un 0xffff que indica el
 * final de los componentes en la entidad.
 */
#define make_entity(name, ...) \
    __VA_ARGS__                \
    name->final_ = 0xffff

/* llamar al sistema de un ECS */
#define call_system(ecs, system) ((void (*)(void*))(ecs system.data.System_func))(ecs);

#define assign_component(name, position, type_data_, data_) \
    name[position] = (Component_t){                         \
        .type_data = type_data_,                            \
        .data = {                                           \
            .type_data_ = data_                             \
        }                                                   \
    };
typedef enum type_data_emum {
        System_func,
        Number,
        Floating,
        String,
        Unkownod    /* se usa para indicar cualquier estado
                        que no sea los anterior mencionados*/
} type_data_emum;

typedef struct type_data {
    type_data_emum type_data;
    union data
    {
        void*    System_func;   /* System_func */
        uint64_t      Number;   /* Number */
        long double Floating;   /* Floating */
        const char*   String;   /* String */
        void*       Unkownod;   /* Unkownod */
    } data;
} type_data, Component_t;

/* tipo de dato para cada componente en una entidad */
typedef struct advanzed_data {
    Component_t     *ptr;
    size_t        offset;
} advanzed_data;

#ifdef DEBUG_ECS
#define debug_ecs(...) printf(""  __VA_ARGS__)
#else
#define debug_ecs(...) 
#endif
Component_t get_type_data(advanzed_data* data);

#include "ecs.c"
#endif /* ECS_H */
