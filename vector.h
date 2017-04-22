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
    type * (*clear) (name##_t *);                                   \
    name##_t * (*copy) (name##_t *);                                \
    int (*count) (name##_t *, type, int (*cmp)(type, type));        \
    type * (*extend) (name##_t *, name##_t *);                      \
    int (*index) (name##_t * , type, int (*cmp)(type, type));       \
    type * (*insert) (name##_t *, type, int);                       \
    type * (*remove) (name##_t *, type, int (*cmp)(type, type));    \
    void (*destroy) (name##_t *);                                   \
} name##_class;                                                     \
\
type * name##_append(name##_t * self, type value){                  \
    *(self->end)++ = value;                                         \
    if(++self->length == self->capacity){                             \
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
type * name##_clear(name##_t * self){                               \
    self->length = 0;                                             \
    self->capacity = 1;                                           \
    self->begin = realloc(self->begin, self->type_size * self->capacity); \
    self->end = self->begin;                                        \
    return self->begin;                                             \
}                                                                   \
\
name##_t * name##_copy(name##_t * self){                            \
    name##_t * copy = malloc(sizeof(name##_t));                     \
    *copy = *self;                                                  \
    copy->begin = malloc(self->type_size * self->capacity);         \
    copy->end = copy->begin + copy->length;                         \
    memcpy(copy->begin, self->begin, self->length * self->type_size); \
    return copy;                                                    \
}                                                                   \
\
int name##_count(name##_t * self, type value, int (*cmp)(type, type)){ \
    int count = 0;                                                  \
    for(name##_itr i = self->begin; i != self->end; ++i)            \
        if(cmp(value, *i) == 0)                                      \
            ++count;                                                \
    return count;                                                   \
}                                                                   \
\
type * name##_extend(name##_t * self, name##_t * other){            \
    self->length += other->length;                                  \
    self->capacity += other->length;                                \
    self->begin = realloc(self->begin, self->capacity * self->type_size); \
    memcpy(self->end, other->begin, other->length * self->type_size); \
    self->end = self->begin + self->length;                         \
    return self->begin;                                             \
}                                                                   \
\
int name##_index(name##_t * self, type value, int (*cmp)(type, type)){ \
    for(int i = 0; i < self->length; ++i)                           \
        if(cmp(value, self->begin[i]) == 0)                         \
            return i;                                               \
    return -1;                                                      \
}                                                                   \
\
type * name##_insert(name##_t * self, type value, int index){        \
    index = index >= 0 ? index : self->length + index;              \
    if(index >= self->length){                                      \
        fprintf(stderr, "Error in pop method: index bigger or equal than length\n"); \
        exit(INDEX_ERROR);                                          \
    }                                                               \
    if(self->length+1 == self->capacity){                           \
        ++self->capacity;                                           \
        self->capacity *= GROW_RATIO;                               \
        self->begin = realloc(self->begin, self->capacity * self->type_size); \
    }                                                               \
    memmove(self->begin+index+1, self->begin+index, self->type_size * (self->length - index));                                                  \
    self->begin[index] = value;                                     \
    ++self->length;                                                 \
    self->end = self->begin + self->length;                         \
    return self->begin;                                             \
}                                                                   \
\
type * name##_remove(name##_t * self, type value, int (*cmp)(type, type)){ \
    for(int i = 0; i < self->length; ++i)                           \
        if(cmp(value, self->begin[i]) == 0){                        \
            memmove(self->begin+i, self->begin+i+1, self->type_size * (self->length - i));                                                      \
            --self->length;                                         \
            break;                                                  \
        }                                                           \
    if(self->capacity > GROW_RATIO * self->length){                 \
        self->capacity = self->length + 1;                          \
        self->begin = realloc(self->begin, self->type_size * self->capacity); \
        self->end = self->begin + self->length;                     \
    }                                                               \
    return self->begin;                                             \
}                                                                   \
\
void name##_destroy(name##_t * self){                               \
    free(self->begin);                                              \
    free(self);                                                     \
}                                                                   \
\
name##_t * name##_constructor(name##_t * self){                     \
    self -> type_size = sizeof(type);                               \
    self -> length   = 0;                                           \
    self -> capacity = 1;                                           \
    self -> begin    = malloc(self->type_size * self->capacity);    \
    self -> end      = self->begin;                                 \
    self -> append   = name##_append;                               \
    self -> pop      = name##_pop;                                  \
    self -> clear    = name##_clear;                                \
    self -> copy     = name##_copy;                                 \
    self -> count    = name##_count;                                \
    self -> extend   = name##_extend;                               \
    self -> index    = name##_index;                                \
    self -> insert   = name##_insert;                               \
    self -> remove   = name##_remove;                               \
    self -> destroy  = name##_destroy;                              \
    return self;                                                    \
}                                                                   \
\
name##_t name##_class = { sizeof(name##_t),                         \
                          name##_constructor };                     \
name##_t * name = &(name##_class);

#endif
