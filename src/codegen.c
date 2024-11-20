#include "codegen.h"
#include <stddef.h> 
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

void stringToSlice(char* ){
    
}

void printHeader() {
    printf(".IFJcode24\n");
    printf("JUMP main\n");
    printf("\
        \
        LABEL readInt \n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            DEFVAR LF@retVal\n\
            DEFVAR LF@readValue\n\
            DEFVAR LF@readValueType\n\
            READ LF@readValue int\n\
            TYPE LF@readValueType LF@readValue\n\
            JUMPIFNEQ correct_input_int LF@readValueType nil@nil\n\
            MOVE LF@retVal nil@nil\n\
            POPFRAME\n\
            RETURN\n\
            LABEL correct_input_int\n\
            MOVE LF@retVal LF@readValue\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL int2float\n\
        CREATEFRAME\n\
        PUSHFRAME\n\
        \
        DEFVAR LF@retValue\n\
        DEFVAR LF@param\n\
        POPS LF@param\n\
        INT2FLOAT LF@retVal LF@param\n\
        \
        POPFRAME\n\
        RETURN\n\
        \
        \
        LABEL float2int\n\
        CREATEFRAME\n\
        PUSHFRAME\n\
        \
        DEFVAR LF@retVal\n\
        DEFVAR LF@param\n\
        POPS LF@param\n\
        FLOAT2INT LF@retVal LF@param\n\
        \
        POPFRAME\n\
        RETURN\n\
        \
        \
        \
    ");