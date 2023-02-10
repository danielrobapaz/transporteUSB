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
#include "Hash_List_Functions.h"

#define MAXINPUTSIZE 6
#define MAXUSERSIZE 100
#define MAXPASSWRDSIZE 100
#define MAXTWTSIZE 100
#define MAXPROMPTSIZE 6
#define MAXTWEETSIZE 282

void login_signup_prompt(char *s1, char *s2);
int login_verify(char *s1, char *s2, Hash_Table table);
int signin_verify(char *s1, char *s2, Hash_Table table);
void add_to_table(char *s1, char *s2, Hash_Table table);
void show_user_feed(char *s1);
void show_user_twts(char *s1);
int user_verify(char *s1);

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
    Tweets_List *TweetList = CreateTweetList();
    Hash_Table UsersTable;
    hash_table_init(&UsersTable);
    
    input = malloc(MAXINPUTSIZE);
    user = malloc(MAXUSERSIZE);
    pswd = malloc(MAXPASSWRDSIZE);
    twt = malloc(MAXTWTSIZE);
    prompt = malloc(MAXPROMPTSIZE);

    do {
        printf("DON'T MISS WHAT'S HAPPENING! LOGIN, SIGNUP OR LEAVE: ");
        scanf("%s", input);
        
        flag = 0;

 if (strcmp(input, "login") == 0 || strcmp(input, "LOGIN") == 0) {
            login_signup_prompt(user, pswd);
            
            system("clear");

            /* verify that the user exists and the password is correct */
            if (login_verify(user, pswd, UsersTable) == 0) {
                /* Obtener user de la tabla de hash */
                User *currentUser = hash_search(&UsersTable, user);

                /* Crear lista enlazada para los tweets del usuario loggeado */
                Tweets_List *UserList = CreateTweetList();
                currentUser->Tweets = UserList;

                /* show user's feed */
                do {
                    system("clear");
                    show_user_feed(user);
                    printf("\nWHAT'S HAPPENING? (+, @ or logout): ");
                    /* reads prompt from user */
                    scanf("%s", prompt);
                    fflush(stdin);

                    /* verify if tweet is user, text or logout*/
                    if (prompt[0] == '+') {
                        Tweet *NewTweet;
                        Tweet_Node *NewTweetNode;
                        
                        printf("New Tweet: ");
                        system("clear");
            
                        NewTweet = CreateTweet(currentUser->Handle);
                        NewTweetNode = CreateTweetNode(NewTweet);

                        /* add tweet to global twt-list */
                        InsertTweetNode(NewTweetNode, TweetList);

                        /* add tweet to user twt-list */
                        InsertTweetNode(NewTweetNode, UserList);

                    } else if (prompt[0] == '@') {
                        printf("user\n");
                        /* go to user */
                        /* trunc first char of twt  */
                        /* verify that user exist*/
                        if (user_verify(user) == 0) {
                            show_user_twts(user);
                            scanf("%s", input);

                            if (strcmp(input, "follow") == 0 || strcmp(input, "FOLLOW") == 0) {
                                printf("follow\n");
                            }
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
            if (signin_verify(user, pswd, UsersTable) == 0) {
                printf("Successfuly signed in!\n");
                /* create a new hash entry */
                add_to_table(user, pswd, UsersTable);
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

/* shows prompt for user and passwrd and reads it from standard input*/
void login_signup_prompt(char *s1, char *s2) {
    printf("USERNAME: ");
    scanf("%s", s1);
    printf("PASSWORD: ");
    scanf("%s", s2);
}

/* returns 0 if user and password is correct for login */
int login_verify(char *s1, char *s2, Hash_Table table) {
    /* Check if user exists on hash table */
    /*if (is_in_hash_table(&table, s1)) {
        User *user = hash_search(&table, s1);
        if (strcmp(s2, user->Password) == 0) {
            return 0;
        } else {
            printf("Incorrect password. Try again\n");
            return 1;
        }
    } else {
        printf("User does not exist. Try again\n");
        return 1;
    }*/
    return 0;
}

/* returns 0 is user and passwrd is correct for signin */
int signin_verify(char *s1, char *s2, Hash_Table table) {
    /*Check if user does not exist on hash table*/
    if (is_in_hash_table(&table, s1)) {
        return 1;
    }

    return 0;
}

void add_to_table(char *s1, char *s2, Hash_Table table) {
    /* add user to hash table */
    User *user = malloc(sizeof(User));

    if (!user) {
        exit(1);
    }

    user->Handle = s1;
    user->Password = s2; /* TO DO HASHING */
    user->Tweets = NULL;
    user->Following = NULL;

    add_elem(&table, user);
}

/* shows user feed */
void show_user_feed(char *s1) {
    printf("TO DO: feed\n");
    
    /* Ubicar a User en la tabla de hash */

    /* Imprimir tweets de lista de tweets global
        dependiendo de los usuarios que siga s1
    */
}

/* shows user twts */
void show_user_twts(char *s1) {
    printf("user twts\n");
}

/* returns 0 if user exist in hash table */
int user_verify(char *s1) {
    return 0;
}