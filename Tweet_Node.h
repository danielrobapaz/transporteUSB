/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera 16-11233
*/

/**
 * Estructura nodo para tweets.
 * Almacena un puntero a un struct tweet y otros dos
 * punteros al nodo anterior y posterior respectivamente.
*/
typedef struct Tweet_Node {
    struct Tweet *Tweet;
    struct Tweet_Node *Prev;
    struct Tweet_Node *Next;
}
Tweet_Node;

