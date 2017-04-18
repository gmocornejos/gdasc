#include <stdio.h>
#include "vector.h"

VECTOR(int, vector_int)

int main(int argc, char * argv[]){

    vector_int_t * my_vec = new(vector_int);
    printf("Size of int %d, size of vector_int -> type_size %d\n", sizeof(int), my_vec -> type_size);

    return 0;
}
