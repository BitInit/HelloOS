#ifndef _HELLOOS_LIST_H
#define _HELLOOS_LIST_H

struct list_head {
    struct list_head *prev;
    struct list_head *next;
};

static inline void list_init(struct list_head *head) {
    head->prev = head;
    head->next = head;
}

static inline void __list_add(struct list_head *new, 
                              struct list_head *prev, 
                              struct list_head *next) {
    prev->next = new;
    new->next = next;
    new->prev = prev;
    next->prev = new;
}

static inline void list_add_tail(struct list_head *new, struct list_head *head) {
    __list_add(new, head, head->next);
}

static inline void list_add_head(struct list_head *new, struct list_head *head) {
    __list_add(new, head->prev, head);
}

static inline struct list_head* list_next(struct list_head *entry) {
    return entry->next;
}

#endif