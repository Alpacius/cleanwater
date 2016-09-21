#pragma once


#include    "./stdc_common.h"
#include    "./linux_common.h"

#include    "../common/util_list.h"
#include    "../common/cp_queue.h"
#include    "../common/recollectable.h"


struct thread_worker_posix {
    struct {
        pthread_t thread;
        void (*lifespan)(void *);
        void *container_reference;
    } base;
    struct {
        struct cp_queue *queue;
        struct {
            pthread_cond_t cond;
            pthread_mutex_t mutex;
        } sync;
    } tasks;
    struct clwater_recollector recollector;
};
