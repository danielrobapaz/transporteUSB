#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXINPUTSIZE 6
#define MAXUSERSIZE 100
#define MAXPASSWRDSIZE 100
#define MAXTWTSIZE 100

void login_signup_prompt(char *s1, char *s2);
int login_verify(char *s1, char *s2);
int signin_verify(char *s1, char *s2);
void show_user_feed(char *s1);
void show_user_twts(char *s1);
int user_verify(char *s1);

int main() {
    char *input, *user, *pswd, *twt;
    int flag;
    
    input = malloc(MAXINPUTSIZE);
    user = malloc(MAXUSERSIZE);
    pswd = malloc(MAXPASSWRDSIZE);
    twt = malloc(MAXTWTSIZE);

    do {
        printf("DON'T MISS WHAT'S HAPPENING! LOGIN, SIGNUP OR LEAVE\n");
        scanf("%s", input);
        
        flag = 0;

        if (strcmp(input, "login") == 0 || strcmp(input, "LOGIN") == 0) {
            login_signup_prompt(user, pswd);

            /* verify that the user exists and the password is correct */
            if (login_verify(user, pswd) == 0) {
                /* show user's feed */
                show_user_feed(user);
                printf("WHAT'S HAPPENING?");
                
                /* reads tweet from user */
                scanf("%s", twt);

                /* verify if tweet is user, text or logout*/
                if (twt[0] == '+') {
                    printf("new twt\n");
                    /* add tweet  to user twt-list*/

                } else if (twt[0] == '@') {
                    printf("user\n");
                    /* go to user */
                    /* trunc first char of twt  */
                    /* verify that user exist*/
                    if (user_verify(user)) {
                        show_user_twts(user);
                    }
                } else if (strcmp(twt, "logout") == 0 || strcmp(twt, "LOGOUT") == 0) {
                    printf("logout");
                }
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
    printf("feed\n");
}

/* shows user twts */
void show_user_twts(char *s1) {
    printf("user twts\n");
}

/* returns 0 if user exist in hash table */
int user_verify(char *s1) {
    return 0;
}