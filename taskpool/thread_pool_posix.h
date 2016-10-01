#pragma once


#include    "./stdc_common.h"
#include    "./linux_common.h"
#include    "../common/util_list.h"
#include    "../common/cp_queue.h"
#include    "../common/recollectable.h"
#include    "./sync_task_queue.h"


struct clwater_thread_worker {
    struct {
        pthread_t thread;
        void (*lifespan)(void *);
        void *container_reference;
        ucontext_t ucontext;
    } base;
    struct {
        struct clwater_sync_task_queue queue;
        struct {
            sem_t semaphore;
        } sync;
    } tasks;
    uint8_t index;
    struct clwater_recollector async_result_recollector;
};
