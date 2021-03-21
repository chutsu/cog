#ifndef COG_LIST_H
#define COG_LIST_H

#include <stdlib.h>

#include "util.h"


/* STRUCTURES */
struct list_node {
    struct list_node *next;
    struct list_node *prev;
    void *value;
};

struct list {
    int length;
    struct list_node *first;
    struct list_node *last;
};

/* ERROR MESSAGES */
#define LIST_ENULL "list is NULL!"
#define LIST_ECNULL "cmp is NULL!"
#define LIST_EVNULL "value is NULL!"
#define LIST_EEMPTY "list is empty!"
#define LIST_EINNODE "input node is NULL!"


/* FUNCTIONS */
struct list *list_new(void);
void list_destroy(struct list *list);
void list_clear(struct list *list);
void list_clear_destroy(struct list *list);
void list_push(struct list *list, void *value);
void *list_pop(struct list *list);
void *list_pop_front(struct list *list);
void *list_shift(struct list *list);
void list_unshift(struct list *list, void *value);
void *list_remove(
    struct list *list,
    void *target,
    int (*cmp)(const void *, const void *)
);
int list_remove_destroy(
    struct list *list,
    void *value,
    int (*cmp)(const void *, const void *),
    void (*free_func)(void *)
);

#endif
