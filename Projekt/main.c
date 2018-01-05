#include<stdio.h>
#include<stdlib.h>
#include "simple_threads.h"

sthread_t T1, T2, T3, Main;

void fn1() {
    printf("this is from 1\n");
    schedule();
    printf("once again 1\n");
    thread_exit();
}
void fn2() {
    printf("this is from 2\n");
    thread_exit();
}

void fn3() {
    printf("this is from 3\n");
    thread_exit();
}

int main() {
    init_threads(&Main);
    thread_create(&Main, (void *) &main, NULL);
    thread_create(&T1, (void *) &fn1, NULL);
    thread_create(&T2, (void *) &fn2, NULL);
    thread_create(&T3, (void *) &fn3, NULL);
    printf("Main\n");
    schedule();
    printf("Main once again\n");
    schedule();
    printf("My last call\n");
    thread_exit();
}