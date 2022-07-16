#ifndef _HELLOOS_LIST_H
#define _HELLOOS_LIST_H

#include "printk.h"

typedef struct list_head {
    struct list_head *prev, *next;
} list_head_t;

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
        struct list_head_t name = LIST_HEAD_INIT(name)

static inline void list_init(list_head_t *list) {
    list->next = list;
    list->prev = list;
}

static inline void __list_add(list_head_t *new,
                              list_head_t *prev,
                              list_head_t *next) {
    prev->next = new;
    new->prev = prev;
    new->next = next;
    next->prev = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 */
static inline void list_add(list_head_t *new, list_head_t *head) {
    __list_add(new, head, head->next);
}

/**
 * list_add_tail - add a new entry to tail
 * @new: new entry to be added
 * @head: list head to add it before
 */
static inline void list_add_tail(list_head_t *new, list_head_t *head) {
    __list_add(new, head->prev, head);
}

static inline void __list_del(list_head_t *prev, list_head_t *next) {
    prev->next = next;
    next->prev = prev;
}

/**
 * list_del - delete a entry
 * @prev: prev entry
 * @next: next entry
 */
static inline void list_del(list_head_t *prev, list_head_t *next) {
    __list_del(prev, next);
}

/**
 * list_del_entry - delete a entry
 * @entry: a entry will be deleted
 */
static inline void list_del_entry(list_head_t *entry) {
    __list_del(entry->prev, entry->next);
}

#endif