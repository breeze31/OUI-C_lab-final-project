#include "mcr.h"
#include "mcr.h"

/*get file name + extension and return them as new one long name*/
char* add_extension(const char* file_name, const char* extension){
    
    char * new_name = malloc_test(strlen(file_name)+strlen(extension)+1) ;
    return strcat(strcpy(new_name,file_name),extension); 
       
}

/*create a file*/
FILE* open_and_allocate( char* filename,const char* extension,char* mode){
    char* name= add_extension(filename, extension);
    FILE* file = fopen(name,mode);
    if (file == NULL){
        perror("failed to open file");
    }
    free(name);
    return file;
}
/*compare macro to list of saved words*/
bool macro_in_reserved_words(const char *list[],char *macro_key){
    if (find_if_key_exist_in_list(list,macro_key))
        return true;
    return false;
}
/*use to get the next word in a line*/
char* get_next_word_in_line(const char** line){
    char* word_from_line = NULL;
    int index = 0;
  
    if (*line == NULL || **line == '\0' ){
        return NULL;
    }

    word_from_line = malloc_test(strlen(*line)+1);
    if(sscanf(*line,"%s%n", word_from_line, &index)==1){
        *line += index;
        return word_from_line;
    }
    free(word_from_line);    
    return NULL;
}
/*parse the macro definition lines and save the values into the hashtable*/
error_handler process_macro_definition(Hash_table macro_table, const char* macro_name, FILE* fp_as) {
    char line[MAX_LINE_LENGTH];
    const char* line_ptr;
    char* word_temp;
    node lines = NULL;
    error_handler handler = {0,0};
    while (fgets(line, sizeof(line), fp_as) != NULL) {
        line_ptr = line;
        handler.lnum++;
        word_temp = get_next_word_in_line(&line_ptr);
        if (word_temp != NULL && strcmp(word_temp, ENDMCRO) == 0) {
            if (get_next_word_in_line(&line_ptr) != NULL) {
                handler.error_num = ENDMAC_E;
                free(word_temp);
                free_linked_list(lines);
                return handler;
            }
                
            free(word_temp);
            break;
         }
        insert_linked_list(&lines, line, NULL);
        free(word_temp);
    }
    
    insert(macro_table, macro_name, lines);
    return handler;    
}
/*checks if there are no duplicate macro names and that macro is leagal*/
int macro_wasnt_used(Hash_table macro_table,char* word,char* word_temp){
    if (!macro_in_reserved_words(saved_words, word) && word_temp == NULL) {
            if ((search_key_in_table(macro_table, word))){
                return NAME_USED_E;
                }
            return true;
            }
    else if (!macro_in_reserved_words(saved_words, word) && word_temp != NULL){
        return MACR_E;
    }
    return false;
}
/*delete the output if errors were found*/
void cleanup_if_error(char* filename,Hash_table macro_table) {
    char *new_name;
    free_table(macro_table);
    new_name = add_extension(filename,AM_EXT);
    remove(new_name);
    free(new_name);
    
}
/*the main function of the preprocessor go over line by line handel the macro deployment and return the error number */
error_handler deploy_macro(char* file_name,Hash_table macro_table) {
    char line[MAX_LINE_LENGTH];
    const char *line_ptr;
    char *word = NULL;
    char* word_temp = NULL;
    int test_integrity;
    error_handler holder = {0,0};
    error_handler secndery = {0,0};
    
    
    FILE* fp_as = open_and_allocate(file_name, AS_EXT, READ);
    FILE* fp_am = open_and_allocate(file_name, AM_EXT, WRITE);
    if (fp_as == NULL|| fp_am == NULL){
        holder.error_num = FILE_NAME_E;
        return holder;
    }
       
    while (fgets(line, sizeof(line), fp_as) != NULL) {
        line_ptr = line;
        free(word);
        word =NULL;
        word = get_next_word_in_line(&line_ptr);
        holder.lnum ++;
        if (word != NULL) {
            if (strcmp(word, MACRO) == 0) { 
                free(word);
                word =NULL;
                word = get_next_word_in_line(&line_ptr);
                if(word == NULL){
                    holder.error_num = MACRO_NAME_WASNT_FOUND;
                    break;
                }
                word_temp = get_next_word_in_line(&line_ptr);
                test_integrity = macro_wasnt_used(macro_table, word, word_temp);

                if (test_integrity == true) {
                    free(word_temp);
                    word_temp = NULL;
                    secndery = process_macro_definition(macro_table, word, fp_as);
                    holder.lnum += secndery.lnum;
                    holder.error_num += secndery.error_num;
                    if (holder.error_num != 0) break;
                }
                else if (test_integrity >= errorbreak) {
                    free(word_temp);
                    word_temp = NULL;
                    holder.error_num = test_integrity;
                    break;
                }
            }

            else if (search_key_in_table(macro_table, word)) {
                extract_macro_from_table(macro_table, word, fp_am);
            }
            else {
                fputs(line, fp_am);
            }
        }
        
    }
    if (word != NULL){ 
        free(word);
        word = NULL;
    }
    
    
    fclose(fp_am);
    fclose(fp_as);
    return holder;
}
