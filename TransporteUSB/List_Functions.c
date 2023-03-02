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
#include "Node.h"
#include "List.h"

/** Estructuras, procedimientos y funciones
 * relevantes a las listas enlazadas de struct Service.
*/

/** Inicializa la lista de Service apuntada por el
 * apuntador list.
*/
void List_init(struct List *list) {
    (*list).Head = NULL;
    (*list).Tail = NULL;
}

/**
 * Función para crear e inicializar una lista enlazada.
*/
List *Create_List() {
    List *List = malloc(sizeof(struct List));

    if (!List) {
        return NULL;
    }

    List_init(List);

    return List;
}

/** Procedimiento que agrega un nodo nuevo a una lista enlazada de tipo 
 * List. El nodo agregado apuntará a un usuario de Handle "Handle" e
 * identificación id. Este será agregado al final de la cola.
*/
void add_Node(List *list, Service *Service) {

    Node *new_node = malloc(sizeof(Node));

    if (!new_node) {
        exit(1);
    }

    new_node->Service = Service;
    new_node->Next = NULL;

    /** Si la lista está vacía se inicializa la cabeza
    * y la cola con el nodo ingresado*/
    if ((*list).Head == NULL) {
        (*list).Head = new_node;
        (*list).Tail = new_node;
        (*(*list).Head).Next = NULL;
    }
    /*En caso contrario se agrega el nodo al final de la cola*/
    else {
        (*(*list).Tail).Next = new_node;
        (*list).Tail = new_node;
        (*(*list).Tail).Next = NULL;
    }
}