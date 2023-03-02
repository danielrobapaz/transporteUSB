/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finamore
 * Valeria Vera Herrera
*/

#include "Sch_List.h"

/**
 * Estructura para almacenar servicios
*/
typedef struct Service {
    char *Route;
    struct Sch_List Sch_List;
} Service;
