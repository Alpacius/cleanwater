#pragma once

#include    "./stdc_common.h"
#include    "./linux_common.h"
#include    "../common/util_list.h"
#include    "../common/cp_queue.h"


struct clwater_sync_task_queue {
    int status;
    struct cp_queue queue;
    struct {
        sem_t semaphore;
    } sync;
};


#define     ASYNC_QUEUE_READY       0
#define     ASYNC_QUEUE_INACTIVE    1
#define     ASYNC_QUEUE_PENDING     2
