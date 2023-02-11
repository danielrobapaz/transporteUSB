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

void login_signup_prompt(char *s1, char *s2);
int login_verify(char *s1, char *s2, Hash_Table *table);
int signin_verify(char *s1, char *s2, Hash_Table *table);
void add_to_table(char *s1, char *s2, Hash_Table *table);
void show_user_feed(char *s1, Hash_Table *table, Tweets_List *list);
void show_user_twts(char *s1, Hash_Table *table);
int user_verify(char *s1, Hash_Table *table);

/**
 * Función que pide al usuario un input
 * para crear un tweet. El mismo se almacena
 * junto con el username del usuario que lo escribió
 * y la hora y fecha de creación.
*/
Tweet *CreateTweet(char *username) {
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
    newTweet->TimeStamp = timestamp;

    return newTweet;
}

/**
 * Función para imprimir tweets.
*/
void PrintTweet(struct Tweet *Tweet) {
    printf("\n@%s: \"%s\"\n", Tweet->Username, Tweet->Tweet);
    printf("(%s)\n", Tweet->TimeStamp);
}

int main() {
    char *input, *user, *pswd, *twt, *prompt;
    int flag;
    int j;
    Tweets_List *Tweet_List = CreateTweetList();
    User *logged_user, *aux_user;
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
                    show_user_feed(user, &Users_Table, Tweet_List);
                    printf("\nWHAT'S HAPPENING? (+, @ or logout): ");

                    /* reads prompt from user */
                    scanf("%s", prompt);
                    fflush(stdin);

                    /* verify if tweet is user, text or logout*/
                    if (!strcmp(prompt, "+")) {
                        Tweet *New_Tweet;
                        Tweet_Node *New_Tweet_Node;
                        
                        printf("New Tweet: ");
            
                        New_Tweet = CreateTweet("logged_user->Handle");
                        New_Tweet_Node = CreateTweetNode(New_Tweet);

                        /* add tweet to global twt-list */
                        InsertTweetNode(New_Tweet_Node, Tweet_List);

                        /* add tweet to user twt-list */
                        InsertTweetNode(New_Tweet_Node, logged_user->Tweets);

                        PrintTweetList(logged_user->Tweets);
                    } else if (!strcmp(prompt, "@")) {
                        /* ask user */
                        printf("User: ");
                        fflush(stdout);
                        scanf("%s", user);

                        /* verify that user exist*/
                        if (user_verify(user, &Users_Table) == 1) {
                            show_user_twts(user, &Users_Table);

                            /* wait input from user */
                            do {
                                printf("follow or leave: ");
                                fflush(stdout);
                                scanf("%s", input);

                                if (strcmp(input, "follow") == 0 || strcmp(input, "FOLLOW") == 0) {
                                    /* add user to following list */
                                    aux_user = hash_search(&Users_Table, user);
                                    add_User_Node(logged_user->Following, aux_user);

                                    printf("Now you follow @%s", input);
                                } else if (strcmp(input, "leave") == 0 || strcmp(input, "LEAVE") == 0) {
                                    printf("Returning to timeline\n");
                                } else {
                                    printf("Invalid option.\n");
                                }
                            } while(strcmp(input, "leave") != 0 && strcmp(input, "LEAVE") != 0 && strcmp(input, "follow") != 0 && strcmp(input, "FOLLOW") != 0);

                        } else {
                            printf("User @%s doesn\'t exist", input);
                        }
                    } else if (strcmp(prompt, "logout") == 0 || strcmp(prompt, "LOGOUT") == 0) {
                        printf("logout\n");
                    } else {
                        printf("prompt invalido\n");
                    }
                } while (strcmp(prompt, "logout") != 0 && strcmp(prompt, "LOGOUT") != 0);
            }
        } else if (strcmp(input, "signup") == 0 || strcmp(input, "SIGNUP") == 0) {
            login_signup_prompt(user, pswd);
            
            /* verify that the user doesn't exist */
            if (signin_verify(user, pswd, &Users_Table) == 0) {
                printf("Successfuly signed up!\n");
                /* create a new hash entry */
                add_to_table(user, pswd, &Users_Table);
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
    strcpy(new_user->Password, pwd); /* TO DO HASHING */
    new_user->Tweets = user_tweets;
    new_user->Following = user_following;

    add_elem(table, new_user);
}

/* shows user feed */
void show_user_feed(char *user, Hash_Table *table, Tweets_List *list) {
    User *aux_user;
    Tweet_Node *curr_node_twt_list;
    User_Node *curr_node_follow_list;
    User_List *user_follow_list;
    char *username;

    aux_user = hash_search(table, user);
    user_follow_list = aux_user->Following;
    curr_node_twt_list = list->Tail;

    /* find every tweet such that tweet.username is in user.followgin */
    while (curr_node_twt_list != NULL) {
        /* user of the curr twt */
        username = curr_node_twt_list->Tweet->Username;

        curr_node_follow_list = user_follow_list->Tail; 
        while (curr_node_follow_list != NULL) {
            if (strcmp(username, curr_node_follow_list->User->Handle) == 0 ||  strcmp(username, user) == 0) {
                printf("\n@%s: \"%s\"\n",
                curr_node_twt_list->Tweet->Username, curr_node_twt_list->Tweet->Tweet);
                printf("(%s)", curr_node_twt_list->Tweet->TimeStamp);
                printf("\n------------------------\n");
            }
            curr_node_follow_list = curr_node_follow_list->Next;
        }

        curr_node_twt_list = curr_node_twt_list->Next;    
    }
}

/* shows user twts */
void show_user_twts(char *user, Hash_Table *table) {
    /* buscamos el usuario */
    User *aux_user;
    aux_user = hash_search(table, user);

    /* mostramos la lisa de usuario*/
    PrintTweetList(aux_user->Tweets);
}

/* returns 1 if user exist in hash table */
int user_verify(char *user, Hash_Table *table) {
    /* checks if user is in hash table */
    return is_in_hash_table(table, user);
}