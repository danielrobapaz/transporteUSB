/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef __USER_LIST_FUNCTIONS_H_
#define __USER_LIST_FUNCTIONS_H_

void User_List_init(User_List *list);
void add_User_Node(User_List *list, struct User *user);
int is_in_User_List(User_List *list, char *handle);

#endif