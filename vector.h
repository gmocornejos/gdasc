#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_EMPTY  4000
#define INDEX_ERROR   4001 

#include <string.h>
#include "generic_object.h"

#define GROW_RATIO 1.2

#define VECTOR(type, name)                                          \
typedef struct name name##_t;                                       \
typedef type * name##_itr;                                          \
struct name{                                                        \
    GENERIC_OBJECT_HEADER(name##_t)                                 \
    size_t type_size;                                               \
    size_t length;                                                  \
    size_t capacity;                                                \
    type * begin;                                                   \
    type * end;                                                     \
    type * (*append) (name##_t *, type);                            \
    type (*pop) (name##_t *, int);                                  \
    void (*destroy) (name##_t *);                                   \
} name##_class;                                                     \
\
type * name##_append(name##_t * self, type value){                  \
    *(self->end)++ = value;                                         \
    ++self->length;                                                 \
    if(self->length == self->capacity){                             \
        ++self->capacity;                                           \
        self->capacity *= GROW_RATIO;                               \
        self->begin = realloc(self->begin, self->type_size * self->capacity); \
        self->end = self->begin + self->length;                     \
    }                                                               \
    return self->begin;                                             \
}                                                                   \
\
type name##_pop(name##_t * self, int index){                        \
    if(self->length == 0){                                          \
        fprintf(stderr, "Error in pop method: vector empty");       \
        exit(VECTOR_EMPTY);                                         \
    }                                                               \
    index = index >= 0 ? index : self->length + index;              \
    if(index >= self->length){                                      \
        fprintf(stderr, "Error in pop method: index bigger or equal than length\n"); \
        exit(INDEX_ERROR);                                          \
    }                                                               \
    type pop_value = self->begin[index];                            \
    --self->length;                                                 \
    memmove(self->begin + index, self->begin + index+1, self->type_size * (self->length - index));                                              \
    if(self->capacity > GROW_RATIO * self->length){                 \
        self->capacity = self->length + 1;                          \
        self->begin = realloc(self->begin, self->type_size * self->capacity); \
    }                                                               \
    self->end = self->begin + self->length;                         \
    return pop_value;                                               \
}                                                                   \
\
void name##_destroy(name##_t * v){                                  \
    free(v->begin);                                                 \
    free(v);                                                        \
type name##_pop(name##_t * self, size_t index){                     \
    if(self->length == 0)                                           \
        return NULL;                                                \
    index = index > 0 ? index : self->length + index;               \
    if(index > self->length - 1){                                   \
        fprintf(stderr, "Error in pop method, index bigger than lenght-1\n"); \
        return NULL;                                                \
    }                                                               \
}                                                                   \
\
name##_t * name##_constructor(name##_t * v){                        \
    v -> type_size = sizeof(type);                                  \
    v -> length = 0;                                                \
    v -> capacity = 1;                                              \
    v -> begin = malloc(v->type_size * v->capacity);                \
    v -> end = v->begin;                                            \
    v -> append = name##_append;                                    \
    v -> pop = name##_pop;                                          \
    v -> destroy = name##_destroy;                                  \
    return v;                                                       \
}                                                                   \
\
name##_t name##_class = { sizeof(name##_t),                         \
                          name##_constructor };                     \
name##_t * name = &(name##_class);

#endif
