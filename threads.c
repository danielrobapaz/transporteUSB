#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct thread_args{
    int thread;
    int i;
};

void *prueba(void *arguments) {
    struct thread_args *arg = (struct thread_args *)arguments;
    int n = arg->i;
    int h;

    for (h = 0; h < n; h++) {
        printf("hilo %d -> %d\n", arg->thread, h);
    }
    return NULL;
}



int main() {
    struct thread_args *args1 = malloc (sizeof (struct thread_args));
    struct thread_args *args2 = malloc (sizeof (struct thread_args));
    struct thread_args *args3 = malloc (sizeof (struct thread_args));
    int n = 10;

    args1->thread = 1,
    args1->i = n;

    args2->thread = 2,
    args2->i = n;

    args3->thread = 3,
    args3->i = n;

    pthread_t thread1, thread2, thread3;

    pthread_create(&thread1, NULL, prueba, (void*)args1);
    pthread_create(&thread2, NULL, prueba, (void*)args2);
    pthread_create(&thread3, NULL, prueba, (void*)args3);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    free(args1);
    free(args2);
    free(args3);
    
    return 0;
}