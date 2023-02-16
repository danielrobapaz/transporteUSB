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
#define MAXDESCSIZE 142

void login_signup_prompt(char *user, char *pwd);
void desc_signup_prompt(User *user);
int login_verify(char *user, char *pwd, Hash_Table *table);
int signin_verify(char *user, char *pwd, Hash_Table *table);
void add_to_table(char *user, char *pwd, char *desc, Hash_Table *table);
void show_user_feed(User *user, Tweets_List *list);
void show_user_twts(char *user, Hash_Table *table);
int user_verify(char *user, Hash_Table *table);
void add_tweet(User *user, Tweets_List *twt_list);
void follow_user(User *user, Hash_Table *table);
Tweet *create_tweet(char *username);
void print_tweet(Tweet *Tweet);
void find_user_mentions(char *user, Tweets_List *tweets);
char *hash_password(char *password);

int main() {
    char *input, *user, *pswd, *twt, *prompt, *desc_prompt;
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
    desc_prompt = malloc(MAXDESCSIZE);
    do {
        printf("DON'T MISS WHAT'S HAPPENING! LOGIN, SIGNUP OR LEAVE: ");
        scanf("%s", input);
        
        flag = 0;

        if (!strcmp(input, "login") || !strcmp(input, "LOGIN")) {
            login_signup_prompt(user, pswd);

            /* verify that the user exists and the password is correct */
            if (!login_verify(user, hash_password(pswd), &Users_Table)) {
                /* Obtener User de la tabla de hash */
                logged_user = hash_search(&Users_Table, user);

                do {  
                    system("clear");
                    show_user_feed(logged_user, Tweet_List);
                    printf("\nWHAT'S HAPPENING? (+, @, mentions or logout): ");
                    fflush(stdout);

                    /* reads prompt from user */
                    scanf("%s", prompt);

                    /* verify if tweet is user, text or logout*/
                    if (!strcmp(prompt, "+")) {
                        add_tweet(logged_user, Tweet_List);
                        
                    } else if (!strcmp(prompt, "@")) {
                        follow_user(logged_user, &Users_Table);

                    } else if (!strcmp(prompt, "mentions") || !strcmp(prompt, "MENTIONS")) { 
                        system("clear");
                        find_user_mentions(logged_user->Handle, Tweet_List);
                        /*Quick-Fix para el clear que no dejaba ver mentions, se pide al usuario que presione un boton antes de borrar*/
                        printf("Presione cualquier botón para continuar...");
                        fflush(stdout);
                        system("read -sn 1");


                    } else if (!strcmp(prompt, "logout") || !strcmp(prompt, "LOGOUT")) {
                        system("clear");
                        printf("Logout, come back soon.\n");
                    } else {
                        system("clear");
                        printf("Invalid option\n");
                    }
                } while (strcmp(prompt, "logout") && strcmp(prompt, "LOGOUT"));
            }
        } else if (!strcmp(input, "signup") || !strcmp(input, "SIGNUP")) {

            login_signup_prompt(user, pswd);
            system("clear");
            /* verify that the user doesn't exist */
            if (!signin_verify(user, pswd, &Users_Table)) {
                /*Prmopt user for description*/
                printf("Do you want to add a description to your profile? [Y/N]: ");
                fflush(stdout);
                scanf("%s", prompt);
                if (!strcmp(prompt, "y") || !strcmp(prompt, "Y")) {
                    printf("Type in your description (max. 140 char): ");
                    fflush(stdout);
                    getc(stdin); /*Eliminando \n residual en buffer de input*/
                    fgets(desc_prompt, MAXDESCSIZE - 2, stdin);
                }
                else {
                    strcpy(desc_prompt, "No description.");
                }
                /* create a new hash entry */
                add_to_table(user, pswd, desc_prompt, &Users_Table);

                printf("Successfuly signed up!\n");
            } else {
                printf("Username already in use. Try again\n");
            }

        } else if (!strcmp(input, "leave") || !strcmp(input, "LEAVE")) {
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
    } while(!flag);

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

/*Creo que esta funcion ya no hace falta pero la dejo porsia*/
void desc_signup_prompt(User *user) {
    char *user_desc = malloc(MAXDESCSIZE);

    printf("DESCRIPTION (140 char. max): ");

    if (!user_desc) {
        exit(1);
    }

    fgetc(stdin);
    fgets(user_desc, MAXDESCSIZE, stdin);
    user_desc[strlen(user_desc) - 1] = 0;

    user->Desc = malloc(strlen(user_desc) + 1);
    strcpy(user->Desc, user_desc);
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

void add_to_table(char *user, char *pwd, char *desc, Hash_Table *table) {
    User *new_user = malloc(sizeof(User));
    Tweets_List *user_tweets = CreateTweetList();
    User_List *user_following = Create_User_List();

    if (!user) {
        exit(1);
    }

    new_user->Handle = malloc(strlen(user)+1);
    new_user->Password = malloc(strlen(pwd)+1);
    new_user->Desc = malloc(MAXDESCSIZE);
    strcpy(new_user->Handle, user);
    strcpy(new_user->Password, hash_password(pwd));
    strcpy(new_user->Desc, desc);
    new_user->Tweets = user_tweets;

    add_User_Node(user_following, new_user);

    new_user->Following = user_following;

    add_elem(table, new_user);
}

/* shows user feed */
void show_user_feed(User *user, Tweets_List *list) {
    Tweet_Node *curr_node_twt_list;
    User_Node *curr_node_follow_list;
    User_List *user_follow_list;
    char *username;

    printf("Timeline of @%s:\n", user->Handle);
    printf("\n------------------------\n");

    user_follow_list = user->Following;
    curr_node_twt_list = list->Tail;

    /* find every tweet such that tweet.username is in user.followgin */
    while (curr_node_twt_list != NULL) {
        /* username of the curr twt */
        username = curr_node_twt_list->Tweet->Username;

        curr_node_follow_list = user_follow_list->Head;
        while (curr_node_follow_list != NULL) {
            if (!strcmp(username, curr_node_follow_list->User->Handle)) {
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

    printf("----------------------------------------------\n");
    printf("|   @%s's profile\n", aux_user->Handle);
    printf("|   %s \n", aux_user->Desc);
    printf("----------------------------------------------\n");

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

        /*system("clear");*/

        show_user_twts(user_to_find, table);

        do {
            if (!strcmp(user_to_find, user->Handle)) {
                    printf("following or leave: ");
            } else {
                printf("follow, following or leave: ");
            }

            fflush(stdout);
            scanf("%s", option);

            flag = 0;

            if (!strcmp(option, "follow") || !strcmp(option, "FOLLOW")) {
                if (strcmp(user_to_find, user->Handle)) {
                    if (!is_in_User_List(user->Following, user_to_find)) {
                        add_User_Node(user->Following, hash_search(table, user_to_find));
                        printf("Now you follow @%s!\n", user_to_find);
                    } else {
                        printf("You already follow @%s.\n", user_to_find);
                    }
                }
                flag = 1;

            } else if (!strcmp(option, "leave") || !strcmp(option, "LEAVE")) {
                printf("Returning to timeline.\n");
                flag = 1;

            } else if (!strcmp(option, "following") || !strcmp(option, "FOLLOWING")) {
                User *following_user = hash_search(table, user_to_find);
                printf("Users @%s follows:\n", user_to_find);
                print_User_List(following_user->Following);
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
    newTweet->TimeStamp = malloc(strlen(timestamp) + 1);
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

void find_user_mentions(char *user, Tweets_List *tweets) {

    Tweet_Node *CurrentNode = tweets->Tail;
    char mention[100];
    int flag_mentions = 0;

    strncpy(mention, "@", 2);
    strncat(mention, user, strlen(user) + strlen(mention)+ 1);

    printf("Mentions of @%s:\n", user);

    while (CurrentNode != NULL) {
        if (strstr(CurrentNode->Tweet->Tweet, mention) != NULL) {
            printf("\n@%s: \"%s\"\n",
            CurrentNode->Tweet->Username, CurrentNode->Tweet->Tweet);
            printf("(%s)\n", CurrentNode->Tweet->TimeStamp);
            printf("------------------------\n");
            flag_mentions = 1;
        }
        CurrentNode = CurrentNode->Prev;
    }
	printf("\n");

    if (!flag_mentions) {
        printf("No mentions found.\n");
        printf("\n------------------------\n\n");
    }
}

/**
 * Función para hacer hashing de una contraseña.
 * Se toma el largo del string y se hace X mod length
 * siendo X el ascii de cada caracter del string.
 * Luego, cada valor restante se transforma nuevamente
 * en un caracter y se guarda un string.
 * 
 * Posteriormente, asigna ese hash a la contraseña del usuario.
*/
char *hash_password(char *password) {
    int length = strlen(password);
    char *hashed = malloc(length+1);
    int i = 0;

    for (i = 0; i < length; i++) {
        hashed[i] = password[i]%length + 33;
    }

    return(hashed);
}