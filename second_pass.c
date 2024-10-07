
#include "second_pass.h"
/*skeleton of the binary conversion still needs to think about the logic of how we get the numbers to make it function correctly */
short int generate_label_adress_in_binary(int word) {
    short int bvalue = 0;
    short int max_value = 0xFFF;
    int i;
    if (word < 0){
        word = (max_value +1)+word;
    }
    word &= max_value;
    for (i = 0; i < 12; i++) {
        if (word & 1) {  
            bvalue |= (1 << i);  
        }
        word >>= 1; 
    }

    return bvalue;
}


int get_register_number(char *token){
    char* ptr= token;
     while(!isdigit(*ptr))
        ptr++;
     
    
    return atoi(ptr);
    
}

/*get a token and a flag to tell it if its a source or a destination and bitwise the value into the word*/
short int handle_extra_word(char *token, Hash_table label_table,int source_flag){
    short int extra_word =0;
    short int label_handl =0;
    bool label_flag = false;
    int register_number =0;
    int number;
    
    if (is_valid_register_indirect(token)||is_valid_register(token)) {
        register_number = get_register_number(token);
        if(source_flag)
            extra_word |= (register_number << 6);
        else 
            extra_word |= (register_number << 3);
    } 

    
    else if (line_type_test(token, NUMBER_SIGN)) {
        number = atoi(token +1);
        
        extra_word |= generate_label_adress_in_binary(number) << 3;
        
    } 
    
    else{
        label_handl = get_label_location(label_table,token);
        extra_word |= generate_label_adress_in_binary(label_handl) <<3;
        label_flag =true;
    }
    if (label_flag == true){     
        if(check_label_not_extern(label_table,token)){
            extra_word |= RELOCATABLE;
            return extra_word;
        }

    }

    if  (check_label_not_extern(label_table,token))
        extra_word |= ABSOLUTE;
    
        
    
    return extra_word;

}
/*save */
void if_extern_save_word_and_location(Hash_table label_table,char* word,int *counter ,Hash_table extern_table ){
    int* counter_copy;
    if (!check_label_not_extern(label_table,word)){
        counter_copy = (int*)malloc_test(sizeof(int));
        *counter_copy = *counter;
        insert(extern_table,word,counter_copy);
    }
}

/*in my algorthitem apart from the first word any other word in action instruction is handeled in the 2nd pass */
ERROR_ID handle_action_word_seconed_pass(char *word,const char *line_ptr, int *counter, Hash_table label_table,Hash_table extern_table) {
    
    ERROR_ID error_num = NO_ERRORS;
    char *token;
    char *source = NULL;
    char *destination = NULL;
    int register_flag =0;
    int L =0;
    short int extra_words_holder = 0;
    char* line =(char*)line_ptr;
    (*counter)++; /*skip the first word that was handled by the first pass*/

    token = strtok(line, TOK);
    if (token != NULL) {
        if_extern_save_word_and_location(label_table,token,counter,extern_table);
        source = token;
        L++;
        if (is_valid_register(token)||is_valid_register_indirect(token))
            register_flag +=1;
        token = strtok(NULL, TOK);
    }
    
    
    if (token != NULL) {
        if_extern_save_word_and_location(label_table,token,counter,extern_table);
        destination = token;
        L++;
        if (is_valid_register(token) || is_valid_register_indirect(token))
            register_flag += 1;
        token = strtok(NULL, TOK);
        if (token != NULL)
            L++;
    }

    if (destination == NULL && source != NULL){
        destination = source;
        source = NULL;
    }
    
    
    /*handle the action words in case of 2 registers combine them into one word*/
    if (source != NULL) {
        extra_words_holder = handle_extra_word(source, label_table, true);
        if (register_flag == 2){
            extra_words_holder |= (get_register_number(destination) << 3);
        }
        if(extra_words_holder == 0)
            extra_words_holder = E;
        
        code_array[*counter] = extra_words_holder;
        (*counter)++;
        extra_words_holder =0;
        }

    if (destination != NULL && (source == NULL || register_flag != 2)) {
        extra_words_holder = handle_extra_word(destination, label_table, false);
        if(extra_words_holder == 0)
            extra_words_holder = E; 
        code_array[*counter] = extra_words_holder;
        (*counter)++;
    }
  


    return error_num;
}

/*used to validate that all labels that were reported in .entry are found in the code*/
ERROR_ID no_entry_errors(Hash_table labe_table, char * token){
    label* result;
    result = search(labe_table,token);
    if(get_location(result) == NO_LOCATION)
        return ENTRY_IMPLAMENT_E;
    else
        return NO_ERRORS;
}

/*in this algorithem we validate the entries in the 2nd pass but they were inserted into the label_table in the first pass*/
ERROR_ID handle_second_pass_entry(char *line,Hash_table label_table,error_handler error_return){
    const char *line_ptr = line;
    char *word = get_next_word_in_line(&line_ptr);
    ERROR_ID return_value = NO_ERRORS;
    int flag = false;
    /*skip all words before entry*/
    while (word != NULL){
        if (strcmp(word,".entry") == 0){
            free(word);
            word = get_next_word_in_line(&line_ptr);
            break;
        }
        free(word);
        word = get_next_word_in_line(&line_ptr);
    }
    while(word !=NULL){
        return_value = no_entry_errors(label_table,word);
        if(return_value != NO_ERRORS){
            error_return.error_num = return_value;
            report_error_with_location(error_return);
            flag = true;
        }
        free(word);
        word = get_next_word_in_line(&line_ptr);
    }
    if (word != NULL)
        free(word);
    if(flag == true)
        return ENTRY_IMPLAMENT_E;
    
    return return_value;
}



/*the main function of this file*/
bool deploy_second_pass(char* file_name,Hash_table label_table,Hash_table macro_table,Hash_table extern_table){
    char line[MAX_LINE_LENGTH];
    const char *line_ptr;
    char *word =NULL;
    int IC = MEM_START;
    int line_counter =0;
    
    short int instruction_type_switcher;
    bool no_error_flag =true;
    error_handler error_return = {0,0};

    FILE* fp_am = open_and_allocate(file_name, AM_EXT, READ);
    
    while (fgets(line, sizeof(line), fp_am) != NULL) {
        line_counter ++;
        error_return.lnum = line_counter;
        line_ptr = line;
        word = get_next_word_in_line(&line_ptr);
        
        if(line_type_test(word,NOTE)){
            continue;
        }

        if (is_label_definiton(word)) {
            free(word);
            word = get_next_word_in_line(&line_ptr);
        }
        instruction_type_switcher = instruction_type(word);
        if (instruction_type_switcher != CASE_NOT_INSTRUCTION){
            switch (instruction_type_switcher){
                case CASE_ENT:
                    error_return.error_num = handle_second_pass_entry(line,label_table,error_return);
                    if (error_return.error_num != NO_ERRORS)
                        no_error_flag =false;
                    break;
                case CASE_EXT:
                case CASE_STRING:
                case CASE_DATA:
                    continue;;
                
            }
        }
        
        else if(valid_action_word(word)){
            error_return.error_num= handle_action_word_seconed_pass(word,line_ptr,&IC,label_table,extern_table);
            if (error_return.error_num != NO_ERRORS)
                        no_error_flag =false;
        }
        free(word);
        
    }
    
    fclose(fp_am);
    return no_error_flag;
    
}
        