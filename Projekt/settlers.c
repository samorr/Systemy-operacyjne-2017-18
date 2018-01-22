#include <stdio.h>
#include "simple_threads.h"
#include "simple_semaphores.h"
#include <time.h>
#include <unistd.h>

#define MAX_SETTLERS 10

int Animals, Food;
ssemaphore_t animalSem, foodSem;
sthread_t huntersThreads[MAX_SETTLERS];
sthread_t cooksThreads[MAX_SETTLERS];
sthread_t checker, Main;

void hunterJob(void *id) {
    for(int i = 0; i < 365; i++) {
        thread_yield();
        sem_wait(&animalSem);
        int v1 = rand() % 6;
        int v2 = rand() % 6;
        thread_yield();
        if(v1 > v2) {
            Animals++;
        }
        thread_yield();
        sem_signal(&animalSem);
        sem_wait(&foodSem);
        thread_yield();
        if(Food > 0) {
            Food--;
            sem_signal(&foodSem);
            thread_yield();
        } else {
            sem_signal(&foodSem);
            thread_exit(NULL);
            return;
        }
        //usleep(1000);
    }
    printf("Osadnik przetrwał cały rok\n");
    thread_exit(NULL);
    return;
}
  
void cookJob(void *id) {
    for(int i = 0; i < 365; i++) {
        thread_yield();
        sem_wait(&animalSem);
        if(Animals > 0) {
            Animals--;
            sem_signal(&animalSem);
            thread_yield();
            sem_wait(&foodSem);
            int v1 = rand() % 6;
            Food += v1;
            sem_signal(&foodSem);
            thread_yield();
        } else {
            sem_signal(&animalSem);
        }
        sem_wait(&foodSem);
        if(Food > 0){
            Food--;
            thread_yield();
            sem_signal(&foodSem);
        } else {
            sem_signal(&foodSem);
            thread_exit(NULL);
            return;
        }
        //usleep(1000);
    }
    printf("Osadnik przetrwał cały rok\n");
    thread_exit(NULL);
    return;
}

void checkerJob(void *t) {
    for(int i = 0; i < 365; i++) {
        if(Animals < 0)
            printf("Animal error\n");

        if(Food < 0)
            printf("Food error\n");

        //usleep(10);
    }
    thread_exit(NULL);
    return;
}

int main (int argc, char *argv[]) {
    init_threads();    
    thread_create(&Main, (void *) &main, NULL);
    thread_detach(&Main);
    if(argc != 5){
        thread_exit((void *) -1);
    }
    srand(time(NULL));
    int startHunters = atoi(argv[1]), startCooks = atoi(argv[2]);
    Animals = atoi(argv[3]);
    Food = atoi(argv[4]);

    // thread_create(&checker,(void *) &checkerJob, NULL);
    // thread_detach(&checker);

    animalSem.counter = 1;
    foodSem.counter = 1;

    for(long i = 0; i < startHunters; i++) {
        thread_create(&huntersThreads[i],(void *) &hunterJob, NULL);
        thread_detach(&huntersThreads[i]);
    }

    for(long i = 0; i < startCooks; i++) {
        thread_create(&cooksThreads[i], (void *) &cookJob, NULL);
        thread_detach(&cooksThreads[i]);
    }

    thread_exit(NULL);
    return 0;
}
