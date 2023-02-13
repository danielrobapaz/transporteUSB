/**
 * CI3825: Sistemas de Operación
 * Daniel Robayo
 * Santiago Finnamore
 * Valeria Vera Herrera
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Tweets_List_Functions.h"
#include "Hash_Table_Functions.h"
#include "User_List_Functions.h"

#define MAXINPUTSIZE 40
#define MAXUSERSIZE 100
#define MAXPASSWRDSIZE 100
#define MAXTWTSIZE 100
#define MAXPROMPTSIZE 6
#define MAXTWEETSIZE 282

void login_signup_prompt(char *user, char *pwd);
int login_verify(char *user, char *pwd, Hash_Table *table);
int signin_verify(char *user, char *pwd, Hash_Table *table);
void add_to_table(char *user, char *pwd, Hash_Table *table);
void show_user_feed(User *user, Tweets_List *list);
void show_user_twts(char *user, Hash_Table *table);
int user_verify(char *user, Hash_Table *table);
void add_tweet(User *user, Tweets_List *twt_list);
void follow_user(User *user, Hash_Table *table);
Tweet *create_tweet(char *username);
void print_tweet(Tweet *Tweet);

int main() {
    char *input, *user, *pswd, *twt, *prompt;
    int flag;
    Tweets_List *Tweet_List = CreateTweetList();
    User *logged_user;
    Hash_Table Users_Table;
    hash_table_init(&Users_Table);
    
    input = malloc(MAXINPUTSIZE);
    user = malloc(MAXUSERSIZE);
    pswd = malloc(MAXPASSWRDSIZE);
    twt = malloc(MAXTWTSIZE);
    prompt = malloc(MAXPROMPTSIZE);

    do {
        printf("DON'T MISS WHAT'S HAPPENING! LOGIN, SIGNUP OR LEAVE: ");
        scanf("%s", input);
        
        flag = 0;

        if (!strcmp(input, "login") || !strcmp(input, "LOGIN")) {
            login_signup_prompt(user, pswd);

            /* verify that the user exists and the password is correct */
            if (login_verify(user, pswd, &Users_Table) == 0) {
                /* Obtener User de la tabla de hash */
                logged_user = hash_search(&Users_Table, user);

                do {
                    system("clear");
                    show_user_feed(logged_user, Tweet_List);
                    printf("\nWHAT'S HAPPENING? (+, @ or logout): ");

                    /* reads prompt from user */
                    scanf("%s", prompt);
                    fflush(stdout);

                    /* verify if tweet is user, text or logout*/
                    if (!strcmp(prompt, "+")) {
                        add_tweet(logged_user, Tweet_List);
                        
                    } else if (!strcmp(prompt, "@")) {
                        follow_user(logged_user, &Users_Table);

                    } else if (strcmp(prompt, "logout") == 0 || strcmp(prompt, "LOGOUT") == 0) {
                        system("clear");
                        printf("Logout, come back soon.\n");
                    } else {
                        system("clear");
                        printf("Invalid option\n");
                    }
                } while (strcmp(prompt, "logout") != 0 && strcmp(prompt, "LOGOUT") != 0);
            }
        } else if (strcmp(input, "signup") == 0 || strcmp(input, "SIGNUP") == 0) {
            login_signup_prompt(user, pswd);
            system("clear");
            /* verify that the user doesn't exist */
            if (signin_verify(user, pswd, &Users_Table) == 0) {
                /* create a new hash entry */
                add_to_table(user, pswd, &Users_Table);

                printf("Successfuly signed up!\n");
            } else {
                printf("Username already in use. Try again\n");
            }

        } else if (strcmp(input, "leave") == 0 || strcmp(input, "LEAVE") == 0) {
            flag = 1;

            /* free heap */
            free(input);
            free(user);
            free(pswd);
            free(twt);

        }  else {
            system("clear");
            printf("Invalid option, try again.\n");
        }
    } while(flag == 0);

    return 0;
}

/* shows prompt for user and password and reads it from standard input*/
void login_signup_prompt(char *user, char *pwd) {
    printf("USERNAME: ");
    fflush(stdout);
    scanf("%s", user);
    printf("PASSWORD: ");
    fflush(stdout);
    scanf("%s", pwd);
}

/* returns 0 if user and password is correct for login */
int login_verify(char *user, char *pwd, Hash_Table *table) {
    /* Check if user exists on hash table */
    if (is_in_hash_table(table, user)) {
        /* Check if password matches*/
        User *User = hash_search(table, user);
        if (!strcmp(pwd, User->Password)) {
            return 0;
        } else {
            printf("Incorrect password. Try again\n");
            return 1;
        }
    } else {
        printf("User does not exist. Try again\n");
        return 1;
    }
    return 0;
}

/* returns 1 if the ingested user handle already exists and 0 otherwise */
int signin_verify(char *user, char *pwd, Hash_Table *table) {
    /*Check if user does not exist on hash table*/
    if (is_in_hash_table(table, user)) {
        return 1;
    }
    return 0;
}

void add_to_table(char *user, char *pwd, Hash_Table *table) {
    User *new_user = malloc(sizeof(User));
    Tweets_List *user_tweets = CreateTweetList();
    User_List *user_following = Create_User_List();

    if (!user) {
        exit(1);
    }

    new_user->Handle = malloc(strlen(user)+1);
    new_user->Password = malloc(strlen(pwd)+1);
    strcpy(new_user->Handle, user);
    strcpy(new_user->Password, pwd); /* todo hashing */
    new_user->Tweets = user_tweets;

    add_User_Node(user_following, new_user);

    new_user->Following = user_following;

    add_elem(table, new_user);
}

/* shows user feed */
void show_user_feed(User *user, Tweets_List *list) {
    printf("Timeline of @%s.\n", user->Handle);
    printf("\n------------------------\n");

    Tweet_Node *curr_node_twt_list;
    User_Node *curr_node_follow_list;
    User_List *user_follow_list;
    char *username;

    user_follow_list = user->Following;
    curr_node_twt_list = list->Tail;

    /* find every tweet such that tweet.username is in user.followgin */
    while (curr_node_twt_list != NULL) {
        /* username of the curr twt */
        username = curr_node_twt_list->Tweet->Username;

        curr_node_follow_list = user_follow_list->Head;
        while (curr_node_follow_list != NULL) {
            if (strcmp(username, curr_node_follow_list->User->Handle) == 0) {
                printf("\n@%s: \"%s\"\n",
                curr_node_twt_list->Tweet->Username, curr_node_twt_list->Tweet->Tweet);
                printf("(%s)", curr_node_twt_list->Tweet->TimeStamp);
                printf("\n------------------------\n");
            }
            curr_node_follow_list = curr_node_follow_list->Next;
        }

        curr_node_twt_list = curr_node_twt_list->Prev;    
    }
}

/* shows user twts */
void show_user_twts(char *user, Hash_Table *table) {
    /* buscamos el usuario */
    User *aux_user;
    aux_user = hash_search(table, user);

    printf("Feed of @%s", user);

    /* mostramos la lisa de usuario*/
    PrintTweetList(aux_user->Tweets);
}

/* returns 1 if user exist in hash table */
int user_verify(char *user, Hash_Table *table) {
    /* checks if user is in hash table */
    return is_in_hash_table(table, user);
}


/*  add a new tweet to user tweet list.

    args: *user -> user to add a new tweet in its list
          *twt_list -> list that stores every twt.

    returns:
        none
*/
void add_tweet(User *user, Tweets_List *twt_list) {
    Tweet *new_tweet;
    Tweet_Node *new_tweet_node_global;
    Tweet_Node *new_tweet_node_user;

    printf("New Tweet: ");

    new_tweet = create_tweet(user->Handle);
    new_tweet_node_global = CreateTweetNode(new_tweet);
    new_tweet_node_user = CreateTweetNode(new_tweet);

    InsertTweetNode(new_tweet_node_global, twt_list);
    InsertTweetNode(new_tweet_node_user, user->Tweets);
}

/*  add a new follower to user.following list

    args:
        *user -> user to add a new following user.
        *table -> table that stores every user

    returns:
        none

*/
void follow_user(User *user, Hash_Table *table) {
    char *user_to_find, *option;
    int flag;

    user_to_find = malloc(MAXUSERSIZE);
    option = malloc(MAXINPUTSIZE);

    printf("User: ");
    fflush(stdout);
    scanf("%s", user_to_find);

    /* verify that user exist */
    if (user_verify(user_to_find, table) == 1) {
        system("clear");

        show_user_twts(user_to_find, table);

        do {
            printf("follow or leave: ");
            fflush(stdout);
            scanf("%s", option);

            flag = 0;

            if (strcmp(option, "follow") == 0 || strcmp(option, "FOLLOW") == 0) {
                add_User_Node(user->Following, hash_search(table, user_to_find));
                printf("Now you follow @%s!\n", user_to_find);
                flag = 1;

            } else if (strcmp(option, "leave") == 0 || strcmp(option, "LEAVE") == 0) {
                printf("Returning to timeline.\n");
                flag = 1;

            } else {
                printf("Invalid option.\n");
            }
        } while(flag != 1);
    } else {
        printf("User @%s doesn\'t exist\n", user_to_find);
    }

    free(user_to_find);
    free(option);
}

/**
 * Función que pide al usuario un input
 * para crear un tweet. El mismo se almacena
 * junto con el username del usuario que lo escribió
 * y la hora y fecha de creación.
*/
Tweet *create_tweet(char *username) {
    Tweet *newTweet = malloc(sizeof(struct Tweet));

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char *timestamp = asctime(tm);

    char *content = malloc(MAXTWEETSIZE);

    if (!newTweet) {
        exit(1);
    }

    fgetc(stdin);

    fgets(content, MAXTWEETSIZE, stdin);

    /* Deletes \n from the end of strings */
    content[strlen(content) - 1] = 0;
    timestamp[strlen(timestamp) - 1] = 0;

    newTweet->Username = username;
    newTweet->Tweet = content;
    newTweet->TimeStamp = malloc(strlen(timestamp)+1);
    strcpy(newTweet->TimeStamp, timestamp);

    return newTweet;
}


/**
 * Función para imprimir tweets.
*/
void print_tweet(Tweet *tweet) {
    printf("\n@%s: \"%s\"\n", tweet->Username, tweet->Tweet);
    printf("(%s)\n", tweet->TimeStamp);
}