#include "globals.h"
const char *saved_words[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", "macr","endmacr",".data",".string",".entry",".extern"};
const char *valid_action_name[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

void* malloc_test(size_t size){
    void *ptr = malloc(size);
    if (ptr == NULL){
        perror("error malloc failed to ellocate memory");
        exit(EXIT_FAILURE);
        
    }
    return  ptr;
}
/*compare a value to a list*/
bool find_if_key_exist_in_list(const char *list[],char *macro_key){
    int i;
    if (list == NULL || macro_key == NULL){
        return errorbreak;
    }
    for(i = 0; list[i] != NULL;i++){
        if (strcmp(list[i],macro_key)==0)
            return true;
    }
    return false;
}


