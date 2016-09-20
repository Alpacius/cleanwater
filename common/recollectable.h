#pragma once

#include    <stddef.h>
#include    <stdint.h>

#include    "./util_list.h"


struct clwater_recollector {
    uint32_t n_current_elements, n_max_elements;
    struct link_index pool;
};


static inline
struct clwater_recollector *clwater_recollector_init(struct clwater_recollector *recollector, uint32_t n_max_elements) {
    (recollector->n_max_elements = n_max_elements), (recollector->n_current_elements = 0);
    list_init(&(recollector->pool));
}

static inline
struct link_index *clwater_recollector_ruin(struct clwater_recollector *recollector) {
    return &(recollector->pool);
}

static inline
int clwater_recollector_put(struct clwater_recollector *recollector, struct link_index *linkable) {
    if (recollector->n_current_elements < recollector->n_max_elements) {
        recollector->n_current_elements++;
        list_add_tail(linkable, &(recollector->pool));
        return 1;
    }
    return 0;
}

static inline
struct link_index *clwater_recollector_get(struct clwater_recollector *recollector) {
    if (recollector->n_current_elements) {
        struct link_index *target = recollector->pool.next;
        list_del(target);
        return target;
    }
    return NULL;
}
