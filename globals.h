#ifndef GLOBALS
#define GLOBALS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define TABLE_SIZE 100
#define MAX_LINE_LENGTH 81
#define MAX_MACRO_NAME_LENGTH 50
#define MEM_START 100
#define MAX_LABEL_LENGTH 32
#define MASK 32767
/*file related definitions*/
#define AS_EXT ".as"
#define AM_EXT ".am"
#define READ "r"
#define WRITE "w"
/*type_test related definitions*/
#define NUMBER_SIGN '#'
#define NOTE ';'
#define TO_REG '*'
#define END_OF_STRING '00000'
#define OBJECT_LINE_DOUBLE_SPACE '  '
extern void* malloc_test(size_t size);

typedef enum registers {
	r0 = 0,
	r1,
	r2,
	r3,
	r4,
	r5,
	r6,
	r7
} reg;
#define TOK " ,\r\t\n"
#define NUM_OF_OP_CODS 16
#define NUM_OF_SAVED_WORDS 22

#define ENTRY_FLAG 9/*random value*/
#define NO_ENTRY_FLAG 0
#define EXTERN_LOCATION 0
#define E 1;

typedef enum {false, true, errorbreak} bool;
extern const char *saved_words[NUM_OF_SAVED_WORDS];
extern const char *valid_action_name[NUM_OF_OP_CODS];
extern bool find_if_key_exist_in_list(const char *list[],char *macro_key);
#endif