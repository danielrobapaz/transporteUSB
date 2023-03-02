/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Service_List.h"
#include "Service.h"
#include "Sch_List.h"

#ifndef __SERVICE_LIST_FUNCTIONS_H_
#define __SERVICE_LIST_FUNCTIONS_H_

void Service_List_init(Service_List *list);
Service_List *Create_Service_List();
Service *create_Service(char *route, Sch_List *sch);
void add_Service_Node(Service_List *list, Service *Service);

#endif