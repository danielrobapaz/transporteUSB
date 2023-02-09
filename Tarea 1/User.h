/**
* CI3825: Sistemas de Operación
* Daniel Robayo
* Santiago Finnamore
* Valeria Vera Herrera 16-11233
*/

typedef struct User {
    char *Username;
    int UserId;
    struct LinkedListTweet *Tweets;
    struct LinkedListFollowing *Following;
}
User;