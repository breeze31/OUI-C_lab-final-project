#include "assembler.h"
/*print the data in the array to the file*/
void print_to_file(unsigned short array[],int start,int end,FILE* file_name,int extra){
    int i;
    for (i = start; i< (start+end) ;i++){
        fprintf(file_name,"%d :%05o \n",i+extra,array[i]);
    }
    
}
/*makes the enrty output file*/
bool print_entry_labels(Hash_table label_table, FILE *fp_ent) {
    int i;
    node current;
    label *l;
    bool is_empty =true;
    for (i = 0; i < TABLE_SIZE; i++) {
        current = label_table[i];
        while (current != NULL) {
            l = (label*)get_value(current);
            if (get_entry_flag(l) == ENTRY_FLAG) {
                is_empty = false;
                fprintf(fp_ent, "%s %d\n", get_key(current), get_location(l));
            }
            current = get_next(current);
        }
    }
   
    
    return is_empty;
    
   
}
bool print_extern_file(FILE* fp_ext,Hash_table extern_table){
    int i;
    node current;
    char* key;
    int* location;
    bool is_empty =true;
    for (i = 0; i < TABLE_SIZE; i++){
        current = extern_table[i];
        while (current != NULL)
        {
            key = get_key(current);
            location = (int*)get_value(current);
            
            if (key != NULL && location != NULL){
                is_empty = false;
                fprintf(fp_ext," %s, %d\n", key, *location);
            }
            current = get_next(current);
        }


    }
    return is_empty;
}

/*if no errors are found create output files*/
void create_final_output(char *file_name, int IC, int DC,Hash_table label_tabel,Hash_table extern_table){
    FILE* fp_ob = open_and_allocate(file_name,OB,WRITE);
    FILE* fp_ext = open_and_allocate(file_name,EXT_F,WRITE);
    FILE* fp_ent = open_and_allocate(file_name,ENT_F,WRITE);
    bool empty_checker;
    char *name;
    fprintf(fp_ob, "%d    %d\n", IC, DC);
    print_to_file(code_array,MEM_START,IC,fp_ob,0);
    print_to_file(data_array,1,DC-1,fp_ob,MEM_START +IC-1);
    fclose(fp_ob);
    empty_checker = print_entry_labels(label_tabel,fp_ent);
    if (empty_checker){
        name = add_extension(file_name,ENT_F);
        remove(name);
        free(name);
    }
    empty_checker = print_extern_file(fp_ext,extern_table);
    if (empty_checker){
        name = add_extension(file_name,EXT_F);
        remove(name);
        free(name);
    }
    fclose(fp_ent);
    fclose(fp_ext);
}

int main(int argc, char* argv[]) {
    ERROR_ID fpass_no_error = NO_ERRORS;
    bool spass_no_error = false;
    int instruction_counter;
    int data_counter;
    error_handler file ={0,0};
    
    int i;
    for (i = 1; i < argc; i++) {
        Hash_table label_table ={0};
        Hash_table macro_table ={0};
        Hash_table extern_table = {0};
        file = deploy_macro(argv[i],macro_table);
        if (file.error_num != NO_ERRORS){
            cleanup_if_error(argv[i],macro_table);
            fprintf(stderr,"in the \"%s\" file there is the following: ",argv[i]);
            report_error_with_location(file);
            continue;
        }
        fpass_no_error = deploy_first_pass(argv[i],label_table,&instruction_counter,&data_counter,macro_table);
        
        
        if (fpass_no_error != NO_ERRORS){
            free_table(macro_table);
            free_label_table(label_table);
            continue;
        }
        
        spass_no_error = deploy_second_pass(argv[i],label_table,macro_table,extern_table);
        
        
        if (spass_no_error == false){
            free_table(macro_table);
            free_label_table(label_table);
            continue;
        }
        
        create_final_output(argv[i],instruction_counter,data_counter,label_table,extern_table);
        free_table(macro_table);
        free_label_table(label_table);
    }
  
    return 0;
    
}

