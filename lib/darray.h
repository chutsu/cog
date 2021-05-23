#ifndef COG_DARRAY_H
#define COG_DARRAY_H

#include <stdlib.h>
#include <math.h>
#include "util.h"

/* STRUCTURES */
struct darray {
  int end;
  int max;

  size_t element_size;
  size_t expand_rate;

  void **contents;
};

/* CONSTANTS */
#ifndef DEFAULT_EXPAND_RATE
#define DEFAULT_EXPAND_RATE 300
#endif

/* ERROR MESSAGES */
#define DARRAY_ENULL "darray is NULL!"
#define DARRAY_EEL "element is NULL!"
#define DARRAY_ECMP "comparator is NULL!"
#define DARRAY_EEMPTY "darray is empty!"
#define DARRAY_EINDEXOUT "index is out of bounds!"
#define DARRAY_EELSIZE "element size must be > 0!"
#define DARRAY_EINITMAX "initial max must be > 0!"
#define DARRAY_ENEWMAX "new max must be > 0!"
#define DARRAY_EEXPAND "failed to expand array to new size: %d"

/* FUNCTIONS */
struct darray *darray_new(size_t element_size, size_t initial_max);
void darray_destroy(struct darray *array);
void darray_clear(struct darray *array);
void darray_clear_destroy(struct darray *array);

int darray_push(struct darray *array, void *el);
void *darray_pop(struct darray *array);

int darray_contains(struct darray *array,
                    void *el,
                    int (*cmp)(const void *, const void *));
struct darray *darray_copy(struct darray *array);

void *darray_new_element(struct darray *array);
void *darray_first(struct darray *array);
void *darray_last(struct darray *array);

void darray_set(struct darray *array, int i, void *el);
void *darray_get(struct darray *array, int i);

void *darray_update(struct darray *array, int i, void *el);
void *darray_remove(struct darray *array, int i);

int darray_expand(struct darray *array);
int darray_contract(struct darray *array);

#endif
