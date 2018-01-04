#include "simple_threads.h"
#define MEM 64000

void thread_create(sthread_t *thr, void (*func)(), void *arg) {
    thr = (sthread_t *) malloc(sizeof(sthread_t));
    getcontext(thr->context);
    thr->context->uc_stack.ss_sp = malloc(MEM);
    thr->context->uc_stack.ss_size = MEM;
    thr->context->uc_stack.ss_flags = 0;
    makecontext(thr->context, func, 1, arg);
    global_threads_queue->last->next = thr;
    global_threads_queue->last = thr;
    thr->next = NULL;
}
void thread_yield() {

}
void thread_join(sthread_t *thr) {

}