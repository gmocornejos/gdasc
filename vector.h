#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_EMPTY    4000
#define VEC_INDEX_ERROR 4001 

#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define VEC_GROW_RATIO 1.2

#define VECTOR_DECLARE(type, name)                                  \
typedef struct name name;                                           \
typedef type * name##_itr;                                          \
struct name{                                                        \
    pthread_mutex_t mutex;                                          \
    size_t type_size;                                               \
    size_t length;                                                  \
    size_t capacity;                                                \
    type * begin;                                                   \
    type * end;                                                     \
    name * (*constructor) ();                                       \
    type * (*append)  (name *, type);                               \
    type   (*pop)     (name *, int);                                \
    type * (*clear)   (name *);                                     \
    name * (*copy)    (name *);                                     \
    int    (*count)   (name *, type, int (*cmp)(type, type));       \
    type * (*extend)  (name *, name *);                             \
    int    (*index)   (name * , type, int (*cmp)(type, type));      \
    type * (*insert)  (name *, type, int);                          \
    type * (*remove)  (name *, type, int (*cmp)(type, type));       \
    void   (*destroy) (name *);                                     \
};                                                                  \
extern name name##_class;                                                


#define VECTOR_GEN_CODE(type, name)                                 \
type * name##_append(name * self, type value){                      \
    pthread_mutex_lock(&(self->mutex));                             \
    *(self->end)++ = value;                                         \
    if(++self->length == self->capacity){                           \
        ++self->capacity;                                           \
        self->capacity *= VEC_GROW_RATIO;                           \
        self->begin = realloc(self->begin, self->type_size * self->capacity); \
        self->end = self->begin + self->length;                     \
    }                                                               \
    pthread_mutex_unlock(&(self->mutex));                           \
    return self->begin;                                             \
}                                                                   \
\
type name##_pop(name * self, int index){                            \
    pthread_mutex_lock(&(self->mutex));                             \
    if(self->length == 0){                                          \
        fprintf(stderr, "Error in pop method: vector empty");       \
        exit(VECTOR_EMPTY);                                         \
    }                                                               \
    index = index >= 0 ? index : self->length + index;              \
    if(index >= self->length || index < 0){                         \
        fprintf(stderr, "Error in pop method: index bigger or equal than length\n"); \
        exit(VEC_INDEX_ERROR);                                      \
    }                                                               \
    type pop_value = self->begin[index];                            \
    --self->length;                                                 \
    memmove(self->begin + index, self->begin + index+1, self->type_size * (self->length - index));                                              \
    if(self->capacity > VEC_GROW_RATIO * self->length){             \
        self->capacity = self->length + 1;                          \
        self->begin = realloc(self->begin, self->type_size * self->capacity); \
    }                                                               \
    self->end = self->begin + self->length;                         \
    pthread_mutex_unlock(&(self->mutex));                           \
    return pop_value;                                               \
}                                                                   \
\
type * name##_clear(name * self){                                   \
    pthread_mutex_lock(&(self->mutex));                             \
    self->length = 0;                                               \
    self->capacity = 1;                                             \
    self->begin = realloc(self->begin, self->type_size * self->capacity); \
    self->end = self->begin;                                        \
    pthread_mutex_unlock(&(self->mutex));                           \
    return self->begin;                                             \
}                                                                   \
\
name * name##_copy(name * self){                                    \
    pthread_mutex_lock(&(self->mutex));                             \
    name * copy = malloc(sizeof(name));                             \
    *copy = *self;                                                  \
    pthread_mutex_init(&(copy->mutex), NULL);                       \
    copy->begin = malloc(self->type_size * self->capacity);         \
    copy->end = copy->begin + copy->length;                         \
    memcpy(copy->begin, self->begin, self->length * self->type_size); \
    pthread_mutex_unlock(&(self->mutex));                           \
    return copy;                                                    \
}                                                                   \
\
int name##_count(name * self, type value, int (*cmp)(type, type)){  \
    pthread_mutex_lock(&(self->mutex));                             \
    int count = 0;                                                  \
    for(name##_itr i = self->begin; i != self->end; ++i)            \
        if(cmp(value, *i) == 0)                                     \
            ++count;                                                \
    pthread_mutex_unlock(&(self->mutex));                           \
    return count;                                                   \
}                                                                   \
\
type * name##_extend(name * self, name * other){                    \
    pthread_mutex_lock(&(self->mutex));                             \
    pthread_mutex_lock(&(other->mutex));                            \
    self->length += other->length;                                  \
    self->capacity += other->length;                                \
    self->begin = realloc(self->begin, self->capacity * self->type_size); \
    memcpy(self->end, other->begin, other->length * other->type_size); \
    self->end = self->begin + self->length;                         \
    pthread_mutex_unlock(&(other->mutex));                          \
    pthread_mutex_unlock(&(self->mutex));                           \
    return self->begin;                                             \
}                                                                   \
\
int name##_index(name * self, type value, int (*cmp)(type, type)){  \
    pthread_mutex_lock(&(self->mutex));                             \
    int r_value = -1;                                               \
    for(int i = 0; i < self->length; ++i)                           \
        if(cmp(value, self->begin[i]) == 0){                        \
            r_value = i;                                            \
            break;                                                  \
        }                                                           \
    pthread_mutex_unlock(&(self->mutex));                           \
    return r_value;                                                 \
}                                                                   \
\
type * name##_insert(name * self, type value, int index){           \
    pthread_mutex_lock(&(self->mutex));                             \
    index = index >= 0 ? index : self->length + index;              \
    if(index >= self->length || index < 0){                         \
        fprintf(stderr, "Error in pop method: index bigger or equal than length\n"); \
        exit(VEC_INDEX_ERROR);                                      \
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
    pthread_mutex_unlock(&(self->mutex));                           \
    return self->begin;                                             \
}                                                                   \
\
type * name##_remove(name * self, type value, int (*cmp)(type, type)){ \
    pthread_mutex_lock(&(self->mutex));                             \
    for(int i = 0; i < self->length; ++i)                           \
        if(cmp(value, self->begin[i]) == 0){                        \
            memmove(self->begin+i, self->begin+i+1, self->type_size * (self->length - i));                                                      \
            --self->length;                                         \
            break;                                                  \
        }                                                           \
    if(self->capacity > VEC_GROW_RATIO * self->length){             \
        self->capacity = self->length + 1;                          \
        self->begin = realloc(self->begin, self->type_size * self->capacity); \
    }                                                               \
    self->end = self->begin + self->length;                         \
    pthread_mutex_unlock(&(self->mutex));                           \
    return self->begin;                                             \
}                                                                   \
\
void name##_destroy(name * self){                                   \
    pthread_mutex_lock(&(self->mutex));                             \
    free(self->begin);                                              \
    pthread_mutex_destroy(&(self->mutex));                          \
    free(self);                                                     \
}                                                                   \
\
name * name##_constructor(){                                        \
    name * self   = malloc(sizeof(name));                           \
    *self         = name##_class;                                   \
    self -> begin = malloc(self->type_size * self->capacity);       \
    self -> end   = self->begin;                                    \
    return self;                                                    \
}                                                                   \
\
name name##_class = { PTHREAD_MUTEX_INITIALIZER,                    \
                      sizeof(type),                                 \
                      0,                                            \
                      1,                                            \
                      NULL,                                         \
                      NULL,                                         \
                      name##_constructor,                           \
                      name##_append,                                \
                      name##_pop,                                   \
                      name##_clear,                                 \
                      name##_copy,                                  \
                      name##_count,                                 \
                      name##_extend,                                \
                      name##_index,                                 \
                      name##_insert,                                \
                      name##_remove,                                \
                      name##_destroy                                \
                    };
#endif
