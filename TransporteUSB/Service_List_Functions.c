/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Service.h"
#include "Service_Node.h"
#include "Service_List.h"

/** Estructuras, procedimientos y funciones
 * relevantes a las Service_listas enlazadas de struct Service.
*/

/** Inicializa la Service_lista de Service apuntada por el
 * apuntador Service_list.
*/
void Service_List_init(struct Service_List *Service_list) {
    (*Service_list).Head = NULL;
    (*Service_list).Tail = NULL;
}

/**
 * Función para crear e inicializar una Service_lista enlazada.
*/
Service_List *Create_Service_List() {
    Service_List *Service_List = malloc(sizeof(struct Service_List));

    if (!Service_List) {
        return NULL;
    }

    Service_List_init(Service_List);

    return Service_List;
}

/** Procedimiento que agrega un nodo nuevo a una Service_lista enlazada de tipo 
 * Service_List. El nodo agregado apuntará a un usuario de Handle "Handle" e
 * identificación id. Este será agregado al final de la cola.
*/
void add_Service_Node(Service_List *Service_list, Service *Service) {

    Service_Node *new_node = malloc(sizeof(Service_Node));

    if (!new_node) {
        exit(1);
    }

    new_node->Service = Service;
    new_node->Next = NULL;

    /** Si la Service_lista está vacía se inicializa la cabeza
    * y la cola con el nodo ingresado*/
    if ((*Service_list).Head == NULL) {
        (*Service_list).Head = new_node;
        (*Service_list).Tail = new_node;
        (*(*Service_list).Head).Next = NULL;
    }
    /*En caso contrario se agrega el nodo al final de la cola*/
    else {
        (*(*Service_list).Tail).Next = new_node;
        (*Service_list).Tail = new_node;
        (*(*Service_list).Tail).Next = NULL;
    }
}