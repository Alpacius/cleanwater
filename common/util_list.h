#pragma once

#include    <stddef.h>
#include    "./miscutils.h"


// circle linked list

struct link_index {
    struct link_index *prev, *next;
};

static inline
void list_init(struct link_index *h) {
    h->next = h;
    h->prev = h;
}

static inline
void list_add__(struct link_index *e, struct link_index *p, struct link_index *n) {
    n->prev = e;
    e->next = n;
    e->prev = p;
    p->next = e;
}

static inline
void list_add_head(struct link_index *e, struct link_index *h) {
    list_add__(e, h, h->next);
}

static inline
void list_add_tail(struct link_index *e, struct link_index *h) {
    list_add__(e, h->prev, h);
}

static inline
void list_del__(struct link_index *p, struct link_index *n) {
    n->prev = p;
    p->next = n;
}

static inline
void list_del(struct link_index *e) {
    list_del__(e->prev, e->next);
    e->next = e->prev = NULL;
}

static inline
int list_node_isolated(struct link_index *e) {
    return (e->next == NULL) || (e->prev == NULL);
}

static inline
void list_node_isolate(struct link_index *e) {
    e->prev = e->next = NULL;
}

static inline
int list_is_empty(struct link_index *h) {
    return (h->next == h) && (h->prev == h);
}

// XXX These macros are dangerous - arguments may be evaluated more than once.

#define     list_foreach(head) \
    for (struct link_index *iterator = (head)->next; iterator != (head); iterator = iterator->next)

#define     list_foreach_remove(head) \
    for (struct link_index *iterator_aux = (head)->next, *iterator = iterator_aux; \
        ((iterator = iterator_aux) != (head)) && (iterator_aux = iterator_aux->next);)

#define     current_iterator                        iterator 
#define     detach_current_iterator                 list_del(iterator)
#define     current_object_of_type(type, member) \
    container_of(iterator, type, member)
