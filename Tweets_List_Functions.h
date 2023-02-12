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

#ifndef __TWEETSLIST_H_
#define __TWEETSLIST_H_

Tweets_List *CreateTweetList();
Tweet_Node *CreateTweetNode(struct Tweet *tweet);
void InsertTweetNode(struct Tweet_Node *Node, struct Tweets_List *List);
void PrintTweetList(struct Tweets_List *List);

#endif