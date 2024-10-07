#ifndef SECONDP
#define SECONDP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "mcr.h"
#include "errors.h"
#include "node.h"
#include "linked_list.h"
#include "hash_table.h"
#include "first_pass.h"

bool deploy_second_pass(char* file_name,Hash_table label_table,Hash_table macro_table,Hash_table extern_table);
#endif