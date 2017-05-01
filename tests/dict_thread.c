#include <stdio.h>

#include "../dictionary.h"

DICTIONARY(char, int, d_char_int)

void * print(void * d){
    d_char_int_t * dict = d;
    int n;

    while(dict->length < 256){
        n = 10000;
        while(--n);
        pthread_mutex_lock(&(dict->mutex));
        for(d_char_int_itr i = dict->begin; i != dict->end; ++i)
            printf("%c: %d, ", i->key, i->value);
        printf("\n");
        pthread_mutex_unlock(&(dict->mutex));
    }
    pthread_exit(NULL);
}

void * fill_dict(void * d){
    d_char_int_t * dict = d;
    int n;

    while(dict->length < 256){
        n = 10000;
        while(--n);
        dict->set(dict, (char)dict->length, (int)dict->length);
    }
    pthread_exit(NULL);
}

int cmp_chars(char a, char b){
    return a-b;
}

int main(int argc, char * argv[]){

    d_char_int_t * dict = d_char_int->constructor(cmp_chars);
    pthread_t one, two;

    pthread_create(&one, NULL, fill_dict, dict);
    pthread_create(&two, NULL, print, dict);

    pthread_join(one, NULL);
    pthread_join(two, NULL);

    dict->destroy(dict);

    return 0;
}
