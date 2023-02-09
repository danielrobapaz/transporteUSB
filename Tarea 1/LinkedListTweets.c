/**
* CI3825: Sistemas de Operación
* Daniel Robayo
* Santiago Finnamore
* Valeria Vera Herrera 16-11233
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TweetNode.h"
#include "Tweet.h"
#include "LinkedListTweet.h"

/*
* Retorno: Pointer a nueva lista enlazada
* con una cabeza no nula de largo 0.
*/
LinkedListTweet *CreateTweetList() {
    LinkedListTweet *List = malloc(sizeof(struct LinkedListTweet));
    TweetNode *Head = malloc(sizeof(struct TweetNode));

    if (!List) {
        return NULL;
    }
    if (!Head) {
        return NULL;
    }

    List->Head = Head;

    return List;
}

void CreateTweet(char *username, char *content, time_t* timestamp) {
    Tweet *newTweet = malloc(sizeof(struct Tweet));

    if (!newTweet) {
        exit(1);
    }

    newTweet->Username = username;
    newTweet->Tweet = content;
    newTweet->TimeStamp = timestamp;
}

void createNoteTweet(struct Tweet *tweet) {
    TweetNode *newNode = malloc(sizeof(struct TweetNode));

    if (!newNode) {
        exit(1);
    }

    newNode->Tweet = tweet;
    newNode->Next = NULL;
}

/*
* Entrada: Pointer a lista enlazada y a nodo a insertar en la misma.
* Los nodos son agregados utilizando ordenamiento lineal
* ascendente dependiendo del elemento "length" de cada uno.
*/
void InsertTweetNode(struct TweetNode *Node, struct LinkedListTweet *List) {
    if (List->Head == NULL) {
        List->Head = Node;
    } else {
        List->Tail->Next = Node;
        List->Tail = Node;
    }
}

/*
* Entrada: Pointer a lista enlazada
*/
void PrintTweetList(struct LinkedListTweet *List) {
	TweetNode *Temp = List->Head;

	do {
		printf("%s: \"%s\"\n",Temp->Tweet->Username, Temp->Tweet->Tweet);
		Temp = Temp->Next;
	}while(Temp->Next != NULL);
	printf("\n");
}