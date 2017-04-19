#ifndef VECTOR_H
#define VECTOR_H

#include "generic_object.h"

#define GROW_RATIO 1.3

#define VECTOR(type, name)                                          \
typedef struct name name##_t;                                       \
struct name{                                                        \
    GENERIC_OBJECT_HEADER(name##_t)                                 \
    size_t type_size;                                               \
    size_t length;                                                  \
    size_t capacity;                                                \
    type * begin;                                                   \
    type * end;                                                     \
    type * (*append) (name##_t *, type);                            \
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
type name##_pop(name##_t * self, size_t index){                   \
    if(self->length == 0)                                           \
        return NULL;                                                \
    index = index > 0 ? index : self->length + index;               \
    if(index > self->length - 1){                                    \
        fprintf(stderr, "Error in pop method, index bigger than lenght-1\n"); \
        return NULL;                                                \
    }                                                               \
// La idea es poner el dato en una variable temporal, usar memcpy para mover los datos y después restar length, como en append().
}                                                                   \
\
name##_t * name##_constructor(name##_t * v){                        \
    v -> type_size = sizeof(type);                                  \
    v -> length = 0;                                                \
    v -> capacity = 1;                                              \
    v -> begin = malloc(v->type_size * v->capacity);                \
    v -> end = v->begin;                                            \
    v -> append = name##_append;                                    \
    return v;                                                       \
}                                                                   \
\
name##_t name##_class = { sizeof(name##_t),                         \
                          name##_constructor };                     \
name##_t * name = &(name##_class);                                  \
typedef type * name##_itr;                                          \

#endif
