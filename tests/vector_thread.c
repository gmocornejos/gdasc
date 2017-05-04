#include <stdio.h>
#include <pthread.h>

#include "../gdasc_vector.h"

VECTOR_DECLARE(int, vec_int)
VECTOR_GEN_CODE(int, vec_int)

void * print(void * vector){
    vec_int * vec = vector;
    int n;

    while(vec->length < 100){
        n = 10000;
        while(--n);
        pthread_mutex_lock(&(vec->mutex));
        for(vec_int_itr i = vec->begin; i != vec->end; ++i)
            printf("%d ", *i);
        printf("\n");
        pthread_mutex_unlock(&(vec->mutex));
    }
    pthread_exit(NULL);
}

void * inc_counter(void * vector){
    vec_int * vec = vector;
    int n;

    while(vec->length < 100){
        n = 10000;
        while(--n);
        vec->append(vec, vec->length);
    }
    pthread_exit(NULL);
}

int main(int argc, char * argv[]){

    vec_int * vec = vec_int_class.constructor();
    pthread_t one, two;

    pthread_create(&one, NULL, inc_counter, vec);
    pthread_create(&two, NULL, print, vec);
    
    pthread_join(one, NULL);
    pthread_join(two, NULL);

    vec->destroy(vec);

    return 0;
}
