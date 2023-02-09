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

#ifndef __LINKEDLISTTWEETS_H__
#define __LINKEDLISTTWEETS_H__

LinkedListFollowing *CreateFollowingList();
UserNode *CreateUserNode(struct User *user);
void InsertUserNode(struct UserNode *Node, struct LinkedListFollowing *List);
void PrintUserList(struct LinkedListFollowing *List);

#endif