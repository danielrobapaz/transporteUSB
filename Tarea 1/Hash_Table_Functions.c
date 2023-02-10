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
#include "User_List_Functions.h"

/* Estructuras, funciones y procedimientos relevantes a la tabla de hash..*/

/** 
 * Funcion de hash para los Handles. POR AHORA solo suma los
 * ascii de los caracteres del Handle y devuelve el resultado.
 * Cuando encuentre un hash mejor lo implemento.
*/
int hash_function(char *handle) {
    int i; /*variable auxiliar*/
    int sum = 0;

    for (i=0; i < strlen(handle); i++) {
        sum += handle[i];
    }
    return sum%20;
}

/**
 * Procedimiento que agrega un nuevo
 * elemento a la tabla de Hash.
*/
void add_elem(Hash_Table *table, User *user) {
    /*se calcula el hash value del usuario apuntado por user*/
    int hash_val = hash_function(user->Handle);
    add_User_Node(&(table->list_array)[hash_val], user); /*Se agrega el ususario a la tabla de Hash*/
}

/**
 * 
*/
int is_in_hash_table(Hash_Table *table, char *handle) {
    int hash_val = hash_function(handle);
    return (is_in_User_List(&(table->list_array[hash_val]), handle));
}

/**
 * Funcion que busca al usuario de nombre handle
 * y retorna el objeto de tipo User que le corresponde.
 * Se asume que el usuario existe y está presente en 
 * la lista apuntada por lista.
*/
/*User hash_search(Hash_Table *table, char *handle) {

    int hash_val = hash_function(handle);
    User_List *list = &(table->list_array[hash_val]);
    User_Node *tmp = (*list).Head;
    while (!strcmp((*(*tmp).User).Handle, handle)) {
        tmp = (*tmp).Next;
    }
    return (*(*tmp).User);
}*/

User *hash_search(Hash_Table *table, char *handle) {

    int hash_val = hash_function(handle);
    User_List *list = &(table->list_array[hash_val]);
    User_Node *tmp = (*list).Head;

    while (!strcmp((*(*tmp).User).Handle, handle)) {
        tmp = (*tmp).Next;
    }
    return (*tmp).User;
}

/**
 * 
*/
void hash_table_init(Hash_Table *table) {
    int i;
    for (i = 0; i < 20; i++) {
        User_List_init(&(table->list_array[i]));
    }
}

/**int main(void) {
    User usr1, usr2, usr3;
    usr1.Handle = "@realBenShapiro";
    usr2.Handle = "@elunmosk";
    usr3.Handle = "@iguaido";
    usr1.User_Id = 1;
    usr2.User_Id = 2;
    usr3.User_Id = 3;
    Hash_Table table;
    hash_table_init(&table);
    add_elem(&table, &usr1);
    add_elem(&table, &usr2);
    add_elem(&table, &usr3);

    int i;
    printf("Prueba iteración sobre tabla de hash\n");
    for (i = 0; i < 20; i++) {
        User_Node *tmp = (table.list_array[i]).Head;
        while (tmp != NULL) {
            User *usr_ptr = tmp->User;
            printf("Handle: %s\n", usr_ptr->Handle);
            printf("User_Id: %d\n", usr_ptr->User_Id);
            printf("Hash value: %d\n", hash_function(usr_ptr->Handle));
            printf("\n");
            tmp = tmp->Next;
        }
    }
    printf("Prueba de busqueda\n");
    printf("Is @iguaido in table: %d\n", is_in_hash_table(&table, "@iguaido"));
    printf("Is @bioloco in table: %d\n", is_in_hash_table(&table, "@bioloco"));
}*/