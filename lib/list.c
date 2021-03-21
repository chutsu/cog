#include "list.h"

struct list *list_new(void) {
  struct list *list;

  list = calloc(1, sizeof(struct list));
  list->length = 0;
  list->first = NULL;
  list->last = NULL;

  return list;
}

void list_destroy(struct list *list) {
  struct list_node *node;
  struct list_node *next_node;

  /* pre-check */
  silent_check(list);

  /* destroy */
  node = list->first;
  while (node != NULL) {
    next_node = node->next;
    free_mem(node, free);

    node = next_node;
  }

  free(list);
error:
  return;
}

void list_clear(struct list *list) {
  struct list_node *node;
  struct list_node *next_node;

  /* pre-check */
  silent_check(list);

  /* clear */
  node = list->first;
  while (node != NULL) {
    next_node = node->next;
    free(node->value);

    node = next_node;
  }

error:
  return;
}

void list_clear_destroy(struct list *list) {
  struct list_node *node;
  struct list_node *next_node;

  /* pre-check */
  silent_check(list);

  node = list->first;
  while (node != NULL) {
    next_node = node->next;
    free(node->value);
    free(node);

    node = next_node;
  }
  free(list);

error:
  return;
}

void list_push(struct list *list, void *value) {
  struct list_node *node;

  /* pre-check */
  check(list, LIST_ENULL);
  check(value, LIST_EVNULL);

  /* initialize node */
  node = calloc(1, sizeof(struct list_node));
  check_mem(node);
  node->value = value;

  /* push node */
  if (list->last == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }

  list->length++;
error:
  return;
}

void *list_pop(struct list *list) {
  void *value;
  struct list_node *last;
  struct list_node *before_last;

  /* setup */
  value = NULL;
  before_last = NULL;

  /* pre-check */
  check(list, LIST_ENULL);

  /* get last */
  last = list->last;
  silent_check(last);

  value = last->value;
  before_last = last->prev;
  free(last);

  /* pop */
  if (before_last == NULL && list->length == 1) {
    list->last = NULL;
    list->first = NULL;
  } else {
    list->last = before_last;
  }
  list->length--;

  silent_check(before_last);
  before_last->next = NULL;

  return value;
error:
  return value;
}

void *list_pop_front(struct list *list) {
  void *data;
  struct list_node *first_node;
  struct list_node *next_node;

  /* pre-check */
  check(list->first != NULL, LIST_EEMPTY);

  /* pop front */
  first_node = list->first;
  data = first_node->value;
  next_node = first_node->next;

  if (next_node != NULL) {
    list->first = next_node;
  } else {
    list->first = NULL;
  }
  list->length--;

  /* clean up */
  free(first_node);

  return data;
error:
  return NULL;
}

void *list_shift(struct list *list) {
  void *value;
  struct list_node *first;
  struct list_node *second;

  /* pre-check */
  check(list, LIST_ENULL);
  check(list, LIST_EEMPTY);

  /* shift */
  first = list->first;
  value = first->value;
  second = list->first->next;

  list->first = second;
  list->length--;
  free(first);

  return value;
error:
  return NULL;
}

void list_unshift(struct list *list, void *value) {
  struct list_node *node;

  /* pre-check */
  check(list, LIST_ENULL);

  /* unshift */
  node = calloc(1, sizeof(struct list_node));
  check_mem(node);
  node->value = value;

  if (list->first == NULL) {
    list->first = node;
    list->last = node;
  } else {
    node->next = list->first;
    list->first->prev = node;
    list->first = node;
  }

  list->length++;

error:
  return;
}

void *list_remove(struct list *list,
                  void *value,
                  int (*cmp)(const void *, const void *)) {
  struct list_node *node;

  /* pre-check */
  check(list, LIST_ENULL);
  check(value, LIST_EVNULL);
  check(cmp, LIST_ECNULL);

  /* iterate list */
  node = list->first;
  while (node != NULL) {

    /* compare target with node value */
    if (cmp(node->value, value) == 0) {
      value = node->value;

      if (list->length == 1) {
        /* last node in list */
        list->first = NULL;
        list->last = NULL;

      } else if (node == list->first) {
        /* first node in list */
        list->first = node->next;
        node->next->prev = NULL;

      } else if (node == list->last) {
        /* in the case of removing last node in list */
        list->last = node->prev;
        node->prev->next = NULL;

      } else {
        /* remove others */
        node->prev->next = node->next;
        node->next->prev = node->prev;
      }
      list->length--;
      free(node);

      return value;
    }

    node = node->next;
  }

  return NULL;
error:
  return NULL;
}

int list_remove_destroy(struct list *list,
                        void *value,
                        int (*cmp)(const void *, const void *),
                        void (*free_func)(void *)) {
  void *result;

  result = list_remove(list, value, cmp);
  silent_check(result);
  free_func(result);

  return 0;
error:
  return -1;
}
