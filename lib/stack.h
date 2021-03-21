#ifndef COG_STACK_H
#define COG_STACK_H

#include <stdlib.h>

/* STRUCTURES */
struct stack_node {
  void *value;
  struct stack_node *next;
  struct stack_node *prev;
};

struct stack {
  int size;
  struct stack_node *root;
  struct stack_node *end;
};

/* FUNCTIONS */
struct stack *stack_new(void);
void stack_destroy_traverse(struct stack_node *n, void (*free_func)(void *));
void stack_clear_destroy(struct stack *s, void (*free_func)(void *));
void stack_destroy(struct stack *s);
int stack_push(struct stack *s, void *value);
void *stack_pop(struct stack *s);

#endif
