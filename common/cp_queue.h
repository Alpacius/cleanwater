#pragma once

#include    <stddef.h>
#include    <stdint.h>

#include    "./util_list.h"


static uint8_t index_mapping_[] = {
    [0]            = 0,
    [(uint8_t) -1] = 1
};

struct cp_queue {
    uint8_t index_producer;
    struct link_index buffers[2];
};

#define producer_index_of(queue_) __atomic_load_n(&((queue_)->index_producer), __ATOMIC_ACQUIRE)
#define consumer_index_of(queue_) (1 - producer_index_of((queue_)))
#define new_consumer_index_of(queue_) \
    ({ \
        __auto_type queue__ = (queue_); \
        index_mapping_[ \
            __atomic_fetch_xor(&(queue__->index_producer), (uint8_t) -1, __ATOMIC_ACQ_REL) \
        ]; \
    })

static inline
struct link_index *cp_queue_consume(struct cp_queue *queue) {
    uint8_t index_consumer = consumer_index_of(queue);
    return
        list_is_empty(&(queue->buffers[index_consumer])) ?
            &(queue->buffers[index_consumer]) :
            &(queue->buffers[new_consumer_index_of(queue)]);
}

static inline
void cp_queue_produce(struct cp_queue *queue, struct link_index *product) {
    list_add_tail(product, &(queue->buffers[producer_index_of(queue)]));
}

static inline
struct cp_queue *cp_queue_init(struct cp_queue *queue) {
    __atomic_store_n(&(queue->index_producer), 0, __ATOMIC_RELEASE);
    list_init(&(queue->buffers[0]));
    list_init(&(queue->buffers[1]));
}
