#include "linked_list.h"
struct node {
    char* key;
    void* value;
    struct node* next;
};

struct label{   
    int entry_flag;
    bool data_flag;
    int location;
};

void* get_value(node current){
    return current->value;
}

char* get_key(node current){
    return current ->key;
}

label* create_label(){
    label* new_label =(label*)malloc_test(sizeof(label));
    if(new_label == NULL)
        return NULL;
    new_label->entry_flag = 0;
    new_label->data_flag = false;
    new_label->location = 0;
    return new_label;
}

node get_next(node current){
    return current ->next;
}

int get_location(label* current){
    return current->location;
}

int set_location(label* current, int new_location) {
    return current->location = new_location;
    
}

bool get_data_flag(label* current){
    return current ->data_flag;
}
bool set_data_flag(label* current,bool flag){
    return current->data_flag = flag;
}
int get_entry_flag(label* current){
    return current -> entry_flag;
}
int set_entry_flag(label* current,int flag){
    return current -> entry_flag = flag;
}


node create_node(const char* key, void* value){
    node new_node = (node)malloc_test(sizeof(struct node));
    new_node-> key = (char*)malloc_test(strlen(key) + 1);
    strcpy(new_node-> key,key);
    new_node-> value = value;
    new_node-> next = NULL;
    return new_node;
}

void insert_linked_list(node *head,const char* key, void* value){
    node new_node = create_node(key, value); 
    node curr= *head;

    if (*head == NULL){
        *head = new_node;
    }

    else{
        while(curr->next != NULL)
            curr = curr->next;
        curr->next = new_node;
    }

}
/*return the value of a node*/
void* search_in_node(node current,const char* key){
    while (current != NULL){
        if (strcmp(current -> key, key) == 0){
            return current -> value;
        }
        current = current -> next;
    }
    return NULL;  
} 

void free_linked_list(node current){

        while (current != NULL){
            node temp = current;
            current = current-> next;
            free(temp -> key);
            if(temp -> value){
                free_linked_list(temp->value);
            }
            free(temp);
        }
}
