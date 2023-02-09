/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Hash_Node.h"
#include "Hash_List.h"
#include "Hash_Table.h"
#include "User.h"

#ifndef __HASH_TABLE_FUNCTIONS_H_
#define __HASH_TABLE_FUNCTIONS_H_

int hash_function(char *handle);
void add_elem(Hash_Table *table, User *user);
int is_in_hash_table(Hash_Table *table, char *handle);
User hash_search(Hash_Table *table, char *handle);
void hash_table_init(Hash_Table *table);

#endif