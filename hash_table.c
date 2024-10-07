#include "hash_table.h"

unsigned long hash(const char *str){  
        unsigned long hash = 5381;
        int c;
        while ((c = *str++)){
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }
        return hash % TABLE_SIZE;
    } /*dan berenstien hash function djb2 http://www.cse.yorku.ca/~oz/hash.html*/

bool search_key_in_table(Hash_table table,const char* key){
    if (key != NULL && search(table, key) == NULL ){
        return false;
        }
    return true;
}
void free_label_table(Hash_table label_table){
    int i;
    label* current_label;
    node temp;
    for (i =0; i<TABLE_SIZE; i++){
        node current = label_table[i];
        while (current != NULL){
            current_label = (label*)get_value(current);
            
            free(current_label);
            temp = current;
            current = get_next(current);
            /*free(temp->key);*/
            free(temp);
        }
    }
    
}

void free_table(Hash_table table){
    int i;
    for (i =0; i < TABLE_SIZE; i++){
       free_linked_list(table[i]);
    }
}

void* search(Hash_table table, const char* key){
    unsigned long index = hash(key);
    node current = table[index];
    char* result = search_in_node(current, key);
    return result;
    
}

int get_label_location(Hash_table label_tabel, char* label_name){
    label* result = search(label_tabel,label_name);
    if (result == NULL){
        /*error*/
        return -1;
    }
    
    return get_location(result);
}
/*checks that a label isnt mark as .extern*/
bool check_label_not_extern(Hash_table label_tabel, char* label_name){
    label* result = search(label_tabel,label_name);
    if (result == NULL){
        /*error*/
        return true;
    }
    
    if(get_location(result) !=EXTERN_LOCATION)
        return true;
    return false;
}
/*checks that a label isnt mark as .entry*/
bool check_label_not_entry(Hash_table label_tabel, char* label_name){
   label* result = search(label_tabel,label_name);
    if (result == NULL){
        
        return false;
    }
    else{
        if(get_entry_flag(result) != ENTRY_FLAG)
            return false;
    return true;
}
}



void insert(Hash_table table, const char* key, void* value){
    unsigned long index = hash(key);
    insert_linked_list((table + index), key, value);
}

int check_entry_flag(Hash_table table, char* label_name){
    label*  result = search(table,label_name);
    return get_entry_flag(result);
}


void create_and_insert_label(Hash_table table, char* label_name, int flag, int location_in_code,bool is_data){
    label* new_label;
    label* existing_label;
    
    existing_label = (label*)search(table,label_name);
    if(existing_label){
        if(flag != NO_ENTRY_FLAG)
            set_entry_flag(existing_label,flag);
        set_data_flag(existing_label,is_data);
        if(location_in_code != -1)
            set_location(existing_label,location_in_code);
    }

    else{
    new_label = create_label();   
    set_entry_flag(new_label,flag);
    set_location(new_label,location_in_code);
    set_data_flag(new_label,is_data);

    insert(table, label_name, new_label);
    }
}

void update_label_adress(Hash_table table, int IC){
    int i;
    node current;
    label *l;
    for(i =0; i < TABLE_SIZE; i++){
        current = table[i];
        while( current != NULL){
            l = (label*)get_value(current);
            if(get_data_flag(l)){
                set_location(l,(get_location(l)+IC -1));     
            }
            current = get_next(current);   
        }

    }
}


void extract_macro_from_table(Hash_table table, const char* key, FILE* output_file) {
    node outer_node = (node)search(table, key);
    node inner_node;

    if (outer_node == NULL) {
        fprintf(stderr, "Macro not found: %s\n", key);
        return;
    }

    inner_node = (node)outer_node;
    
    if (inner_node == NULL) {
        fprintf(stderr, "Macro definition is empty: %s\n", key);
        return;
    }

    while (inner_node != NULL) {
        if (get_key(inner_node) == NULL) {
            fprintf(stderr, "Warning: NULL key in macro definition of %s\n", key);
        } 
        else {
            fputs(get_key(inner_node), output_file);
          
        }
        inner_node = get_next(inner_node);
    }
}