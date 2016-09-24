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


#define     SYNC_QUEUE_READY       0
#define     SYNC_QUEUE_INACTIVE    1
#define     SYNC_QUEUE_PENDING     2


struct clwater_sync_task_queue *clwater_sync_task_queue_init(struct clwater_sync_task_queue *queue);
struct clwater_sync_task_queue *clwater_sync_task_queue_ruin(struct clwater_sync_task_queue *queue);
struct link_index *clwater_sync_task_queue_dequeue(struct clwater_sync_task_queue *queue);
void clwater_sync_task_queue_enqueue(struct clwater_sync_task_queue *queue, struct link_index *product);
struct clwater_sync_task_queue *clwater_sync_task_queue_new(void);
void clwater_sync_task_queue_delete(struct clwater_sync_task_queue *queue);
