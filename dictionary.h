#ifndef DICTIONARY_H
#define DICTIONARY_H

#define DICT_EMPTY  5000

#include <string.h>
#include <pthread.h>
#include "generic_object.h"

#define GROW_RATIO 1.2

#define DICTIONARY(key_t, value_t, name)                            \
typedef struct{                                                     \
    key_t key;                                                      \
    value_t value;                                                  \
} name##_entry;                                                     \
typedef struct name name##_t;                                       \
typedef name##_entry * name##_itr;                                  \
struct name{                                                        \
    GENERIC_OBJECT_HEADER(name##_t)                                 \
    pthread_mutex_t mutex;                                          \
    size_t type_size;                                               \
    size_t length;                                                  \
    size_t capacity;                                                \
    name##_entry * begin;                                           \
    name##_entry * end;                                             \
    int (*cmp)(key_t, key_t);                                       \
    name##_entry * (*set)(name##_t*, key_t, value_t);               \
};                                                                  \
\
name##_t * name##_constructor(name##_t * self, ...){                \
    pthread_mutex_init(&(self->mutex), NULL);                       \
    self -> type_size = sizeof(name##_entry);                       \
    self -> length   = 0;                                           \
    self -> capacity = 1;                                           \
    self -> begin    = malloc(self->type_size * self->capacity);    \
    self -> end      = self -> begin;                               \
    self -> 
}                                                                   \
\
name##_t name##_class = { sizeof(name##_t),                         \
                          name##_constructor };                     \
name##_t * name = &(name##_class);
#endif
