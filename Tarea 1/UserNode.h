/**
* CI3825: Sistemas de Operación
* Daniel Robayo
* Santiago Finnamore
* Valeria Vera Herrera 16-11233
*/

/**
* Estructura de Nodo para lista enlazada de usuarios
*/
typedef struct UserNode {
    struct User *User;
    struct UserNode *Prev;
    struct UserNode *Next;
}
UserNode;

