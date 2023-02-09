/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

/**
 * Estructura de lista doblemente enlazada
 * para almacenar tweets. La misma está compuesta
 * por una cabeza y una cola.
*/
typedef struct Tweets_List {
    struct Tweet_Node *Head;
    struct Tweet_Node *Tail;
}
Tweets_List;