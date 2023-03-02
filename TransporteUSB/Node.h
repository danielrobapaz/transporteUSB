/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

/**
 * Estructura nodo para la lista enlazada de servicios.
*/
typedef struct Node {
    struct Service *Service;
    struct Node *Next;
} Node;