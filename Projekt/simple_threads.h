#ifndef _SIMPLE_THREADS_H
#define _SIMPLE_THREADS_H 1

#include<ucontext.h>
#include<stdlib.h>

typedef struct sthread {
	ucontext_t      *context;
    struct sthread  *next;
    void            *retval;
} sthread_t;

struct sthreads_global_queue {
    //first is currently running thread
    sthread_t *first;
    sthread_t *last;
};

struct sthreads_global_queue *global_threads_queue;

void init_threads(sthread_t *caller);

void schedule();

void thread_create(sthread_t *thr, void (*func)(), void *arg);
void thread_yield();
void thread_join(sthread_t *thr, void *retval);
void thread_exit();

#endif /* __SIMPLE_THREADS_H */
