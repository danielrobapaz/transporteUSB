#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedListTweets.h"
#include "LinkedListUsers.h"

#define MAXINPUTSIZE 6
#define MAXUSERSIZE 100
#define MAXPASSWRDSIZE 100
#define MAXTWTSIZE 100
#define MAXPROMPTSIZE 6
#define MAXTWEETSIZE 282

void login_signup_prompt(char *s1, char *s2);
int login_verify(char *s1, char *s2);
int signin_verify(char *s1, char *s2);
void show_user_feed(char *s1);
void show_user_twts(char *s1);
int user_verify(char *s1);

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

void PrintTweet(struct Tweet *Tweet) {
    printf("\n@%s: \"%s\"\n", Tweet->Username, Tweet->Tweet);
    printf("(%s)\n", Tweet->TimeStamp);
}

int main() {
    char *input, *user, *pswd, *twt, *prompt;
    int flag;
    LinkedListTweet *TweetList = CreateTweetList();
    
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

            /* verify that the user exists and the password is correct */
            if (login_verify(user, pswd) == 0) {
                /* show user's feed */
                do {
                    show_user_feed(user);
                    printf("\nWHAT'S HAPPENING? (+, @ or logout): ");
                
                    /* reads prompt from user */
                    scanf("%s", prompt);

                    /* verify if tweet is user, text or logout*/
                    if (prompt[0] == '+') {
                        Tweet *NewTweet;
                        TweetNode *NewTweetNode;

                        printf("New Tweet: ");

                        NewTweet = CreateTweet("LOGGED_USER_69");
                        NewTweetNode = CreateTweetNode(NewTweet);

                        /* add tweet to global twt-list */
                        InsertTweetNode(NewTweetNode, TweetList);

                        /* add tweet to user twt-list */
                        /* TO DO */

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
            if (signin_verify(user, pswd) == 0) {
                printf("signin succesfull\n");
                /* create a new hash entry */
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

/* returns 0 if user and passwrd is correct for login */
int login_verify(char *s1, char *s2) {
    return 0;
}

/* returns 0 is user and passwrd is correct for signin */
int signin_verify(char *s1, char *s2) {
    return 0;
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