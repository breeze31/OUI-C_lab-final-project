#ifndef MCR_H
#define MCR_H
#define MACRO "macr"
#define ENDMCRO "endmacr"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"
#include "errors.h"
#include "node.h"
#include "linked_list.h"
#include "hash_table.h"
char* add_extension(const char* file_name, const char* extension);
extern FILE* open_and_allocate( char* filename,const char* extension,char* mode);
extern char* get_next_word_in_line(const char** line);
extern bool macro_in_reserved_words(const char *list[],char *macro_key);
extern error_handler deploy_macro(char* file_name,Hash_table macro_table);
extern void cleanup_if_error(char* filename,Hash_table macro_table);
#endif





