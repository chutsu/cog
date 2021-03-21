#include "hashmap.h"

static inline int default_cmp(void *a, void *b) { return strcmp(a, b); }

static uint32_t default_hash(void *a) {
  size_t len;
  char *k;
  uint32_t i;
  uint32_t hash;

  /* setup */
  len = strlen(a);
  k = a;
  i = 0;
  hash = 0;

  /* simple bob jenkins's hash algorithm */
  for (hash = i = 0; i < len; i++) {
    hash += (uint32_t) k[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

static inline void *default_key_copy(void *target) {
  return malloc_string(target);
}

static inline void *default_value_copy(void *target) {
  return malloc_string(target);
}

struct hashmap *hashmap_new(void) {
  struct hashmap *map;
  size_t list_sz;

  /* setup */
  list_sz = sizeof(struct darray *);
  map = malloc(sizeof(struct hashmap));
  check_mem(map);

  /* create bucket */
  map->buckets = darray_new(list_sz, DEFAULT_NUMBER_OF_BUCKETS);
  map->buckets->end = map->buckets->max; // fake out expanding it
  check_mem(map->buckets);

  /* set comparator and hash functions */
  map->cmp = default_cmp;
  map->hash = default_hash;

  /* set key and value copy functions */
  map->copy_kv = 1;
  map->k_copy = default_key_copy;
  map->v_copy = default_value_copy;
  map->k_free = free;
  map->v_free = free;

  return map;
error:
  free_mem(map, hashmap_destroy);
  return NULL;
}

static void free_bucket(struct darray *bucket) {
  int i;
  struct hashmap_node *n;

  /* pre-check */
  check(bucket, HASHMAP_EBNULL);

  /* free bucket */
  for (i = 0; i < bucket->end; i++) {
    n = darray_get(bucket, i);
    free(n);
  }

  darray_destroy(bucket);

error:
  return;
}

static void clear_free_bucket(struct hashmap *map, struct darray *bucket) {
  int i;
  struct hashmap_node *n;

  /* pre-check */
  check(map, HASHMAP_ENULL);
  check(bucket, HASHMAP_EBNULL);

  /* clear free bucket */
  for (i = 0; i < bucket->end; i++) {
    n = darray_get(bucket, i);
    map->k_free(n->key);
    map->k_free(n->value);
    free(n);
  }

  darray_destroy(bucket);

error:
  return;
}

static void free_buckets(struct hashmap *map) {
  int i;
  struct darray *bucket;

  /* pre-check */
  check(map, HASHMAP_ENULL);

  /* free buckets */
  for (i = 0; i < map->buckets->end; i++) {
    bucket = darray_get(map->buckets, i);

    if (bucket) {
      if (map->copy_kv) {
        clear_free_bucket(map, bucket);
      } else {
        free_bucket(bucket);
      }
    }
  }

  darray_destroy(map->buckets);

error:
  return;
}

void hashmap_clear_destroy(struct hashmap *map) {
  if (map) {
    if (map->buckets) {
      free_buckets(map);
    }
    free(map);
  }
}

void hashmap_destroy(struct hashmap *map) {
  if (map) {
    if (map->buckets) {
      free_buckets(map);
    }
    free(map);
  }
}

static struct hashmap_node *hashmap_node_new(uint32_t h, void *k, void *v) {
  struct hashmap_node *node;

  /* pre-check */
  check(k, HASHMAP_EKNULL);
  check(v, HASHMAP_EVNULL);

  /* setup */
  node = calloc(1, sizeof(struct hashmap_node));
  check_mem(node);

  /* create hashmap node */
  node->key = k;
  node->value = v;
  node->hash = h;

  return node;
error:
  return NULL;
}

static struct darray *hashmap_find_bucket(struct hashmap *map,
                                          void *k,
                                          int create,
                                          uint32_t *hash_out) {
  int bucket_n;
  uint32_t hash;
  struct darray *bucket;

  /* pre-check */
  check(map, HASHMAP_ENULL);
  check(k, HASHMAP_EKNULL);
  hash = map->hash(k);
  bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS;
  check(bucket_n >= 0, HASHMAP_EBUCKET, bucket_n);
  *hash_out = hash; /* store it for return so caller can use it */

  /* find bucket */
  bucket = darray_get(map->buckets, bucket_n);

  /* coundn't find bucket, create one instead */
  if (!bucket && create) {
    /* new bucket, set it up */
    bucket = darray_new(sizeof(void *), DEFAULT_NUMBER_OF_BUCKETS);
    check_mem(bucket);
    darray_set(map->buckets, bucket_n, bucket);
  }

  return bucket;
error:
  return NULL;
}

int hashmap_set(struct hashmap *map, void *k, void *v) {
  uint32_t hash;
  struct darray *bucket;
  struct hashmap_node *node;

  /* pre-check */
  check(map, HASHMAP_ENULL);
  check(map->k_copy, HASHMAP_EKCOPY);
  check(map->v_copy, HASHMAP_EVCOPY);
  check(k, HASHMAP_EKNULL);
  check(v, HASHMAP_EVNULL);
  bucket = hashmap_find_bucket(map, k, 1, &hash);
  check(bucket, HASHMAP_ECBUCKET);

  /* set hashmap  */
  node = hashmap_node_new(hash, map->k_copy(k), map->v_copy(v));
  check_mem(node);
  darray_push(bucket, node);

  return 0;
error:
  return -1;
}

static inline int hashmap_get_node(struct hashmap *map,
                                   uint32_t hash,
                                   struct darray *bucket,
                                   void *k) {
  int i;
  struct hashmap_node *node;

  /* pre-check */
  check(map, HASHMAP_ENULL);
  check(bucket, HASHMAP_EBNULL);
  check(k, HASHMAP_EKNULL);

  /* get node */
  for (i = 0; i < bucket->end; i++) {
    node = darray_get(bucket, i);
    if (node->hash == hash && map->cmp(node->key, k) == 0) {
      return i;
    }
  }

  return -1;
error:
  return -1;
}

void *hashmap_get(struct hashmap *map, void *k) {
  int i = 0;
  uint32_t hash = 0;
  struct darray *bucket;
  struct hashmap_node *node;

  /* pre-check */
  check(map, HASHMAP_ENULL);
  check(k, HASHMAP_EKNULL);

  /* find bucket */
  bucket = hashmap_find_bucket(map, k, 0, &hash);
  silent_check(bucket);

  /* find hashmap node */
  i = hashmap_get_node(map, hash, bucket, k);
  silent_check(i != -1);

  /* get value */
  node = darray_get(bucket, i);
  check(node != NULL, HASHMAP_EGET);

  return node->value;
error:
  return NULL;
}

int hashmap_traverse(struct hashmap *map,
                     int (*hashmap_traverse_cb)(struct hashmap_node *)) {
  int i;
  int j;
  int rc;
  struct darray *bucket;
  struct hashmap_node *node;

  /* pre-check */
  check(map, HASHMAP_ENULL);
  check(hashmap_traverse_cb, HASHMAP_ETCBNULL);

  /* traverse */
  rc = 0;
  for (i = 0; i < map->buckets->end; i++) {
    bucket = darray_get(map->buckets, i);

    if (bucket) {
      for (j = 0; j < bucket->end; j++) {
        node = darray_get(bucket, j);
        rc = hashmap_traverse_cb(node);

        if (rc != 0) {
          return rc;
        }
      }
    }
  }

  return 0;
error:
  return -1;
}

void *hashmap_delete(struct hashmap *map, void *k) {
  int i;
  void *v;
  uint32_t hash;
  struct darray *bucket;
  struct hashmap_node *node;
  struct hashmap_node *ending;

  /* pre-check */
  check(map, HASHMAP_ENULL);
  check(k, HASHMAP_EKNULL);

  /* setup */
  i = 0;
  hash = 0;

  /* find bucket containing hashmap node */
  bucket = hashmap_find_bucket(map, k, 0, &hash);
  silent_check(bucket);

  /* from bucket get hashmap node and free it */
  i = hashmap_get_node(map, hash, bucket, k);
  silent_check(i != -1);

  /* get node */
  node = darray_get(bucket, i);
  v = node->value;
  if (map->copy_kv) {
    map->k_free(node->key);
  }
  free(node);

  /* check to see if last element in bucket is a node */
  ending = darray_pop(bucket);
  if (ending != node) {
    /* alright looks like it's not the last one, swap it */
    darray_set(bucket, i, ending);
  }

  return v;
error:
  return NULL;
}
