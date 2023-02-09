/**
* CI3825: Sistemas de Operación
* Daniel Robayo
* Santiago Finnamore
* Valeria Vera Herrera 16-11233
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "UserNode.h"
#include "User.h"
#include "LinkedListFollowing.h"

LinkedListFollowing *CreateFollowingList() {
    LinkedListFollowing *List = malloc(sizeof(struct LinkedListFollowing));

    if (!List) {
        return NULL;
    }

    List->Head = NULL;

    return List;
}

UserNode *CreateUserNode(struct User *user) {
    UserNode *newNode = malloc(sizeof(struct UserNode));

    if (!newNode) {
        exit(1);
    }

    newNode->User = user;
    newNode->Next = NULL;

    return newNode;
}

void InsertUserNode(struct UserNode *Node, struct LinkedListFollowing *List) {
    if (List->Head == NULL) {
        List->Head = Node;
        List->Tail = Node;
        List->Head->Next = NULL;
    } else {
        List->Tail->Next = Node;
        List->Tail = Node;
        List->Tail->Next = NULL;
    }
}

void PrintUserList(struct LinkedListFollowing *List) {
    UserNode *CurrentNode = List->Head;

    while (CurrentNode != NULL) {
        printf("(%s): %s", CurrentNode->User->Username, CurrentNode->User->Username);
    }
	printf("\n");
}