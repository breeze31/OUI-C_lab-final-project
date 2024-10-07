#ifndef ERROR
#define ERROR
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct error_handler{
    int error_num;
    int lnum;
} error_handler;


typedef enum ERROR_ID {
    NO_ERRORS =0,
    FILE_NAME_E =1,
    ERROR_ID3 =2,
    MACR_E =3,
    ENDMAC_E =4,
    LABEL_VALUE_ALREADY_STORED =5,
    NO_EXTERN_LABEL_E =6,
    NAME_USED_E =7,
    MACRO_NAME_WASNT_FOUND =8,
    RESERVED_LABEL_NAME_E =9,
    LABEL_TAKEN_E =10,
    BAD_ACTION_NAME =11,
    STRING_BRACKET_S =12,
    STRING_BRACKET_E =13,
    DATA_NOT_NUM_E =14,
    DATA_COMMA_SEE =15,
    DATA_2_COMMAS = 16,
    DATA_2_NUM =17,
    DATA_OUT_OF_RANGE =18,
    INSTRUCTION_EMPY_E =19,
    TOO_MANY_INSTRUCTIONS_E =20,
    ILLIGAL_ADRESSING_E =21,
    ENTRY_ERROR =22,
    ENTRY_IMPLAMENT_E =23,
    LABEL_E1 =24,
    LABEL_E2 =25,
    LABEL_E3 =26,
    LABEL_E4 =27
    

} ERROR_ID;
extern void report_error_with_location(error_handler file);

extern const char *error_message[];
#endif
/*extern void ext_check(const char *file, int line);*/
/*global array string for error possible and printf for errors by return error number*/

