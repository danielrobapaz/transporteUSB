/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "User.h"
#include "User_Node.h"
#include "User_List.h"
#include "Hash_Table.h"

/** Estructuras, procedimientos y funciones relevantes
 * a las listas enlazadas de la tabla de hash.
*/

/**
 * OJO: Dado que esta estructura de lista es utilizada para almacenar
 * usuarios, se utiliza también para crear la lista de "following" de
 * cada usuario.
*/

/** Inicializa la lista enlazada de la tabla de hash apuntada por el
 * apuntador list.
*/
void User_List_init(User_List *list) {
    (*list).Head = NULL;
    (*list).Tail = NULL;
}

/** Procedimiento que agrega un nodo nuevo a una lista enlazada de tipo 
 * User_List. El nodo agregado apuntará a un usuario de Handle "Handle" e
 * identificación id. Este será agregado al final de la cola.
*/
void add_User_Node(User_List *list, User *user) {

    User_Node *new_node = malloc(sizeof(User_Node));

    if (!new_node) {
        exit(1);
    }

    new_node->User = user;
    new_node->Next = NULL;

    /** Si la lista está vacía se inicializa la cabeza
    * y la cola con el nodo ingresado*/
    if ((*list).Head == NULL) {
        (*list).Head = new_node;
        (*list).Tail = new_node;
    }
    /*En caso contrario se agrega el nodo al final de la cola*/
    else {
        (*(*list).Tail).Next = new_node;
        (*list).Tail = new_node;
    }
}

/** Función que indica si el usuario de Handle "Handle"
 * está presente o no en la
 * lista User_List apuntada por list.
*/
int is_in_User_List(User_List *list, char *handle) {
    User_Node *tmp = (*list).Head;
    while (tmp != NULL) {
        if (!strcmp((*(*tmp).User).Handle, handle)) {
            return 1;
        }
        tmp = (*tmp).Next;
    }
    return 0;
}

/**
 * Función que imprime los elementos de la Hash List
*/
void print_User_List(User_List *list) {
    User_Node *tmp = (*list).Head;
    while (tmp != NULL) {
        printf("(%d): %s", (*(*tmp).User).User_Id, (*(*tmp).User).Handle);
        tmp = (*tmp).Next;
    }
    printf("\n");
}