/**
* CI3825: Sistemas de Operación
* Daniel Robayo
* Santiago Finnamore
* Valeria Vera Herrera 16-11233
*/

/**
* Estructura de Nodo
*/
typedef struct TweetNode {
    struct Tweet *Tweet;
	struct TweetNode *Next;
}
TweetNode;