#include    "./sync_task_queue.h"


struct clwater_sync_task_queue *clwater_sync_task_queue_init(struct clwater_sync_task_queue *queue) {
    __atomic_store_n(&(queue->status), ASYNC_QUEUE_INACTIVE, __ATOMIC_RELEASE);
    if (sem_init(&(queue->sync.semaphore), 0, 0) < 0) {
        return NULL;
    }
    cp_queue_init(&(queue->queue));
    __atomic_store_n(&(queue->status), ASYNC_QUEUE_READY, __ATOMIC_RELEASE);
    return queue;
}

struct clwater_sync_task_queue *clwater_sync_task_queue_ruin(struct clwater_sync_task_queue *queue) {
    sem_destroy(&(queue->sync.semaphore));
    __atomic_store_n(&(queue->status), ASYNC_QUEUE_INACTIVE, __ATOMIC_RELEASE);
    return queue;
}

struct link_index *clwater_sync_task_queue_dequeue(struct clwater_sync_task_queue *queue) {
    do {
        struct link_index *target_queue = cp_queue_consume(&(queue->queue));
        if (list_is_empty(target_queue)) {
            sem_wait(&(queue->sync.semaphore));
        } else {
            struct link_index *target_product = target_queue->next;
            list_del(target_product);
            return target_product;
        }
    } while (1);
}

void clwater_sync_task_queue_enqueue(struct clwater_sync_task_queue *queue, struct link_index *product) {
    sem_post(&(queue->sync.semaphore));
    cp_queue_produce(&(queue->queue), product);
}

struct clwater_sync_task_queue *clwater_sync_task_queue_new(void) {
    struct clwater_sync_task_queue *queue = malloc(sizeof(struct clwater_sync_task_queue));
    return queue ? clwater_sync_task_queue_init(queue) : NULL;
}

void clwater_sync_task_queue_delete(struct clwater_sync_task_queue *queue) {
    free(clwater_sync_task_queue_ruin(queue));
}
