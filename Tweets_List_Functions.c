/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera 16-11233
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Tweet_Node.h"
#include "Tweet.h"
#include "Tweets_List.h"

/**
 * Función para crear e inicializar una
 * lista enlazada de tweets. Dado que no contiene
 * elementos, la cabeza de la misma es nula.
*/
Tweets_List *CreateTweetList() {
    Tweets_List *List = malloc(sizeof(struct Tweets_List));

    if (!List) {
        return NULL;
    }

    List->Head = NULL;
    List->Tail = NULL;

    return List;
}

/**
 * Función para crear nodos de tweets. Se debe
 * ingresar un puntero a un tweet ya creado. El
 * nodo en cuestión será posteriormente almacenado
 * en una lista enlazada de tweets.
*/
Tweet_Node *CreateTweetNode(struct Tweet *tweet) {
    Tweet_Node *newNode = malloc(sizeof(struct Tweet_Node));

    if (!newNode) {
        exit(1);
    }

    newNode->Tweet = tweet;
    newNode->Next = NULL;

    return newNode;
}

/**
 * Función para insertar un nodo en una lista enlazada
 * de tweets (parámetros dados).
 * Los mismos se insertan al final de la cola.
*/
void InsertTweetNode(struct Tweet_Node *Node, struct Tweets_List *List) {
    Tweet_Node *temp = List->Tail;

    if (List->Head == NULL) {
        List->Head = Node;
        List->Tail = Node;
        List->Head->Next = NULL;
        List->Head->Prev = NULL;
    } else {
        List->Tail = Node;
        List->Tail->Prev = temp;
        List->Tail->Next = NULL;
    }
}

/**
 * Función para imprimir una lista enlazada de tweets.
*/
void PrintTweetList(struct Tweets_List *List) {
    Tweet_Node *CurrentNode = List->Tail;

    while (CurrentNode != NULL) {
        printf("\n@%s: \"%s\"\n",
            CurrentNode->Tweet->Username, CurrentNode->Tweet->Tweet);
        printf("(%s)", CurrentNode->Tweet->TimeStamp);
        printf("\n------------------------\n");
        CurrentNode = CurrentNode->Prev;
    }
	printf("\n");
}