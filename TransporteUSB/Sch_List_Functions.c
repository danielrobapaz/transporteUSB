/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Schedule.h"
#include "Sch_Node.h"
#include "Sch_List.h"

/** Estructuras, procedimientos y funciones
 * relevantes a las Sch_Listas enlazadas de struct Service.
*/

/** Inicializa la Sch_Lista de Service apuntada por el
 * apuntador Sch_List.
*/
void Sch_List_init(struct Sch_List *Sch_List) {
    (*Sch_List).Head = NULL;
    (*Sch_List).Tail = NULL;
}

/**
 * Función para crear e inicializar una Sch_Lista enlazada.
*/
Sch_List *Create_Sch_List() {
    Sch_List *Sch_List = malloc(sizeof(struct Sch_List));

    if (!Sch_List) {
        return NULL;
    }

    Sch_List_init(Sch_List);

    return Sch_List;
}

Schedule *create_Schedule(int hour, int min, int cap) {
    Schedule *new_Sch = malloc(sizeof(Schedule));

    if (!new_Sch) {
        printf("Error: No se pudo reservar memoria.\n");
        exit(1);
    }

    new_Sch->Hour = hour;
    new_Sch->Min = min;
    new_Sch->Capacity = cap;

    return new_Sch;
}

/** Procedimiento que agrega un nodo nuevo a una Sch_Lista enlazada de tipo 
 * Sch_List. El nodo agregado apuntará a un usuario de Handle "Handle" e
 * identificación id. Este será agregado al final de la cola.
*/
void add_Sch_Node(Sch_List *Sch_List, Schedule *sch) {

    struct Sch_Node *new_node = malloc(sizeof(Sch_Node));

    if (!new_node) {
        exit(1);
    }

    /*new_node->Schedule = sch;*/
    new_node->Next = NULL;

    /** Si la Sch_Lista está vacía se inicializa la cabeza
    * y la cola con el nodo ingresado*/
    /*if ((*Sch_List).Head == NULL) {
        (*Sch_List).Head = new_node;
        (*Sch_List).Tail = new_node;
        (*(*Sch_List).Head).Next = NULL;
    }*/
    /*En caso contrario se agrega el nodo al final de la cola*/
    /*else {
        (*(*Sch_List).Tail).Next = new_node;
        (*Sch_List).Tail = new_node;
        (*(*Sch_List).Tail).Next = NULL;
    }*/
}