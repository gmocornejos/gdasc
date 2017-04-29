#ifndef DICTIONARY_H
#define DICTIONARY_H

#define DICT_EMPTY  5000

#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define GROW_RATIO 1.2

#define DICTIONARY(key_t, value_t, name)                            \
typedef struct{                                                     \
    key_t key;                                                      \
    value_t value;                                                  \
} name##_entry;                                                     \
typedef struct name name##_t;                                       \
typedef name##_entry * name##_itr;                                  \
struct name{                                                        \
    name##_t * (*constructor)(int (*cmp)(key_t, value_t));          \
    pthread_mutex_t mutex;                                          \
    size_t type_size;                                               \
    size_t length;                                                  \
    size_t capacity;                                                \
    name##_entry * begin;                                           \
    name##_entry * end;                                             \
    int (*cmp)(key_t, value_t);                                     \
    name##_entry * (*set)(name##_t *, key_t, value_t);              \
};                                                                  \
\
name##_entry * name##_set(name##_t * self , key_t k, value_t v){    \
    pthread_mutex_lock(&(self->mutex));                             \
    for(name##_itr i = self->begin; i != self->end; ++i){           \
        if(self->cmp(i->key, k) == 0){                              \
            i->value = v;                                           \
            pthread_mutex_unlock(&(self->mutex));                   \
            return self->begin;                                     \
        }                                                           \
    }                                                               \
# Quedé Aquí!!! 
    pthread_mutex_unlock(&(self->mutex));                           \
    return self->begin;                                             \
}                                                                   \
\
name##_t * name##_constructor(int (*cmp)(key_t, value_t)){          \
    name##_t * self = malloc(sizeof(name##_t));                     \
    pthread_mutex_init(&(self->mutex), NULL);                       \
    self -> type_size = sizeof(name##_entry);                       \
    self -> length   = 0;                                           \
    self -> capacity = 1;                                           \
    self -> begin    = malloc(self->type_size * self->capacity);    \
    self -> end      = self -> begin;                               \
    self -> cmp      = cmp;                                         \
    self -> set      = name##_set;                                  \
    return self;                                                    \
}                                                                   \
\
name##_t name##_class = { name##_constructor };                     \
name##_t * name = &(name##_class);
#endif
