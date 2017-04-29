#include <stdio.h>
#include "../dictionary.h"

DICTIONARY(char, int, dict_ints)

int compare_ints(char i, int j){
    return i - j;
}

int main(int argc, char * argv[]){

    dict_ints_t * dict = dict_ints -> constructor(compare_ints);
    dict->set(dict, 'A', 2);

    return 0;
}
