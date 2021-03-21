#include "darray.h"


struct darray *darray_new(size_t element_size, size_t initial_max)
{
    struct darray *array;

    /* pre-check */
    check(element_size > 0, DARRAY_EELSIZE);
    check(initial_max > 0, DARRAY_EINITMAX);

    /* setup */
    array = NULL;

    /* create darray */
    array = malloc(sizeof(struct darray));
    check_mem(array);

    array->end = 0;
    array->max = (int) initial_max;
    array->element_size = element_size;
    array->expand_rate = DEFAULT_EXPAND_RATE;
    array->contents = calloc(initial_max, sizeof(void *));
    check_mem(array->contents);

    return array;

error:
    free_mem(array, free);
    return NULL;
}

void darray_clear(struct darray *array)
{
    int i;

    /* pre-check */
    silent_check(array != NULL);
    silent_check(array->element_size > 0);

    /* clear */
    for (i = 0; i < array->max; i++) {
        free_mem(array->contents[i], free);
    }

error:
    return;
}

void darray_destroy(struct darray *array)
{
    if (array) {
        if (array->contents) {
            free(array->contents);
        }
        free(array);
    }
}

void darray_clear_destroy(struct darray *array)
{
    if (array) {
        darray_clear(array);
        darray_destroy(array);
    }
}

int darray_push(struct darray *array, void *el)
{
    /* pre-check */
    check(array != NULL, DARRAY_ENULL);
    check(array != NULL, DARRAY_EEL);

    /* push */
    array->contents[array->end] = el;
    array->end++;

    /* expand darray if necessary */
    if (array->end >= array->max) {
        return darray_expand(array);
    }

    return 0;
error:
    return -1;
}

void *darray_pop(struct darray *array)
{
    void *el;
    int expanded;
    int trailing_memory;

    /* pre-check */
    check(array->end - 1 >= 0, DARRAY_EEMPTY);

    /* pop */
    el = darray_remove(array, array->end - 1);
    array->end--;

    /* contract */
    expanded = array->end > (int) array->expand_rate;
    trailing_memory = array->end % (int) array->expand_rate;
    if (expanded && trailing_memory) {
        darray_contract(array);
    }

    return el;
error:
    return NULL;
}

int darray_contains(
    struct darray *array,
    void *el,
    int (*cmp)(const void *, const void *)
)
{
    int i;
    void *element;

    /* pre-check */
    check(array != NULL, DARRAY_ENULL);
    check(el != NULL, DARRAY_EEL);
    check(cmp != NULL, DARRAY_ECMP);

    /* check first element */
    element = darray_get(array, 0);
    if (element != NULL && cmp(element, el) == 0) {
        return 1;
    }

    /* rest of element */
    for (i = 0; i < array->end; i++) {
        element = darray_get(array, i);
        if (element != NULL && cmp(element, el) == 0) {
            return 1;
        }
    }

    return 0;
error:
    return -1;
}

struct darray *darray_copy(struct darray *array)
{
    int i;
    void *el;
    void *el_copy;
    struct darray *array_copy;

    /* pre-check */
    check(array != NULL, DARRAY_ENULL);

    /* copy first element */
    array_copy = darray_new(array->element_size, (size_t) array->max);
    el = darray_get(array, 0);
    el_copy = NULL;

    if (el != NULL) {
        el_copy = darray_new_element(array_copy);
        memcpy(el_copy, el, array->element_size);
        darray_set(array_copy, 0, el_copy);
    }

    /* copy the rest of the elements */
    for (i = 1; i < array->end; i++) {
        el = darray_get(array, i);
        el_copy = NULL;

        if (el != NULL) {
            memcpy(el_copy, el, array->element_size);
            darray_set(array_copy, i, el);
        }
    }

    return array_copy;
error:
    return NULL;
}

void *darray_new_element(struct darray *array)
{
    check(array->element_size > 0, DARRAY_EELSIZE);

    return calloc(1, array->element_size);
error:
    return NULL;
}

void *darray_first(struct darray *array)
{
    /* pre-check */
    check(array != NULL, DARRAY_ENULL);

    return array->contents[0];
error:
    return NULL;
}

void *darray_last(struct darray *array)
{
    /* pre-check */
    check(array != NULL, DARRAY_ENULL);

    return array->contents[array->end - 1];
error:
    return NULL;
}

void darray_set(struct darray *array, int i, void *el)
{
    /* pre-check */
    check(array != NULL, DARRAY_ENULL);
    check(i < array->max, DARRAY_EINDEXOUT);

    /* set */
    array->contents[i] = el;

    /* update end */
    if (i > array->end) {
        array->end = i;
    }

error:
    return;
}

void *darray_get(struct darray *array, int i)
{
    /* pre-check */
    check(array != NULL, DARRAY_ENULL);
    check(i < array->max, DARRAY_EINDEXOUT);

    /* get */
    return array->contents[i];
error:
    return NULL;
}

void *darray_update(struct darray* array, int i, void *el)
{
    void *old_el;

    /* pre-check */
    check(array != NULL, DARRAY_ENULL);
    check(i < array->max, DARRAY_EINDEXOUT);

    /* update */
    old_el = darray_get(array, i);
    darray_set(array, i, el);

    return old_el;
error:
    return NULL;
}

void *darray_remove(struct darray *array, int i)
{
    void *el;

    el = array->contents[i];
    array->contents[i] = NULL;

    return el;
}

static inline int darray_resize(struct darray *array, size_t new_max)
{
    int i;
    int old_max;
    void *contents;

    /* pre-check */
    check(array != NULL, DARRAY_ENULL);
    check(new_max > 0, DARRAY_ENEWMAX);

    /* calculate new max and size */
    old_max = (int) array->max;
    array->max = (int) new_max;

    /* reallocate new memory */
    contents = realloc(array->contents, new_max * sizeof(void *));
    check_mem(contents);
    array->contents = contents;

    /* initialize new memory to NULL */
    for (i = old_max; i < (int) new_max; i++) {
        array->contents[i] = NULL;
    }

    return 0;
error:
    return -1;
}

int darray_expand(struct darray *array)
{
    int res;
    size_t old_max;
    size_t new_max;

    /* pre-check */
    check(array != NULL, DARRAY_ENULL);
    check(array->max > 0, DARRAY_EEMPTY);

    /* expand */
    old_max = (size_t) array->max;
    new_max = (size_t) array->max + array->expand_rate;
    res = darray_resize(array, new_max);
    check(res == 0, DARRAY_EEXPAND, (int) new_max);
    memset(array->contents + old_max, 0, array->expand_rate + 1);

    return 0;
error:
    return -1;
}

int darray_contract(struct darray *array)
{
    int new_size;

    /* pre-check */
    check(array != NULL, DARRAY_ENULL);
    check(array->max > 0, DARRAY_EEMPTY);

    /* contract */
    if (array->end < (int) array->expand_rate) {
        new_size = (int) array->expand_rate;
    } else {
        new_size = array->end;
    }

    return darray_resize(array, (size_t) new_size + 1);
error:
    return -1;
}
