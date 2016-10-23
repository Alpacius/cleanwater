#pragma once

#include    <stddef.h>
#include    <stdint.h>

#include    "./util_list.h"


struct cp_queue {
    uint8_t index_producer;
    struct link_index buffers[2];
};

#define     CP_QUEUE_PRODUCER_BUSY              0x2
#define     CP_QUEUE_INITIAL_PRODUCER_INDEX     0x0

#define     cp_queue_normalized_index(index_)   ((index_) & 0x1)
#define     cp_queue_flipped_index(index_)      (1 - cp_queue_normalized_index((index_)))
#define     cp_queue_flipped_index_raw(index_)  (1 - (index_))

#define     cp_queue_index_of(queue_)           __atomic_load_n(&((queue_)->index_producer), __ATOMIC_ACQUIRE)

// Beware: the consumer may fail to refresh its buffer.
static inline
struct link_index *cp_queue_consume(struct cp_queue *queue) {
    uint8_t desired_index = cp_queue_normalized_index(cp_queue_index_of(queue));
    __atomic_compare_exchange_n(&(queue->index_producer), &desired_index, cp_queue_flipped_index_raw(desired_index), 0, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED);
    return &(queue->buffers[cp_queue_flipped_index(queue->index_producer)]);
}

static inline
void cp_queue_produce(struct cp_queue *queue, struct link_index *product) {
    __atomic_or_fetch(&(queue->index_producer), CP_QUEUE_PRODUCER_BUSY, __ATOMIC_RELEASE);
    {
        struct link_index *target_queue = &(queue->buffers[cp_queue_normalized_index(cp_queue_index_of(queue))]);
        list_add_tail(product, target_queue);
    }
    __atomic_and_fetch(&(queue->index_producer), ~CP_QUEUE_PRODUCER_BUSY, __ATOMIC_RELEASE);
}

static inline
struct cp_queue *cp_queue_init(struct cp_queue *queue) {
    __atomic_store_n(&(queue->index_producer), CP_QUEUE_INITIAL_PRODUCER_INDEX, __ATOMIC_RELEASE);
    list_init(&(queue->buffers[0]));
    list_init(&(queue->buffers[1]));
}

#undef      CP_QUEUE_PRODUCER_BUSY
#undef      CP_QUEUE_INITIAL_PRODUCER_INDEX

#undef      cp_queue_normalized_index
#undef      cp_queue_flipped_index
#undef      cp_queue_flipped_index_raw

#undef      cp_queue_index_of
