#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

struct list_head {
    struct list_head *prev, *next;
};

#define LIST_HEAD_INIT(name) {&(name), &(name)}

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list) {
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *entry,
			      struct list_head *prev,
			      struct list_head *next) {
	next->prev = entry;
	entry->next = next;
	entry->prev = prev;
	prev->next = entry;
}

static inline void list_add(struct list_head *entry, struct list_head *head) {
	__list_add(entry, head, head->next);
}

static inline void list_add_tail(struct list_head *entry, struct list_head *head) {
	__list_add(entry, entry->prev, head);
}


#endif