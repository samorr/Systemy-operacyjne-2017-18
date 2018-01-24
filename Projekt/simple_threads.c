#include "simple_threads.h"
#define MEM SIGSTKSZ

void thread_create(sthread_t *thr, void (*func)(), void *arg) {
    thr->context = (ucontext_t *) malloc(sizeof(ucontext_t));
    getcontext(thr->context);
    thr->context->uc_stack.ss_sp = malloc(MEM);
    thr->context->uc_stack.ss_size = MEM;
    thr->context->uc_stack.ss_flags = 0;
    thr->is_joinable = true;
    thr->is_finished = false;
    thr->waiting_for = 0;
    thr->joined = NULL;
    makecontext(thr->context, func, 1, arg);
    if (global_threads_queue->first == NULL) {
        global_threads_queue->first = thr;
    } else {
        global_threads_queue->last->next = thr;
    }
    global_threads_queue->last = thr;
    thr->next = NULL;
}

void schedule() {
    if (global_threads_queue->first->next == NULL) {
        return;
    }
    sthread_t *old_thread;
    old_thread = global_threads_queue->first;
    global_threads_queue->first = global_threads_queue->first->next;
    old_thread->next = NULL;
    global_threads_queue->last->next = old_thread;
    global_threads_queue->last = old_thread;
    if (global_threads_queue->first->waiting_for == 0) {
        swapcontext(global_threads_queue->last->context, global_threads_queue->first->context);
    } else {
        ommit_waitings(global_threads_queue->last->context);
    }
}

void ommit_waitings(ucontext_t *temp) {
    sthread_t *old_thread;
    while (global_threads_queue->first->waiting_for != 0) { //we need to ommit all waiting threads
        old_thread = global_threads_queue->first;
        global_threads_queue->first = global_threads_queue->first->next;
        old_thread->next = NULL;
        global_threads_queue->last->next = old_thread;
        global_threads_queue->last = old_thread;
    }
    if (temp != NULL) {
        getcontext(temp);        
    }
    if (temp != global_threads_queue->first->context) {
        setcontext(global_threads_queue->first->context);
    }
}

void init_threads() {
    global_threads_queue = (struct sthreads_global_queue*) malloc(sizeof(struct sthreads_global_queue));
}

void thread_yield() {
    // for now it is enough to just call schedule
    schedule();
}

int thread_join(sthread_t *thr, void **retval) {
    if (!thr->is_joinable) {
        return -1;
    }

    if (thr->is_finished) {
        if(retval != NULL) {
            *retval = thr->val_to_ret;
        }
        free(thr->context->uc_stack.ss_sp);
        free(thr->context);
    } else {
        thr->is_joinable = false;
        thr->where_to_ret = retval;
        thr->joined = global_threads_queue->first;
        // we have to increase number of threads that executing thread is waiting for
        global_threads_queue->first->waiting_for++;
    }
    return 0;
}

int thread_detach(sthread_t *thr) {
    if (thr->joined != NULL) {
        return -1;
    }
    thr->is_joinable = false;
    return 0;
}

void thread_exit(void *retval) {
    sthread_t *thr = global_threads_queue->first;
    // next thread to execute now
    global_threads_queue->first = global_threads_queue->first->next;

    // if exiting thread isn't joinable or it has been already joined we can save returned value and free its resources
    if (!thr->is_joinable) {
        if(thr->joined != NULL ) {
            thr->joined->waiting_for--;
        }
        if (thr->where_to_ret != NULL) {
            *thr->where_to_ret = retval;
        }
        free(thr->context->uc_stack.ss_sp);
        free(thr->context);
    } else { //else we need to save returned value for thread which can join this in the future
        thr->val_to_ret = retval;
    }
    thr->is_finished = true;
    // if there is no another threads to execute we can free global queue
    if (global_threads_queue->first == NULL) {
        free(global_threads_queue);
    } else {
        ommit_waitings(NULL);
    }
}