#ifndef FIRST_PASS
#define FIRST_PASS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "mcr.h"
#include "errors.h"
#include "node.h"
#include "linked_list.h"
#include "hash_table.h"
enum instruction_holder{
    CASE_STRING = 1,
    CASE_DATA = 2,
    CASE_EXT = 3,
    CASE_ENT = 4,
    CASE_NOT_INSTRUCTION =5
};
typedef enum {
    MOV,
    CMP = 1<<11,
    ADD =2<<11,
    SUB =3<<11,
    LEA =4<<11,
    CLR =5<<11,
    NOT =6<<11,
    INC =7<<11,
    DEC =8<<11,
    JMP =9<<11,
    BNE =10<<11,
    RED =11<<11,
    PRN =12<<11,
    JSR =13<<11,
    RTS =14<<11,
    STOP =15<<11


}opcodes;

typedef enum {
    /*sources*/
    S_IMMIDIATE = 1<<7,
    S_DIRECT= 1<<8,
    S_INDIRECT_R = 1<<9,
    S_DIRECT_R = 1<<10,
    /*destination*/
    D_IMMIDIATE =1<<3,
    D_DIRECT = 1<<4,
    D_INDIRECT_R = 1<<5,
    D_DIRECT_R = 1<<6
}adressing;


#define EXTERN_MARKER 1
#define NO_LOCATION -1
#define DC_START_VALUE 1
#define ABSOLUTE 4
#define RELOCATABLE 2
#define MAX_VALUE 16383
#define MIN_VALUE -16384
#define ARRAY_SIZE 4096
#define OPCODE_BIT 11
#define INSTRUCTION_ERROR 20000 /*anumber that is outside of the given leagal range used to keep an error that cant be accidently inserted*/
extern unsigned short int data_array[ARRAY_SIZE];
extern unsigned short int code_array[ARRAY_SIZE];
bool is_label_definiton(char* word);
ERROR_ID is_valid_label(char* word);
short int instruction_type(char *word);
extern bool is_valid_register(const char* word);
extern bool is_valid_register_indirect(const char* word);
extern bool line_type_test(char *word,char to_test);
extern bool valid_and_undefined_label(char* word,Hash_table label_list);
extern opcodes return_opcode_bits(char* word);
extern ERROR_ID deploy_first_pass(char* file_name,Hash_table label_table,int *instruction_counter,int *data_counter,Hash_table macro_table);
bool valid_action_word(char* word);
short int handle_addressing(char *source,char*destination,int number_of_instrctions,short int binary_code_word);
#endif
