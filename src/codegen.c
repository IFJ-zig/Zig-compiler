#include "codegen.h"
#include <stddef.h> 
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

void printHeader() {
    printf(".IFJcode24\n");
    printf("JUMP main\n");
    printf("\
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