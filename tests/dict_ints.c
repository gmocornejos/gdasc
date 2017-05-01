#include <stdio.h>
#include "../dictionary.h"

DICTIONARY(char, int, dict_ints)

int compare_ints(char i, int j){
    return i - j;
}

void print(dict_ints_t * d){
    for(dict_ints_itr i = d->begin; i != d->end; ++i)
        printf("%c:%d ", i->key, i->value);
    printf("\n");
}

int main(int argc, char * argv[]){

    dict_ints_t * dict = dict_ints -> constructor(compare_ints);

    printf("Sets the letters of the alphabet\n");
    for(int i = 0; i < 26; ++i)
        dict->set(dict, (char)(i+65), i);
    print(dict);

    printf("Sets the value of A:35, and Y:2\n");
    dict->set(dict, 'A', 35);
    dict->set(dict, 'Y', 2);
    print(dict);

    printf("Gets the value of A: %d and W: %d\n", dict->get(dict, 'A'), dict->get(dict, 'W'));

    printf("Pops value of K:%d, L:%d, M:%d and N:%d\n", 
                            dict->pop(dict, 'K'),
                            dict->pop(dict, 'L'),
                            dict->pop(dict, 'M'),
                            dict->pop(dict, 'N'));
    print(dict);

    printf("E is at index %d\n", dict->index(dict, 'E'));

    dict_ints_entry e = dict->popindex(dict, 6);
    printf("Pop entry at index 6, k: %c, v: %d\n", e.key, e.value);
    print(dict);

    printf("Copy dictionary, clear original and set A: 5\n");
    dict_ints_t * copy = dict->copy(dict);
    dict->clear(dict);
    dict->set(dict, 'A', 5);
    print(copy);
    print(dict);

    printf("Print copy and fill original again\n");
    print(copy);
    for(int i = 0; i < 26; ++i)
        dict->set(dict, (char)(i+65), i+20);
    print(dict);

    printf("Update copy with original\n");
    copy->update(copy, dict);
    print(copy);

    dict->destroy(dict);
    copy->destroy(copy);

    return 0;
}
