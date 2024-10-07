#include "first_pass.h"
unsigned short int data_array[ARRAY_SIZE] ={0};
unsigned short int code_array[ARRAY_SIZE] ={0};

/*gets opcode and return a value to be |= to change the associated bit*/
opcodes return_opcode_bits(char* word){
    int i;
    for(i =0; i <NUM_OF_OP_CODS-1;i++)
        if(strcmp(word,valid_action_name[i])==0)
            return (i<< OPCODE_BIT);
    return STOP;
}

/*check that a first word is ending with a : and is of valid length and dont start with number*/
bool is_label_definiton(char* word){
    if (word[strlen(word)-1] == ':' && isalpha(word[0]) && strlen(word) < MAX_LABEL_LENGTH) /*isalpha*/
        return true;
    return false;
}

/*check if a word is of type action*/
bool valid_action_word(char* word){
    if(find_if_key_exist_in_list(valid_action_name,word))
        return true;
    return false;
}

ERROR_ID is_valid_label(char* word){
    int i;
    if (!isalpha(word[0]))
        return LABEL_E1;
    if (strlen(word) > MAX_LABEL_LENGTH)
        return LABEL_E2;
    for (i =0; i<strlen(word);i++){
        if (!isalnum(word[i]))
            return LABEL_E3;
    }
    if (!valid_action_word(word))
        return LABEL_E4;
    
    return NO_ERRORS;
}

/*check if a first word label was used already handle the case when it was inserted as entry*/
bool valid_and_undefined_label(char* word,Hash_table label_list){
    if(is_label_definiton(word) && strlen(word)>=2) {
        word[strlen(word)-1] = '\0';
        if (search_key_in_table(label_list,word)){
            if(check_entry_flag(label_list,word) == ENTRY_FLAG)
                return true;
            return errorbreak;
        }
        return true;
    }    
    return false;
}


/*gets word check if its a type of instruction an returns a value associated with that spesific type of action*/
short int instruction_type(char *word){
    if (strcmp(word,".string") == 0)
        return CASE_STRING;
    else if (strcmp(word,".data") == 0)
        return CASE_DATA;
    else if (strcmp(word,".extern") == 0)
        return CASE_EXT;
    else if (strcmp(word,".entry") == 0)
        return CASE_ENT;
    else
        return CASE_NOT_INSTRUCTION;
    
}

/*an error catching fucntion that checks that there are no case that a set of numbers is separated by white spaces without a comma as well*/
bool has_two_digit_sets(const char *token) {
    int digit_sets_counter = 0;
    int inside_digit_set = 0;
    int seen_whitespace = 0;
    int i;
    for (i = 0; token[i] != '\0'; i++) {
        if (isdigit(token[i])) {
            if (!inside_digit_set && seen_whitespace) {
                digit_sets_counter++;
                if (digit_sets_counter == 2) return true;
            }
            inside_digit_set = 1;
        } 
        else if (isspace(token[i])) {
            seen_whitespace = 1;
            inside_digit_set = 0;
        } 
        else {
            seen_whitespace = 0;
            inside_digit_set = 0;
        }
    }
    return false;
}


/*check if word is 2 or 3 charcters long (to account for ",") if it does and its start with a 'r' check that its a valid reg number retun false for any other case*/
bool is_valid_register(const char* word) {
   char digit = word[1];
    if ((strlen(word) == 2 || (strlen(word) == 3 && word[2] ==',')) && word[0] == 'r'){
        return digit >= '0' && digit <= '7';        
    }
    return false;
}

/*same as previus function but handels cases of indirect resgister (starting with *)*/
bool is_valid_register_indirect(const char* word) {
   char digit = word[2];
    if ((strlen(word) == 3 || (strlen(word) == 4 && word[3] ==',')) && word[1] == 'r' && word[0] =='*'){
        return digit >= '0' && digit <= '7';        
    }
    return false;
}


/*compares a word first character to a diffrent character/symbol*/
bool line_type_test(char *word,char to_test){  
    if (word[0] == to_test)
        return true;
    return false;       
}


/*check that the string is made out of only digits by using stspn ignoring leading white spaces if any exists*/
bool is_only_digits(const char *str) {
    size_t len;
    while (isspace((unsigned char)*str)){ 
        str++;
    }
    if (*str == '-')
        str++;
    len = strspn(str, "0123456789");
    return len == strlen(str) && len > 0;
}


/*converts string of chars or digits to binary representation and loads them into the data_array*/
ERROR_ID convert_to_binary_and_insert(char *word, int*counter,int flag_string){
    
    short int value;
    unsigned short int bvalue = 0;
    
    ERROR_ID return_value = NO_ERRORS;
    
    if (flag_string == true) {
        /*deal with char or single digit*/
        value = (short int)(*word);
    } 
    else {
        /*deal with multi digit number */
        value = (short int)atoi(word);
    }
    if (value > MAX_VALUE || value< MIN_VALUE){
        return return_value = DATA_OUT_OF_RANGE;
    }

    bvalue = (unsigned short int)(value & MASK);

    data_array[*counter] = bvalue;
    (*counter)++;
    return return_value;
}

/*handle string word pass it to insert character by character and manually add the stop codon 0000*/
ERROR_ID process_string(char *word,int *DC){
    ERROR_ID error = NO_ERRORS;
    char* content;
    size_t content_len;
    int i;
    size_t len = strlen(word);
    
    if (word[0] != '"' || word == NULL)
            return error =STRING_BRACKET_S;

    
    
    if (len < 2 || word[len-1] != '"') {
         return error =STRING_BRACKET_E;
    }

    content = word + 1;
    content_len = len - 2;
    if (content_len == 0 || strspn(content, " \t\r\n") == content_len) {
        /*insert zero row to array*/
        return error;
    }
    
    for(i =0 ;i < content_len; i++){
        if(*DC >= ARRAY_SIZE){
            perror("array overflow");
            exit(EXIT_FAILURE);
        }
          convert_to_binary_and_insert(&content[i],DC,true);  
    }
    data_array[*DC] = 0;
    (*DC)++;
    return error;
   

}


/*checks that data line is correctly inputed and send the values between commas to be inserted to the data array*/
ERROR_ID parse_data_line(char *line, int *DC) {
    char *token;
    int line_length = strlen(line);
    int last_comma_index = -1;
    int last_non_whitespace = -1;
    int i,j;
    ERROR_ID to_return = NO_ERRORS;
    
    /*finds the last non whitespace character*/
    for (i = line_length - 1; i >= 0; i--) {
        if (!isspace(line[i])) {
            last_non_whitespace = i;
            break;
        }
    }
    /*check for valid line format*/
    if (line[0] == ',' || (last_non_whitespace >= 0 && line[last_non_whitespace] == ',')) {
        to_return = DATA_COMMA_SEE;
        return to_return;
    }
    /*Check for two commas without a non-whitespace separator*/
    for (i = 0; i < line_length; i++) {
        if (line[i] == ',') {
            if (last_comma_index != -1) {
                int only_whitespace = true;
                for (j = last_comma_index + 1; j < i; j++) {
                    if (!isspace(line[j])) {
                        only_whitespace = false;
                        break;
                    }
                }
                if (only_whitespace) {
                    to_return = DATA_2_COMMAS;
                    return to_return;
                }
            }
            last_comma_index = i;
        }
    }
    token = strtok(line, ", ");
    if(strcmp(token,".data")== 0)
        token = strtok(NULL, ", ");
    else{
        token = strtok(NULL, ", ");
        token = strtok(NULL, ", ");
    }
        
    
    while (token != NULL) {
        char *end;
        while (isspace((unsigned char)*token)) token++;
        end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';
        
        if(strlen(token) ==0){
            
            to_return =DATA_NOT_NUM_E;
            return to_return;
        }
        /*need to write trim leading/trailing whitespaces then*/
        
        if(!is_only_digits(token)){
            
             to_return = DATA_NOT_NUM_E;
             return to_return;  
        }
        if(has_two_digit_sets(token)){
            to_return = DATA_2_NUM;
            return to_return;
        }
        /*check we didnt */
        if (*DC >= ARRAY_SIZE){
            perror("data overflow");
            exit(EXIT_FAILURE);        
        }
         /*add token to array*/
        convert_to_binary_and_insert(token,DC,false);
        token = strtok(NULL, ",");
    }
    
    return to_return;
}

/*switch to handle the first word binary code */
short int handle_addressing(char *source,char*destination,int number_of_instrctions,short int binary_code_word){
    int case_holder = binary_code_word;
    
    if(destination == NULL){
        destination = source;
        source = NULL;
    }

    switch (case_holder){
    case MOV:
    case CMP:
    case ADD:
    case SUB:
        if(number_of_instrctions != 2)
            return INSTRUCTION_ERROR;
        
        if (is_valid_register_indirect(source)){
            binary_code_word |= S_INDIRECT_R;
            
        }
        else if (is_valid_register(source)){
            binary_code_word |= S_DIRECT_R;
            
        }
        else if (line_type_test(source,NUMBER_SIGN)){
            binary_code_word |= S_IMMIDIATE;
            
        }
        else {
            binary_code_word |= S_DIRECT;
            
        }      
        if (is_valid_register_indirect(destination)){
            binary_code_word |= D_INDIRECT_R;
            
        }
        else if (is_valid_register(destination)){
            binary_code_word |= D_DIRECT_R;
            
        }
        else if (line_type_test(destination,NUMBER_SIGN)){
            if (case_holder == CMP)
                binary_code_word |= D_IMMIDIATE;
            else return INSTRUCTION_ERROR;
        }
        else {
            binary_code_word |= D_DIRECT;
            
        }   
            break;
        case LEA:
            if(number_of_instrctions !=2 || (!is_valid_register_indirect(destination) && !is_valid_register(destination) && !line_type_test(destination,NUMBER_SIGN)))
                return INSTRUCTION_ERROR;
            binary_code_word |= S_DIRECT;

            if (is_valid_register_indirect(destination)){
                binary_code_word |= D_INDIRECT_R;
                
            }
            else if (is_valid_register(destination)){
                binary_code_word |= D_DIRECT_R;
                
            }
            else if (line_type_test(destination,NUMBER_SIGN)){
                    return INSTRUCTION_ERROR;
            }
            else {
                binary_code_word |= D_DIRECT;
            }
            break;

        case CLR:
        case NOT:
        case INC:
        case DEC:
        case RED:
        case PRN:
            if(number_of_instrctions !=1)
                return INSTRUCTION_ERROR;
            if (is_valid_register_indirect(destination)){
                binary_code_word |= D_INDIRECT_R;  
            }
            else if (is_valid_register(destination)){
                binary_code_word |= D_DIRECT_R;    
            }
            else if (line_type_test(destination,NUMBER_SIGN)){
                     if (case_holder == PRN)
                        binary_code_word |= D_IMMIDIATE;
                    else return INSTRUCTION_ERROR;
            }
            else {
                binary_code_word |= D_DIRECT;
            }
            break;

        case JMP:
        case BNE:
        case JSR:
            if(number_of_instrctions !=1)
                return INSTRUCTION_ERROR;
            if (is_valid_register_indirect(destination)){
                binary_code_word |= D_INDIRECT_R;  
            }
            else if (is_valid_register(destination)){
                return INSTRUCTION_ERROR;  
            }
            else if (line_type_test(destination,NUMBER_SIGN)){
                    return INSTRUCTION_ERROR;
            }
            else {
                binary_code_word |= D_DIRECT;
            }
            break;
        case RTS:
        case STOP:
            if (number_of_instrctions !=0)
                return INSTRUCTION_ERROR;
        }

    binary_code_word |= ABSOLUTE;
    return binary_code_word;
}



/*procces the first word of action line and save up space for the following words to be filled in the 2nd pass */
ERROR_ID handle_action_word_fp(char *word,char *line, int *counter, Hash_table label_table){
    short int binary_code_word =0;
    ERROR_ID error_num = NO_ERRORS;
    char *token;
    char *source = NULL;
    char *destination = NULL;
    int register_flag =0;
    int L =0;
    
    binary_code_word |= return_opcode_bits(word);
    token = strtok(line, TOK);
    if (is_label_definiton(token))
        token = strtok(NULL, TOK);
    
    /*sets token for the opcode incase line started with label*/
    if (strcmp(word,token)!= 0){
        token = strtok(NULL, TOK);
    }
    token = strtok(NULL, TOK);
    
    if (token != NULL){
        source = token;
        L++;
         if (is_valid_register(token)||is_valid_register_indirect(token))
            register_flag +=1 ;
        token = strtok(NULL,TOK);
    }
    
    if (token != NULL){
        destination = token;
        L++;
        if (is_valid_register(token)||is_valid_register_indirect(token))
            register_flag +=1;
        token = strtok(NULL,TOK);   
    }
    
    if (token != NULL || token != '\0'){
            L++;
        }
    
    if (destination == NULL && source != NULL){
        destination = source;
        source = NULL;
    }
    
    if(L == 0 && (binary_code_word != STOP && binary_code_word != RTS)){
        error_num = INSTRUCTION_EMPY_E;
    }
    else if( L > 2)
        error_num = TOO_MANY_INSTRUCTIONS_E;
    else{
        binary_code_word |= handle_addressing(source,destination,L,binary_code_word);
        if(register_flag == 2){
            L--;
        }
        code_array[*counter] = binary_code_word;
        *counter += L+1;
    }
    return error_num;
}

/*get an extern line and insert its content to the label table*/
void handle_extern_tag(Hash_table label_table,const char **line_ptr, error_handler *error_return, int line_counter){
    char *word = (get_next_word_in_line(line_ptr));
        
        if (word == NULL){
            error_return->error_num = NO_EXTERN_LABEL_E;
            report_error_with_location(*error_return);
            return;
            
        }

        while (word != NULL){
            if (check_label_not_entry(label_table,word)){
                error_return->error_num = LABEL_TAKEN_E;
                report_error_with_location(*error_return); 
            }

            else{
                create_and_insert_label(label_table,word,NO_ENTRY_FLAG,EXTERN_LOCATION,false);
            }
            free(word);
            word = get_next_word_in_line(line_ptr);   
        }
}

/*insert entry line to the label table*/
void handle_entry_tag(Hash_table label_table,const char **line_ptr,bool *label_flag,char *label_holder, error_handler *error_return, int line_counter){
    char *word;
    if (*label_flag ==true){
            printf("warning label in %d .entery line ignored",line_counter);
            free(label_holder);
            *label_flag = false;
        }

    word = get_next_word_in_line(line_ptr);
    if (word == NULL){
        error_return->error_num =ENTRY_ERROR;
        report_error_with_location(*error_return);
        return;
    }

    /*this function assumes words in entry line are separated by spaces and not by ,*/
    while (word != NULL){
        if (check_label_not_extern(label_table,word)){
        create_and_insert_label(label_table,word,ENTRY_FLAG,NO_LOCATION,false);
        }
        else{
            error_return->error_num = LABEL_TAKEN_E;
            report_error_with_location(*error_return);
        }
        free(word);
        word = get_next_word_in_line(line_ptr);
    }

}

/*main function needs work still*/
ERROR_ID deploy_first_pass(char* file_name,Hash_table label_table,int *instruction_counter,int *data_counter,Hash_table macro_table){
    ERROR_ID error_flag = NO_ERRORS;
    error_handler error_return = {0,0};
    char line[MAX_LINE_LENGTH];
    const char *line_ptr;
    char *word =NULL;
    char *label_holder;
    short int instruction_type_switcher =0;
    int IC = MEM_START;   /*instruction counter*/
    int DC = DC_START_VALUE;/*data counter*/
    int line_counter =0;
    bool label_flag = 0;
    bool label_checker= 0;
    
    /*consider adding error flag for subfunctions*/

    FILE* fp_am = open_and_allocate(file_name, AM_EXT, READ);

    while (fgets(line, sizeof(line), fp_am) != NULL) {
        line_counter ++;
        error_return.lnum = line_counter;
        line_ptr = line;
        if (word != NULL) {
            free(word);
            word = NULL;
        }
        word = get_next_word_in_line(&line_ptr);
        
        /*comment line handler*/
        if(line_type_test(word,NOTE)){
            continue;
        }

        /*check if the first word is of type label:*/
        label_checker = valid_and_undefined_label(word,label_table);    
        if(label_checker == true){
            /*valid_action_word checks if the label is a saved word so we give error if it is*/
            if(valid_action_word(word)){
                error_return.error_num = RESERVED_LABEL_NAME_E;
                report_error_with_location(error_return);
                
            }
            /*else we rise label flag and save the label under label holder to use later*/
            label_flag =true;
            label_holder = word;
            word = (get_next_word_in_line(&line_ptr));
        }

        
        else if (label_checker == errorbreak){
            error_return.error_num = LABEL_TAKEN_E;
            report_error_with_location(error_return);
            error_flag =LABEL_TAKEN_E;
            
        }

        /*done with checking if the first word was a label/comment now we test if it, or if label was found the word aftet is an instruction*/
        instruction_type_switcher = instruction_type(word);
        if (instruction_type_switcher != CASE_NOT_INSTRUCTION){
            
            if (instruction_type_switcher == CASE_DATA){
                if(label_flag == true){
                    create_and_insert_label(label_table,label_holder,NO_ENTRY_FLAG,DC,true);
                    /*DC++;*/
                    label_flag =false;
                    free(label_holder);
                }
                error_return.error_num = parse_data_line(line,&DC);
                if (error_return.error_num != NO_ERRORS)
                    report_error_with_location(error_return);

            }

            else if(instruction_type_switcher == CASE_STRING){
                  if(label_flag == true){
                    create_and_insert_label(label_table,label_holder,NO_ENTRY_FLAG,DC,true);
                    /*DC++;*/
                    free(label_holder);
                    label_flag =false;
                }
                free(word);
                word = (get_next_word_in_line(&line_ptr));
                process_string(word,&DC);
                
            }

            else if (instruction_type_switcher == CASE_ENT){
                handle_entry_tag(label_table, &line_ptr, &label_flag, label_holder, &error_return, line_counter);
            }

            else{
                handle_extern_tag(label_table,&line_ptr,&error_return,line_counter);
            }
        }

        /*action word line handler*/
        else if(valid_action_word(word)){
            if(label_flag == true){
                    create_and_insert_label(label_table,label_holder,NO_ENTRY_FLAG,IC,false);
                    label_flag =false;
                    free(label_holder);
                }

            error_return.error_num= handle_action_word_fp(word,line,&IC,label_table);
            if ( error_return.error_num != NO_ERRORS)
                report_error_with_location(error_return);
                error_flag = error_return.error_num ;
        }

        else{
            /*return error for line that action name is invalid*/
            error_return.error_num = BAD_ACTION_NAME;
            report_error_with_location(error_return);
            error_flag = error_return.error_num ;
            break;
        }

    }
    
    *instruction_counter = (IC - MEM_START);
    *data_counter =(DC);
    if(word !=NULL) free(word);
   
    
    update_label_adress(label_table,IC);
    
    fclose(fp_am);
    return error_flag;
}




