#ifndef DICTIONARY_H
#define DICTIONARY_H

#define DICT_EMPTY  5000
#define NO_KEY      5001
#define INDEX_ERROR 5003

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
    size_t entry_size;                                              \
    size_t length;                                                  \
    size_t capacity;                                                \
    name##_entry * begin;                                           \
    name##_entry * end;                                             \
    int (*cmp) (key_t, value_t);                                    \
    name##_entry * (*set) (name##_t *, key_t, value_t);             \
    value_t (*get) (name##_t *, key_t);                             \
    value_t (*pop) (name##_t *, key_t);                             \
    int (*index) (name##_t *, key_t);                               \
    name##_entry (*popindex) (name##_t *, int);                     \
    name##_entry * (*clear) (name##_t *);                           \
    name##_t * (*copy) (name##_t *);                                \
    name##_entry * (*update) (name##_t *, name##_t *);              \
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
    self->end -> key   = k;                                            \
    self->end -> value = v;                                            \
    self->end++;                                                    \
    if(++self->length == self->capacity){                           \
        ++self->capacity;                                           \
        self->capacity *= GROW_RATIO;                               \
        self->begin = realloc(self->begin, self->entry_size * self->capacity);\
        self->end = self->begin + self->length;                     \
    }                                                               \
    pthread_mutex_unlock(&(self->mutex));                           \
    return self->begin;                                             \
}                                                                   \
\
value_t  name##_get(name##_t * self, key_t k){                      \
    pthread_mutex_lock(&(self->mutex));                             \
    if(self->length == 0){                                          \
        fprintf(stderr, "Error in get method: dictionary empty");   \
        exit(DICT_EMPTY);                                           \
    }                                                               \
    for(name##_itr i = self->begin; i != self->end; ++i){           \
        if(self->cmp(i->key, k) == 0){                              \
            pthread_mutex_unlock(&(self->mutex));                   \
            return i->value;                                        \
        }                                                           \
    }                                                               \
    fprintf(stderr, "Error in get method: requested non-existent key\n");\
    exit(NO_KEY);                                                   \
}                                                                   \
\
value_t name##_pop(name##_t * self, key_t k){                       \
    pthread_mutex_lock(&(self->mutex));                             \
    if(self->length == 0){                                          \
        fprintf(stderr, "Error in pop method: dictionary empty");   \
        exit(DICT_EMPTY);                                           \
    }                                                               \
    for(int i = 0; i != self->length; ++i){                         \
        if(self->cmp(self->begin[i].key, k) == 0){                  \
            value_t v = self->begin[i].value;                       \
            --self->length;                                         \
            memmove(self->begin + i, self->begin + i+1, self->entry_size * (self->length - i));\
            if(self->capacity > GROW_RATIO * self->length){         \
                self->capacity = self->length + 1;                  \
                self->begin = realloc(self->begin, self->entry_size * self->capacity);\
            }                                                       \
            self->end = self->begin + self->length;                 \
            pthread_mutex_unlock(&(self->mutex));                   \
            return v;                                        \
        }                                                           \
    }                                                               \
    fprintf(stderr, "Error in pop method: requested non-existent key\n");\
    exit(NO_KEY);                                                   \
}                                                                   \
\
int name##_index(name##_t * self, key_t k){                         \
    pthread_mutex_lock(&(self->mutex));                             \
    for(int i = 0; i != self->length; ++i){                         \
        if(self->cmp(self->begin[i].key, k) == 0){                  \
            pthread_mutex_unlock(&(self->mutex));                   \
            return i;                                               \
        }                                                           \
    }                                                               \
    pthread_mutex_unlock(&(self->mutex));                           \
    return -1;                                                      \
}                                                                   \
\
name##_entry name##_popindex(name##_t * self, int index){           \
    pthread_mutex_lock(&(self->mutex));                             \
    if(self->length == 0){                                          \
        fprintf(stderr, "Error in popindex method: dictionary empty");\
        exit(DICT_EMPTY);                                           \
    }                                                               \
    index = index >= 0 ? index : self->length + index;              \
    if(index >= self->length || index < 0){                         \
        fprintf(stderr, "Error in popindex method: index bigger or equal than length\n");\
        exit(INDEX_ERROR);                                          \
    }                                                               \
    name##_entry e = self->begin[index];                            \
    --self->length;                                                 \
    memmove(self->begin + index, self->begin + index+1, self->entry_size * (self->length - index));\
    if(self->capacity > GROW_RATIO * self->length){                 \
        self->capacity = self->length + 1;                          \
        self->begin = realloc(self->begin, self->entry_size * self->capacity); \
    }                                                               \
    self->end = self->begin + self->length;                         \
    pthread_mutex_unlock(&(self->mutex));                           \
    return e;                                                       \
}                                                                   \
\
name##_entry * name##_clear(name##_t * self){                       \
    pthread_mutex_lock(&(self->mutex));                             \
    self->length = 0;                                               \
    self->capacity = 1;                                             \
    self->begin = realloc(self->begin, self->entry_size * self->capacity);\
    self->end = self->begin;                                        \
    pthread_mutex_unlock(&(self->mutex));                           \
    return self->begin;                                             \
}                                                                   \
\
name##_t * name##_copy(name##_t * self){                            \
    pthread_mutex_lock(&(self->mutex));                             \
    name##_t * copy = malloc(sizeof(name##_t));                     \
    *copy = *self;                                                  \
    pthread_mutex_init(&(copy->mutex), NULL);                       \
    copy->begin = malloc(self->entry_size * self->capacity);        \
    copy->end = copy->begin + copy->length;                         \
    memcpy(copy->begin, self->begin, self->length * self->entry_size);\
    pthread_mutex_unlock(&(self->mutex));                           \
    return copy;                                                    \
}                                                                   \
\
name##_entry * name##_update(name##_t * self, name##_t * other){    \
    pthread_mutex_lock(&(self->mutex));                             \
    pthread_mutex_lock(&(other->mutex));                            \
    name##_itr s;                                                   \
    for(name##_itr o = other->begin; o != other->end; ++o){         \
        for(s = self->begin; s != self->end; ++s)                   \
            if(self->cmp(o->key, s->key) == 0){                     \
                s->value = o->value;                                \
                break;                                              \
            }                                                       \
        if(s == self->end){                                         \
            self->end -> key   = o->key;                            \
            self->end -> value = o->value;                          \
            self->end++;                                            \
            if(++self->length == self->capacity){                   \
                ++self->capacity;                                   \
                self->capacity *= GROW_RATIO;                       \
                self->begin = realloc(self->begin, self->entry_size * self->capacity);\
                self->end = self->begin + self->length;             \
            }                                                       \
        }                                                           \
    }                                                               \
    pthread_mutex_unlock(&(other->mutex));                          \
    pthread_mutex_unlock(&(self->mutex));                           \
    return self->begin;                                             \
}\
\
name##_t * name##_constructor(int (*cmp)(key_t, value_t)){          \
    name##_t * self = malloc(sizeof(name##_t));                     \
    pthread_mutex_init(&(self->mutex), NULL);                       \
    self -> entry_size = sizeof(name##_entry);                      \
    self -> length   = 0;                                           \
    self -> capacity = 1;                                           \
    self -> begin    = malloc(self->entry_size * self->capacity);   \
    self -> end      = self -> begin;                               \
    self -> cmp      = cmp;                                         \
    self -> set      = name##_set;                                  \
    self -> get      = name##_get;                                  \
    self -> pop      = name##_pop;                                  \
    self -> index    = name##_index;                                \
    self -> popindex = name##_popindex;                             \
    self -> clear    = name##_clear;                                \
    self -> copy     = name##_copy;                                 \
    self -> update   = name##_update;                               \
    return self;                                                    \
}                                                                   \
\
name##_t name##_class = { name##_constructor };                     \
name##_t * name = &(name##_class);
#endif
