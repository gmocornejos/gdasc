#include <stdio.h>
#include "vector.h"

VECTOR(int, vector_int)

int main(int argc, char * argv[]){

    vector_int_t * my_vec = new(vector_int);
    for(int i = 0; i < 100; ++i)
        my_vec->append(my_vec, i);

    printf("%d\n", my_vec->pop(my_vec, -1));
    printf("%d\n", my_vec->pop(my_vec, 50));

    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));
    printf("%d\n", my_vec->pop(my_vec, 0));

    printf("%d\n", my_vec->pop(my_vec, -3));

    for(vector_int_itr i = my_vec->begin; i != my_vec->end; ++i)
        printf("%d ", *i);
    printf("\n");
    
    my_vec->destroy(my_vec);

    return 0;
}
