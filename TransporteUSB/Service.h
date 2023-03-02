/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#include "Schedule.h"

/**
 * Estructura para almacenar servicios
*/
typedef struct Service {
    char *Route;
    struct Schedule sch1;
    struct Schedule sch2;
    struct Schedule sch3;
    struct Schedule sch4;
    struct Schedule sch5;
    struct Schedule sch6;
    struct Schedule sch7;
    struct Schedule sch8;
}
Service;
