#ifndef _SIMPLE_THREADS_H
#define _SIMPLE_THREADS_H 1

#include <ucontext.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct sthread {
    bool            is_joinable; // thread is not joinable if it isn't joinable at all or it has been joined already
    bool            is_finished; // resources of joinable thread can be freed only after joining
    struct sthread  *joined; // thread which has called thread_join
    void            *val_to_ret; // value from thread_exit waiting for joinig thread
    void            **where_to_ret; // adress where to save value from thread_exit for joined thread
    unsigned long    waiting_for; // if greater than zero then we are waiting for some threads to exit
                                 // because we called thread_join on them
	ucontext_t      *context;
    struct sthread  *next; // thread which is next in global queue
} sthread_t;

struct sthreads_global_queue {
    sthread_t *first; //first is currently running thread
    sthread_t *last;
};

struct sthreads_global_queue *global_threads_queue;

void init_threads();

void schedule();
void ommit_waitings();

void thread_create(sthread_t *thr, void (*func)(), void *arg);
void thread_yield();
int thread_join(sthread_t *thr, void **retval);
int thread_detach(sthread_t *thr);
void thread_exit(void *retval);

#endif /* __SIMPLE_THREADS_H */
