/**
 * @file ecs.h
 * @brief Implementación de un sistema Entity-Component-System (ECS)
 * @author Desmon (David)
 * @date 2023
 * @license Apache License, Version 2.0 with Modification
*/

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

#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "debug_c.h"

/**
 * @brief Tipo de dato para representar una entidad
 */
typedef void* Entity;

/**
 * @brief ECS = Entidad-Componente-Sistema. Macro entidad
 * 
 * @code
Entity(Enemy)* createEnemy(uint32_t position) {
     Entity(Enemy)* enemy = (Entity(Enemy)*)malloc(sizeof(Entity(Enemy)));
     // codigo para crear una entidad enemigo
}
 * @endcode
 */
#define Entity(name)    name ##_Entity

/**
 * @brief ECS = Entidad-Componente-Sistema. Macro Sistema
 * 
 * @code
void System(Render)(void* data) {
     // codigo del sistema de "Render"
}
 * @endcode
*/
#define System(name)    name ##_System

/**
 * @brief ECS = Entidad-Componente-Sistema. Macro Componente
*/
#define Component(name) name ##_Component

/**
 * @brief   Macro para crear una estructura de entidad. 
 *          uint16_t  final_: todo sistema tiene un miembro final(debe estar siempre en
 *          el final de la estructura) que permite saber el final de los componentes
 *          de una entidad y el valor siempre debe ser 0xffff.
 * @param name Nombre de la entidad
 * @param ... Componentes de la entidad
 * 
 * @code
build_entity( Enemy,    // nombre de la entidad
    type_data Render;   // Componente para renderizar al enemigo
    type_data position; // Componente para la posición del enemigo
    type_data Spawn;    // Componente para el sistema de spawn del enemigo
    type_data Health;   // Componente para la salud del enemigo
);
 * @endcode
*/
#define build_entity(name, ...) typedef struct Entity(name) {       \
                                __VA_ARGS__                         \
                                    uint16_t  final_;               \
                                } Entity(name)


/**
 * @brief Macro para relacionar una entidad con un sistema. ES (Entidad-Sistema)
 * @param now_entity Entidad actual
 * @param now_system Sistema a relacionar
*/
#define relate_es(now_entity, now_system)                           \
    now_entity->now_system.type_data = System_func;                 \
    now_entity->now_system.data.System_func = System(now_system)

/**
 * @brief Macro para inicializar una entidad. Las entidades deben finalizar 
 *          con un 0xffff que indica el final de los componentes en la entidad.
 * @param name Nombre de la entidad
 * @param ... Inicialización de los componentes
 * 
 * @code

    // Relaciona los sistemas con los componentes de la entidad Enemy.
    // los enemigos tienen un sistema de renderizado, de salud y un sistema de spawing
     
    make_entity(enemy,
        relate_es(enemy, Render); // Relaciona el sistema Render con el componente correspondiente
        relate_es(enemy, Health); // Relaciona el sistema Health con el componente correspondiente
        relate_es(enemy, Spawn);  // Relaciona el sistema Spawn con el componente correspondiente
        // Aquí se pueden definir más componentes específicos de Enemy 
    );
    @endcode
*/
#define make_entity(name, ...) __VA_ARGS__ name->final_ = 0xffff

/**
 * @brief Macro para llamar a un sistema de un ECS. 
 * @param ecs Entidad-Componente-Sistema
 * @param system Sistema a llamar
 * 
 * @code
   // Crea una entidad Enemy y con la posición 0x1234
    Entity(Enemy)* enemig = createEnemy(0x1234);
    dump_entidad(enemig, 4);
    // Llama al sistema Render asociado a la entidad Enemy 
    call_system(enemig, -> Render);
   
    Component_t* entidad = createEntity((Entity *)&entidad, 2);
    if (entidad == NULL) {
        printf("Entidad es NULL\n");
        return 0;
    }
    assign_component(entidad, 0, create_data_component(System_func, System(Render)));
    assign_component(entidad, 1, create_data_component(Number,      0xabcdef12));
   
   // Llama al sistema Render asociado a la entidad con los datos de componente.
   // [0] es la posición del componente a llamar en la entidad
   call_system(entidad, [0]);
 * @endcode
*/
#define call_system(ecs, system) ((void (*)(void*))(ecs system.data.System_func))(ecs);

/**
 * @enum type_data_enum
 * @brief Enumeración de los tipos de datos de los componentes
*/
typedef enum type_data_enum {
        System_func,    /**< Función de sistema        */
        Number,         /**< Número entero             */
        Floating,       /**< Número de punto flotante  */
        String,         /**< Cadena de caracteres      */
        Object,         /**< Objeto                    */   
        Unknowned         /**< Tipo desconocido. se usa para indicar cualquier estado
                        que no sea los anterior mencionados*/
} type_data_enum;

/**
 * @union data_component
 * @brief Unión que almacena los diferentes tipos de datos de los componentes
*/
typedef union data_component
{
    void*    System_func;   /**< System_func*/
    uint64_t      Number;   /**< Number*/
    long double Floating;   /**< Floating*/
    const char*   String;   /**< String*/
    void*         Object;   /**< Objects */
    void*      Unknowned;    /**< Unknowned*/
} data_component;

/**
 * @struct type_data
 * @brief Estructura que representa un componente
*/
typedef struct type_data {
    type_data_enum type_data;   /**< Tipo de dato del componente (ver @ref type_data_enum). */
    data_component data;        /**< Datos del componente (ver @ref data_component) */
} type_data, Component_t;

/**
 * @struct advanced_data
 * @brief Estructura para manejar datos avanzados de componentes. Tipo de dato para cada componente en una entidad
*/
typedef struct advanced_data {
    Component_t     *ptr;   /**< Puntero a un componente o varios (ver @ref Component_t). */
    size_t        offset;   /**< offset del dato */
} advanced_data;

#ifdef DEBUG_ECS
#define debug_ecs(...) printf(""  __VA_ARGS__)
#else
#define debug_ecs(...) 
#endif

/**
 * @brief Macro para crear un componente de datos
 * @param type_data_ Tipo de dato del componente
 * @param data_ Datos del componente
*/
#define create_data_component(type_data_, data_) type_data_, (data_component){.type_data_ = data_}

/**
 * @brief Asigna un componente a una entidad
 * @param name Nombre de la entidad (ver @ref Component_t)
 * @param position Posición del componente
 * @param type_data_ Tipo de dato del componente (ver @ref type_data_enum)
 * @param data_ Datos del componente (ver @ref data_component)
 * @note Asegúrese de que `position` esté dentro de los límites del arreglo.
 * @warning No utilizar `position` fuera del rango válido, ya que puede causar acceso a memoria no definida.
 * 
 * @code
   Component_t* entidad = createEntity((Entity *)&entidad, 2);
   if (entidad == NULL) {
          printf("Entidad es NULL\n");
          return 0;
   }
   // Asigna valores a los dos componentes iniciales de la entidad genérica
   assign_component(entidad, 0, create_data_component(System_func, System(Render)));
   assign_component(entidad, 1, create_data_component(Number,      0xabcdef12));
 * @endcode
*/
static inline void assign_component(
        Component_t* name, 
        size_t position, 
        type_data_enum type_data_, 
        data_component data_
    ) {
    name[position] = (Component_t){
        .type_data = type_data_,
        .data = data_
    };
}

/**
 * @brief Obtiene los datos de un componente
 * @param data Puntero a la estructura advanced_data que contiene la información del componente
 * @return Estructura Component_t con los datos del componente
*/
Component_t get_type_data(advanced_data* data);

/**
 * @brief Imprime en hexadecimal los datos de una entidad
 * @param self Puntero a la entidad
 * @param number_data Número de componentes de la entidad
*/
void dump_entidad(void* self, uint16_t number_data);

/**
 * @brief Crea una nueva entidad
 * @warning Todas las entidades del mismo tipo deben seguir el mismo orden de datos.
 * @param self Entity** Puntero doble donde se almacenará la entidad creada
 * @param number_data Número de componentes que tendrá la entidad
 * @return Puntero a la entidad creada, o NULL en caso de error
*/
Entity createEntity(Entity* self, uint16_t number_data);

/**
 * @brief Redimensiona una entidad existente
 * @param self Entity* Puntero doble donde se almacenará la entidad creada
 * @param old_number_data Número actual de componentes de la entidad
 * @param new_number_data Nuevo número de componentes deseado
 * @return Entity Puntero a la entidad redimensionada o NULL si falla la reasignación de memoria.
 * @code
   Entity myEntity = createEntity(&myEntity, 2); // entidad de 2 componentes
   if (myEntity == NULL) {
        printf("Entidad es NULL\n");
        return 0;
   }
   reallocEntity(&entidad, 2, 5); // Redimensiona a 5 componentes
 * @endcode
*/
Entity reallocEntity(Entity* self, size_t old_number_data, size_t new_number_data);

/**
 * @brief Libera la memoria de una entidad y sus componentes
 * @param entity Puntero a la entidad que se desea liberar
 * @code
   Entity myEntity = createEntity(&myEntity, 5);
   freeEntity(myEntity);
   myEntity = NULL; // Para evitar dangling pointers
 * @endcode
 */
void freeEntity(void* entity);

/**
 * @brief Macro para reportar errores en el sistema ECS
 * Esta macro proporciona una forma consistente de manejar y reportar errores en el sistema ECS.
 * Dependiendo de la definición de USE_ASSERT_ECS, utilizará assert o un bloque condicional personalizado.
 * @param condition La condición que se debe cumplir. Si es falsa, se considera un error.
 * @param code_exit Código que se ejecutará si la condición es falsa (solo en modo no assert).
 * @param ... Mensaje de error formateado y sus argumentos (solo en modo no assert).
 * @note Si USE_ASSERT_ECS está definido, se utilizará la función assert de la biblioteca estándar.
 *      En caso contrario, se imprimirá un mensaje de error personalizado con información del archivo y línea.
 * @code
        report_error_ecs(
            ptr != NULL,                        // si el puntero es nulo
                return NULL;                    // retorna NULL en caso de error
            , "Error: puntero nulo detectado\n" // mensaje de error personalizado
        );
 * @endcode
*/
#ifndef report_error_ecs
    #ifdef USE_ASSERT_ECS
    #define report_error_ecs(condition, code_exit, ...)                       \
        assert(condition);                                                    \
        code_exit
    #else
    #define report_error_ecs(condition, code_exit, ...)                       \
        if (!(condition)) {                                                   \
            fprintf(stderr, "[File: %s, Line: %d] ", __FILE__, __LINE__);     \
            fprintf(stderr, __VA_ARGS__);                                     \
            code_exit                                                         \
        }
    #endif
#endif


#endif /* ECS_H*/
