// using GCC extensions

#ifndef        UTIL_LIST_H_
#define        UTIL_LIST_H_

#include    <stddef.h>
#include    "./miscutils.h"


// circle linked list

struct link_index {
    struct link_index *prev, *next;
};

static inline void init_list_head(struct link_index *h) {
    h->next = h;
    h->prev = h;
}

static inline void list_add__(struct link_index *e, struct link_index *p, struct link_index *n) {
    n->prev = e;
    e->next = n;
    e->prev = p;
    p->next = e;
}

static inline void list_add_head(struct link_index *e, struct link_index *h) {
    list_add__(e, h, h->next);
}

static inline void list_add_tail(struct link_index *e, struct link_index *h) {
    list_add__(e, h->prev, h);
}

static inline void list_del__(struct link_index *p, struct link_index *n) {
    n->prev = p;
    p->next = n;
}

static inline void list_del(struct link_index *e) {
    list_del__(e->prev, e->next);
    e->next = e->prev = NULL;
}

static inline int list_node_isolated(struct link_index *e) {
    return (e->next == NULL) || (e->prev == NULL);
}

static inline void list_node_isolate(struct link_index *e) {
    e->prev = e->next = NULL;
}

static inline int list_is_empty(struct link_index *h) {
    return (h->next == h) && (h->prev == h);
}

#define    list_foreach(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define    list_foreach_remove(pos, head, tmp) \
    for (pos = (head)->next; \
        ((tmp = pos) != (head)) && (pos = pos->next);)

#endif        // UTIL_LIST_H_
