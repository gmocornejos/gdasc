#ifndef GENERIC_OBJECT_H
#define GENERIC_OBJECT_H

#include <stdlib.h>

#define GENERIC_OBJECT_HEADER(T)    \
size_t obj_size;                    \
T * (*constructor)(T * o);

typedef struct{
    GENERIC_OBJECT_HEADER(void)
} generic_obj;

void * new(void * o){
    generic_obj * obj = (generic_obj *) o;
    void * new_obj = malloc(obj -> obj_size);
    return obj -> constructor(new_obj);
}

#endif
