#ifndef LINKED_LIST
#define LINKED_LIST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "node.h"
typedef struct node *node;
typedef struct label label;
int get_location(label* current);
label* create_label();
int set_location(label* current, int new_location);
void insert_linked_list(node *head, const char* key, void* value);
void free_linked_list(node head);
void* search_in_node(node current, const char* key);
int set_entry_flag(label* current,int flag);
int get_entry_flag(label* current);
bool set_data_flag(label* current,bool flag);
bool get_data_flag(label* current);
node get_next(node current);
char* get_key(node current);
void* get_value(node current);
#endif
