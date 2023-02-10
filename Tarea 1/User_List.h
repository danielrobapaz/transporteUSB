/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

/**
 * Estructura para la lista enlazada de la tabla de hash.
 * Almacena un puntero apuntando al nodo cabeza de la cola y otro
 * apuntando al nodo cola de la misma.
*/
typedef struct User_List {
    struct User_Node *Head;
    struct User_Node *Tail;
} User_List;
