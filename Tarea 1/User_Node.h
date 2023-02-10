/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

/**
 * Estructura nodo para la lista enlazada de la tabla de Hash.
 * Almacena un puntero a un objeto de tipo usuario y otro puntero
 * que apunta a algun otro nodo de la lista de la que forma parte.
*/
typedef struct User_Node {
    struct User *User;
    struct User_Node *Next;
} User_Node;