#include "simple_threads.h"
#include<stdio.h>
#define MEM 64000

void thread_create(sthread_t *thr, void (*func)(), void *arg) {
    // printf("creating\n");
    thr = (sthread_t *) malloc(sizeof(sthread_t));
    thr->context = (ucontext_t*) malloc(sizeof(ucontext_t));
    getcontext(thr->context);
    thr->context->uc_stack.ss_sp = malloc(MEM);
    thr->context->uc_stack.ss_size = MEM;
    thr->context->uc_stack.ss_flags = 0;
    makecontext(thr->context, func, 1, arg);
    // printf("hello\n");
    if (global_threads_queue->first == NULL) {
        global_threads_queue->first = thr;
    }
    if (global_threads_queue->last != NULL) {
        global_threads_queue->last->next = thr;
    }
    global_threads_queue->last = thr;
    thr->next = NULL;
}

void schedule() {
    // printf("scheduling\n");
    sthread_t *old_thread;
    old_thread = global_threads_queue->first;
    global_threads_queue->first = global_threads_queue->first->next;
    old_thread->next = NULL;
    global_threads_queue->last->next = old_thread;
    global_threads_queue->last = old_thread;
    // printf("hello\n");
    swapcontext(global_threads_queue->last->context, global_threads_queue->first->context);
}

void init_threads(sthread_t *caller) {
    global_threads_queue = (struct sthreads_global_queue*) malloc(sizeof(struct sthreads_global_queue));
    // global_threads_queue->first = caller;
    // global_threads_queue->last = caller;
}

void thread_yield() {

}
void thread_join(sthread_t *thr, void *retval) {

}
void thread_exit() {
    // printf("exiting\n");
    sthread_t *thr = global_threads_queue->first;
    global_threads_queue->first = global_threads_queue->first->next;
    free(thr->context->uc_stack.ss_sp);
    free(thr->context);
    free(thr);
    if (global_threads_queue->first == NULL) {
        // printf("this is the end\n");
        free(global_threads_queue);
    } else {
        setcontext(global_threads_queue->first->context);
    }
}