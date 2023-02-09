/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef __HASH_LIST_FUNCTIONS_H_
#define __HASH_LIST_FUNCTIONS_H_

void Hash_List_init(Hash_List *list);
void add_Hash_Node(Hash_List *list, struct User *user);
int is_in_Hash_List(Hash_List *list, char *handle);

#endif