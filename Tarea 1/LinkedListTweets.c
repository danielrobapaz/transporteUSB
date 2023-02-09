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

LinkedListTweet *CreateTweetList() {
    LinkedListTweet *List = malloc(sizeof(struct LinkedListTweet));

    if (!List) {
        return NULL;
    }

    List->Head = NULL;

    return List;
}

TweetNode *CreateTweetNode(struct Tweet *tweet) {
    TweetNode *newNode = malloc(sizeof(struct TweetNode));

    if (!newNode) {
        exit(1);
    }

    newNode->Tweet = tweet;
    newNode->Next = NULL;

    return newNode;
}

void InsertTweetNode(struct TweetNode *Node, struct LinkedListTweet *List) {
    TweetNode * temp = List->Tail;

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

void PrintTweetList(struct LinkedListTweet *List) {
    TweetNode *CurrentNode = List->Tail;

    while (CurrentNode != NULL) {
        printf("\n@%s: \"%s\"\n",
            CurrentNode->Tweet->Username, CurrentNode->Tweet->Tweet);
        printf("(%s)", CurrentNode->Tweet->TimeStamp);
        printf("\n------------------------\n");
        CurrentNode = CurrentNode->Prev;
    }
	printf("\n");
}