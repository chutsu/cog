#ifndef COG_QUEUE_H
#define COG_QUEUE_H

#include <stdlib.h>

#include "list.h"
#include "util.h"

/* STRUCTURES */
struct queue {
  int count;
  struct list *queue;
};

/* ERROR MESSAGES */
#define QUEUE_ENULL "queue is NULL!"
#define QUEUE_EDNULL "data is NULL!"

/* FUNCTIONS */
struct queue *queue_new(void);
void queue_destroy(struct queue *q);
int queue_enqueue(struct queue *q, void *data);
void *queue_dequeue(struct queue *q);
int queue_count(struct queue *q);
int queue_empty(struct queue *q);
int queue_full(struct queue *q);
void *queue_first(struct queue *q);
void *queue_last(struct queue *q);

#endif
