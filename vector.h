#ifndef VECTOR_H
#define VECTOR_H

#include "generic_object.h"

#define VECTOR(type, name)                                          \
typedef struct name name ## _t;                                     \
struct name{                                                        \
    GENERIC_OBJECT_HEADER(name ## _t)                               \
    size_t type_size;                                               \
} name ## _class;                                                   \
\
name ## _t * name ## _constructor(name ## _t * o){                  \
    o -> type_size = sizeof(type);                                  \
    return o;                                                       \
}                                                                   \
\
name ## _t name ## _class = { sizeof(name ## _t),                   \
                              name ## _constructor,                 \
                              sizeof(type) };                       \
name ## _t * name = &(name ## _class);

#endif
