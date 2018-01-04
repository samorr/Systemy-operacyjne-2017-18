#ifndef _SIMPLE_THREADS_H
#define _SIMPLE_THREADS_H 1

#include<ucontext.h>
#include<stdlib.h>

typedef struct sthread {
	ucontext_t *context;
    struct sthread  *next;
} sthread_t;

struct sthreads_global_queue {
    //first is currently running thread
    sthread_t *first;
    sthread_t *last;
};

struct sthreads_global_queue *global_threads_queue;

void init_threads(sthread_t *caller) {
    global_threads_queue = (struct sthreads_global_queue*) malloc(sizeof(struct sthreads_global_queue));
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

void thread_create(sthread_t *thr, void (*func)(), void *arg);
void thread_yield();
void thread_join(sthread_t *thr);


#endif /* __SIMPLE_THREADS_H */
