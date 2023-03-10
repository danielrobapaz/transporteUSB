/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finamore
 * Valeria Vera Herrera
*/

/**
 * Estructura nodo para la lista enlazada de horarios.
*/
typedef struct Sch_Node {
    struct Schedule *Schedule;
    struct Sch_Node *Next;
} Sch_Node;