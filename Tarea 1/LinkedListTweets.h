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

#ifndef __LINKEDLISTTWEETS_H__
#define __LINKEDLISTTWEETS_H__

LinkedListTweet *CreateTweetList();
TweetNode *CreateTweetNode(struct Tweet *tweet);
void InsertTweetNode(struct TweetNode *Node, struct LinkedListTweet *List);
void PrintTweetList(struct LinkedListTweet *List);


#endif