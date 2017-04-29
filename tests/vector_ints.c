#include <stdio.h>
#include "../vector.h"

VECTOR(int, vector_int)

void append_elements(vector_int_t * v, int n){
    for(; n != 0; --n)
        v->append(v, n);
}

void pop_elements(vector_int_t * v, int n){
    for(; n != 0; --n)
        v->pop(v, -1);
}

void print(vector_int_t * v){
   for(vector_int_itr i = v->begin; i != v->end; ++i)
        printf("%d ", *i);
    printf("\n");
}

int cmp_int(int a, int b){
    return a-b;
}

int main(int argc, char * argv[]){

    vector_int_t * vec = vector_int -> constructor();

    printf("Adds 100 elements and prints\n"); 
    append_elements(vec, 100);
    print(vec);

    printf("Removes 50 elemets and prints\n");
    pop_elements(vec, 50);
    print(vec);

    printf("Adds 100 elements, clear() and prints\n");
    append_elements(vec, 100);
    vec->clear(vec);
    print(vec);

    printf("Adds 100 elements, copy(), adds 50 elements to copy and prints original and copy\n");
    append_elements(vec, 100);
    vector_int_t * copy = vec->copy(vec);
    append_elements(copy, 50);
    print(vec);
    print(copy);

    printf("How many 48 in copy: %d\n", copy->count(copy, 48, cmp_int));

    printf("Extend copy with original\n");
    copy->extend(copy, vec);
    print(copy);

    printf("First occurence of 67 in copy %d\n", copy->index(copy, 67, cmp_int));

    printf("Insert 33 at index 33 of copy, ie: between 67 and 68\n");
    copy->insert(copy, 33, 33);
    print(copy);
    printf("Insert 100 at index -10\n");
    copy->insert(copy, 100, -10);
    print(copy);

    printf("remove 30 elements from copy\n");
    pop_elements(copy, 30);
    print(copy);

    printf("Remove first occurence of value 43\n");
    copy->remove(copy, 43, cmp_int);
    print(copy);

    printf("Adds 100 elements, removes 75 and prints\n");
    append_elements(copy, 100);
    pop_elements(copy, 75);
    print(copy);

    vec->destroy(vec);
    return 0;
}
