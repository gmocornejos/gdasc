#include <stdio.h>
#include <pthread.h>

#include "../vector_thread.h"

VECTOR_THREAD(int, vec_int)

int global_counter = 0;

void * inc_counter(void * vector){
    vec_int_t * vec = vector; 

    while(global_counter < 100)
        vec->append(vec, global_counter++);    
    pthread_exit(NULL);
}

int main(int argc, char * argv[]){

    vec_int_t * vec = new(vec_int);
    pthread_t one, two;

    pthread_create(&one, NULL, inc_counter, vec);
    pthread_create(&two, NULL, inc_counter, vec);
    
    pthread_join(one, NULL);
    pthread_join(two, NULL);

    pthread_mutex_lock(&(vec->mutex));
    for(vec_int_itr i = vec->begin; i != vec->end; ++i)
        printf("%d ", *i);
    pthread_mutex_unlock(&(vec->mutex));

    vec->destroy(vec);

    return 0;
}
