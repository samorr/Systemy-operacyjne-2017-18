#ifndef _SIMPLE_THREADS_H
#define _SIMPLE_THREADS_H 1

#include<ucontext.h>
#include<stdlib.h>

typedef struct sthread {
	ucontext_t *context;
    sthread_t  *next;
} sthread_t;

struct sthreads_global_queue {
    //first is currently running thread
    sthread_t *first;
    sthread_t *last;
};

struct sthreads_global_queue *global_threads_queue;

void init_threads(sthread_t *caller) {
    global_threads_queue = (struct sthreads_global_queue*) malloc(sizeof(sthreads_global_queue));
    global_threads_queue->first = caller;
    global_threads_queue->last = caller;
}

void schedule() {
    ucontext_t *new_context, *old_context;
    new_context = global_threads_queue->first->next->context;
    old_context = global_threads_queue->first->context;
    global_threads_queue->first = global_threads_queue->first->next;
    global_threads_queue->last->next = global_threads_queue->first;
    swapcontext(global_threads_queue->last->context, global_threads_queue->first->context);
}


#endif /* __SIMPLE_THREADS_H */
