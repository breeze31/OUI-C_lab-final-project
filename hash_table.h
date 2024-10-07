#ifndef HASH_TABLE
#define HASH_TABLE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "linked_list.h"
#include "node.h"
typedef node Hash_table[TABLE_SIZE];
extern void* search(Hash_table table, const char* key);
extern void insert(Hash_table table, const char* key, void* value);
extern unsigned long hash(const char *str);
extern void extract_macro_from_table(Hash_table table, const char* key, FILE* output_file);
void free_table(Hash_table table);
extern bool search_key_in_table(Hash_table table,const char* key);
extern int check_entry_flag(Hash_table table, char* label_name);
void free_label_table(Hash_table label_table);
extern void create_and_insert_label(Hash_table table, char* label_name, int flag, int location_in_code,bool is_data);
extern void update_label_adress(Hash_table table, int IC);
extern int get_label_location(Hash_table label_tabel, char* label_name);
bool check_label_not_extern(Hash_table label_tabel, char* label_name);
bool check_label_not_entry(Hash_table label_tabel, char* label_name);
#endif