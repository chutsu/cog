#include "queue.h"

struct queue *queue_new(void) {
  struct queue *q;

  q = calloc(1, sizeof(struct queue));
  q->queue = list_new();
  q->count = 0;

  return q;
}

void queue_destroy(struct queue *q) {
  /* pre-check */
  check(q, QUEUE_ENULL);

  /* destroy */
  list_destroy(q->queue);
  free(q);
  q = NULL;

error:
  return;
}

int queue_enqueue(struct queue *q, void *data) {
  /* pre-check */
  check(q, QUEUE_ENULL);
  check(data, QUEUE_EDNULL);

  /* enqueue */
  list_push(q->queue, data);
  q->count++;

  return 0;
error:
  return -1;
}

void *queue_dequeue(struct queue *q) {
  void *data;

  /* pre-check */
  check(q, QUEUE_ENULL);

  data = list_pop_front(q->queue);
  q->count--;

  return data;
error:
  return NULL;
}

int queue_empty(struct queue *q) {
  /* pre-check */
  check(q, QUEUE_ENULL);

  return (q->count == 0) ? 1 : 0;
error:
  return -1;
}

void *queue_first(struct queue *q) {
  /* pre-check */
  check(q, QUEUE_ENULL);

  if (q->count != 0) {
    return q->queue->first->value;
  } else {
    return NULL;
  }

error:
  return NULL;
}

void *queue_last(struct queue *q) {
  /* pre-check */
  check(q, QUEUE_ENULL);

  if (q->count != 0) {
    return q->queue->last->value;
  } else {
    return NULL;
  }

error:
  return NULL;
}
