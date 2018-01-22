#ifndef _SIMPLE_SEMAPHORES_H
#define _SIMPLE_SEMAPHORES_H 1

#include "simple_threads.h"

typedef struct ssemaphore {
    long      counter;
    sthread_t *first;
    sthread_t *last;
} ssemaphore_t;


void sem_wait(ssemaphore_t *sem);
void sem_signal(ssemaphore_t *sem);

void sem_push(ssemaphore_t *sem);


#endif /* _SIMPLE_SEMAPHORES_H */