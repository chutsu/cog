#ifndef COG_HASHMAP_H
#define COG_HASHMAP_H

#include <stdint.h>
#include <string.h>

#include "util.h"
#include "list.h"
#include "darray.h"

/* STRUCTURES */
struct hashmap_node {
  uint32_t hash;
  void *key;
  void *value;
};

struct hashmap {
  struct darray *buckets;
  int (*cmp)(void *, void *);
  uint32_t (*hash)(void *);

  int copy_kv;
  void *(*k_copy)(void *);
  void *(*v_copy)(void *);
  void (*k_free)(void *);
  void (*v_free)(void *);
};

/* CONSTANTS */
#ifndef DEFEAULT_NUMBER_OF_BUCKETS
#define DEFAULT_NUMBER_OF_BUCKETS 10000
#endif

/* ERROR MESSAGES */
#define HASHMAP_ENULL "hashmap is NULL!"
#define HASHMAP_EKNULL "key is NULL!"
#define HASHMAP_EVNULL "value is NULL!"
#define HASHMAP_EBNULL "bucket is NULL!"
#define HASHMAP_EKCOPY "hashmap->k_copy is NULL!"
#define HASHMAP_EVCOPY "hashmap->v_copy is NULL!"
#define HASHMAP_EBUCKET "invalid bucket found: %d"
#define HASHMAP_ECBUCKET "cannot create bucket!"
#define HASHMAP_EGET "cannot find node from bucket, it should exist!"
#define HASHMAP_ETCBNULL "hashmap_traverse_cb is NULL!"

/* FUNCTIONS */
struct hashmap *hashmap_new(void);
void hashmap_clear_destroy(struct hashmap *map);
void hashmap_destroy(struct hashmap *map);
int hashmap_set(struct hashmap *map, void *key, void *data);
void *hashmap_get(struct hashmap *map, void *key);
int hashmap_traverse(struct hashmap *map,
                     int (*hashmap_traverse_cb)(struct hashmap_node *node));
void *hashmap_delete(struct hashmap *map, void *key);

#endif
