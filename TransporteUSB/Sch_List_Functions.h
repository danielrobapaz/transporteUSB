/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Sch_List.h"
#include "Schedule.h"

#ifndef __SCH_LIST_FUNCTIONS_H_
#define __SCH_LIST_FUNCTIONS_H_

void Sch_List_init(Sch_List *list);
Sch_List *Create_Sch_List();
Schedule *create_Schedule(int hour, int min, int cap);
void add_Sch_Node(Sch_List *list, Schedule *sch);

#endif