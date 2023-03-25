/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

/**
 * Estructura para la lista enlazada.
 * Almacena un puntero apuntando al nodo cabeza de la cola y otro
 * apuntando al nodo cola de la misma.
*/
typedef struct Sch_List {
    struct Sch_Node *Head;
    struct Sch_Node *Tail;
} Sch_List;
