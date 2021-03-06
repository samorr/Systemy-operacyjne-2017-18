#include <stdio.h>
#include "simple_threads.h"
#include "simple_semaphores.h"

sthread_t T1, T2, T3, Main;
ssemaphore_t sem;

void fn1() {
    printf("this is from 1\n");
    sem_wait(&sem);
    thread_yield();
    printf("once again 1\n");
    sem_signal(&sem);
    thread_exit((void *) 11);
}
void fn2() {
    printf("this is from 2\n");
    void *from3;
    thread_join(&T3, &from3);
    thread_yield();
    printf("value from 3 %ld\n", (long) from3);
    thread_exit(0);
}

void fn3() {
    printf("this is from 3\n");
    sem_wait(&sem);
    printf("3 after sem_wait\n");
    sem_signal(&sem);
    // void *from1;
    // if (thread_join(&T1, &from1)) {
    //     printf("Cannot join with T1\n");
    // }
    // printf("value from 1 %ld\n", (long) from1);
    thread_exit((void *) 10);
}

int main() {
    init_threads();
    sem.counter = 1;
    thread_create(&Main, (void *) &main, NULL);
    thread_detach(&Main);
    thread_create(&T1, (void *) &fn1, NULL);
    //thread_detach(&T1);
    thread_create(&T2, (void *) &fn2, NULL);
    thread_detach(&T2);
    thread_create(&T3, (void *) &fn3, NULL);
    printf("Main\n");
    schedule();
    printf("Main once again\n");
    schedule();
    printf("My last call\n");
    thread_exit(0);
}