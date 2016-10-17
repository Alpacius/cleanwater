#pragma once

#include    <stddef.h>
#include    <stdint.h>

#include    "./util_list.h"


struct cp_queue {
    uint8_t index_producer, producer_busy;
    struct link_index buffers[2];
};

// Beware: the consumer may fail to refresh its buffer.
static inline
struct link_index *cp_queue_consume(struct cp_queue *queue) {
    uint8_t index_consumer = 1 - __atomic_load_n(&(queue->index_producer), __ATOMIC_ACQUIRE), desired_producer_state = 0;
    if (list_is_empty(&(queue->buffers[index_consumer])))
        __atomic_compare_exchange_n(&(queue->index_producer), &desired_producer_state, 1 - index_consumer, 0, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED);
    return &(queue->buffers[index_consumer]);
}

static inline
void cp_queue_produce(struct cp_queue *queue, struct link_index *product) {
    __atomic_store_n(&(queue->producer_busy), 1, __ATOMIC_RELEASE);
    list_add_tail(product, &(queue->buffers[__atomic_load_n(&(queue->index_producer), __ATOMIC_ACQUIRE)]));
    __atomic_store_n(&(queue->producer_busy), 0, __ATOMIC_RELEASE);
}

static inline
struct cp_queue *cp_queue_init(struct cp_queue *queue) {
    __atomic_store_n(&(queue->index_producer), 0, __ATOMIC_RELEASE);
    __atomic_store_n(&(queue->producer_busy), 0,__ATOMIC_RELEASE);
    list_init(&(queue->buffers[0]));
    list_init(&(queue->buffers[1]));
}
