#include "simple_semaphores.h"

void sem_wait(ssemaphore_t *sem) {
    sem->counter--;
    if (sem->counter < 0) {
        sem_push(sem);
    }
}

void sem_signal(ssemaphore_t *sem) {
    sem->counter++;
    if (sem->first != NULL) {
        sthread_t *thr = sem->first;
        sem->first = thr->next;
        global_threads_queue->last->next = thr;
        global_threads_queue->last = thr;
        thr->next = NULL;
    }
}

void sem_push(ssemaphore_t *sem) {
    sthread_t *thr = global_threads_queue->first;
    global_threads_queue->first = thr->next;
    if (sem->first == NULL) {
        sem->first = thr;
    } else {
        sem->last->next = thr;
    }
    sem->last = thr;
    thr->next = NULL;
    ommit_waitings(thr->context);
}