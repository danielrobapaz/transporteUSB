/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finamore
 * Valeria Vera Herrera
*/

/**
 * Estructura para almacenar servicios
*/
typedef struct Service {
    char *Route;
    struct Sch_List *Schedule;
} Service;
