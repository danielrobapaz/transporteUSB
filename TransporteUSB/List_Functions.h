/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "List.h"
#include "Service.h"

#ifndef __LIST_FUNCTIONS_H_
#define __LIST_FUNCTIONS_H_

void List_init(List *list);
List *Create_List();
void add_Node(List *list, Service *Service);

#endif