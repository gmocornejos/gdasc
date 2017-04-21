#ifndef VECTOR_THREAD_H
#define VECTOR_THREAD_H

#define VECTOR_EMPTY  4000
#define INDEX_ERROR   4001 

#include <string.h>
#include <pthread.h>
#include "generic_object.h"

#define GROW_RATIO 1.2

#define VECTOR_THREAD(type, name)                                   \
typedef struct name name##_t;                                       \
typedef type * name##_itr;                                          \
struct name{                                                        \
    GENERIC_OBJECT_HEADER(name##_t)                                 \
    pthread_mutex_t mutex;                                          \
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
    pthread_mutex_lock(&(self->mutex));                             \
    *(self->end)++ = value;                                         \
    ++self->length;                                                 \
    if(self->length == self->capacity){                             \
        ++self->capacity;                                           \
        self->capacity *= GROW_RATIO;                               \
        self->begin = realloc(self->begin, self->type_size * self->capacity); \
        self->end = self->begin + self->length;                     \
    }                                                               \
    pthread_mutex_unlock(&(self->mutex));                           \
    return self->begin;                                             \
}                                                                   \
\
type name##_pop(name##_t * self, int index){                        \
    pthread_mutex_lock(&(self->mutex));                             \
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
    pthread_mutex_unlock(&(self->mutex));                           \
}                                                                   \
\
void name##_destroy(name##_t * self){                               \
    pthread_mutex_lock(&(self->mutex));                             \
    pthread_mutex_destroy(&(self->mutex));                          \
    free(self->begin);                                              \
    free(self);                                                     \
}                                                                   \
\
name##_t * name##_constructor(name##_t * self){                     \
    pthread_mutex_init(&(self->mutex), NULL);                       \
    self -> type_size = sizeof(type);                               \
    self -> length = 0;                                             \
    self -> capacity = 1;                                           \
    self -> begin = malloc(self->type_size * self->capacity);       \
    self -> end = self->begin;                                      \
    self -> append = name##_append;                                 \
    self -> pop = name##_pop;                                       \
    self -> destroy = name##_destroy;                               \
    return self;                                                    \
}                                                                   \
\
name##_t name##_class = { sizeof(name##_t),                         \
                          name##_constructor };                     \
name##_t * name = &(name##_class);

#endif
