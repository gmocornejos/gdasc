#include <stdio.h>
#include "../dictionary.h"

DICTIONARY(char, int, dict_ints)

int compare_ints(char i, int j){
    return i - j;
}

int main(int argc, char * argv[]){

    dict_ints_t * dict = dict_ints -> constructor(compare_ints);
    for(int i = 0; i < 26; ++i)
        dict->set(dict, (char)(i+65), i);

    for(dict_ints_itr i = dict->begin; i != dict->end; ++i)
        printf("%c:%d ", i->key, i->value);
    printf("\n");

    return 0;
}
