#include <stdio.h>
#include "../gdasc_dictionary.h"

DICTIONARY_DECLARE(char, int, dict_char_int)
DICTIONARY_GEN_CODE(char, int, dict_char_int)

int compare_ints(char i, char j){
    return i - j;
}

void print(dict_char_int * d){
    for(dict_char_int_itr i = d->begin; i != d->end; ++i)
        printf("%c:%d ", i->key, i->value);
    printf("\n");
}

int main(int argc, char * argv[]){

    dict_char_int * dict = dict_char_int_class.constructor(compare_ints);

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

    dict_char_int_entry e = dict->popindex(dict, 6);
    printf("Pop entry at index 6, k: %c, v: %d\n", e.key, e.value);
    print(dict);

    printf("Copy dictionary, clear original and set A: 5\n");
    dict_char_int * copy = dict->copy(dict);
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
