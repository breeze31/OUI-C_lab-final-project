#include "errors.h"

/*global array string for error possible and printf for errors by return error number*/

const char *error_message[] = {
    "0: no error",
    "1: file not found",
    "2: invalid name",
    "3: more than one word in the macro declaration line",
    "4: more than one word in the endmacro declaration line",
    "5: Label value was already filled",
    "6: empty extern line",
    "7: macro name already taken",
    "8: macro declaration error",
    "9: label name cant be a reserved name",
    "10: label is already taken",
    "11: incorrect action name",
    "12: missing brackets after .string",
    "13: missing closing brackets after .string",
    "14: .data can only accept numbers",
    "15: .data line starts or ends with ','",
    "16: .data line has 2 ',' without any number between them",
    "17: .data line has more then one number between commas",
    "18: .data line numbers was outside of 15digit bit range",
    "19: no data in instruction line",
    "20: too many instructions in an instruction line",
    "21: illigal addressing in an instruction line",
    "22: .entry without any value",
    "23: .entry label without implamentation",
    "24: label must start with a letter",
    "25: label must be no more than 31 characters",
    "26: label must be only letters and characters",
    "27 label must not be a reserved word"
    
};

void report_error_with_location( error_handler file){
    
    fprintf(stderr,"at line: %d  there is : %s\n",  file.lnum, error_message[file.error_num]);


}
