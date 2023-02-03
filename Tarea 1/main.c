#include <stdio.h>
#include <stdlib.h>

#define MAXINPUTSIZE 6
#define MAXTWTSIZE 100

int main() {
    char *input;

    /* biggest input is 6 char */
    input = malloc(MAXINPUTSIZE);

    do {
        printf("DON'T MISS WHAT'S HAPPENING! LOGIN, SIGNUP OR LEAVE\n");
        scanf("%s", input);
        printf("%s \n", input);
    } while(1==1);
    
}