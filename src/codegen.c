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
        LABEL ifjreadi32\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@readValue%%\n\
            DEFVAR LF@readValueType%%\n\
            READ LF@readValue%% int\n\
            TYPE LF@readValueType%% LF@readValue%%\n\
            JUMPIFNEQ correct_input_int LF@readValueType%% nil@nil\n\
            MOVE LF@retVal%% nil@nil\n\
            POPFRAME\n\
            RETURN\n\
            LABEL correct_input_int\n\
            MOVE LF@retVal%% LF@readValue%%\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjreadf64\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@readValue%%\n\
            DEFVAR LF@readValueType%%\n\
            READ LF@readValue%% float\n\
            TYPE LF@readValueType%% LF@readValue%%\n\
            JUMPIFNEQ correct_input_float LF@readValueType%% nil@nil\n\
            MOVE LF@retVal%% nil@nil\n\
            POPFRAME\n\
            RETURN\n\
            LABEL correct_input_float\n\
            MOVE LF@retVal%% LF@readValue%%\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjreadstr\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@readValue%%\n\
            DEFVAR LF@readValueType%%\n\
            READ LF@readValue%% string\n\
            TYPE LF@readValueType%% LF@readValue%%\n\
            JUMPIFNEQ correct_input_string LF@readValueType%% nil@nil\n\
            MOVE LF@retVal%% nil@nil\n\
            POPFRAME\n\
            RETURN\n\
            LABEL correct_input_string\n\
            MOVE LF@retVal%% LF@readValue%%\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifji2f\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@param%%\n\
            POPS LF@param%%\n\
            INT2FLOAT LF@retVal%% LF@param%%\n\
            \
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjf2i\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@param%%\n\
            POPS LF@param%%\n\
            FLOAT2INT LF@retVal%% LF@param%%\n\
            \
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjlength\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@param%%\n\
            POPS LF@param%%\n\
            STRLEN LF@retVal%% LF@param%%\n\
            \
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjconcat\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@param1%%\n\
            DEFVAR LF@param2%%\n\
            POPS LF@param2%%\n\
            POPS LF@param1%%\n\
            CONCAT LF@retVal%% LF@param1%% LF@param2%%\n\
            \
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjsubstring\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            MOVE LF@retVal%% string@\n\
            DEFVAR LF@string%%\n\
            DEFVAR LF@startIndex%%\n\
            DEFVAR LF@endIndex%%\n\
            DEFVAR LF@strLength%%\n\
            POPS LF@endIndex%%\n\
            POPS LF@startIndex%%\n\
            POPS LF@string%%\n\
            STRLEN LF@strLength%% LF@string%%\n\
            DEFVAR LF@indexCheck%%\n\
            LT LF@indexCheck%% LF@startIndex%% int@0\n\
            JUMPIFEQ substrCheckFailed LF@indexCheck%% bool@true\n\
            LT LF@indexCheck%% LF@endIndex%% int@0\n\
            JUMPIFEQ substrCheckFailed LF@indexCheck%% bool@true\n\
            GT LF@indexCheck%% LF@startIndex%% LF@endIndex%%\n\
            JUMPIFEQ substrCheckFailed LF@indexCheck%% bool@true\n\
            LT LF@indexCheck%% LF@startIndex%% LF@strLength%%\n\
            JUMPIFNEQ substrCheckFailed LF@indexCheck%% bool@true\n\
            GT LF@indexCheck%% LF@endIndex%% LF@strLength%%\n\
            JUMPIFEQ substrCheckFailed LF@indexCheck%% bool@true\n\
            DEFVAR LF@substrChar%%\n\
            DEFVAR LF@substrCounter%%\n\
            SUB LF@substrCounter%% LF@endIndex%% LF@startIndex%%\n\
            LABEL substrWhileCycle\n\
            JUMPIFEQ substrCheckPassed LF@substrCounter%% int@0\n\
            GETCHAR LF@substrChar%% LF@string%% LF@startIndex%%\n\
            CONCAT LF@retVal%% LF@retVal%% LF@substrChar%%\n\
            ADD LF@startIndex%% LF@startIndex%% int@1\n\
            SUB LF@substrCounter%% LF@substrCounter%% int@1\n\
            JUMP substrWhileCycle\n\
            LABEL substrCheckFailed\n\
            MOVE LF@retVal%% nil@nil\n\
            LABEL substrCheckPassed\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjstrcmp\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@str1Length%%\n\
            DEFVAR LF@str2Length%%\n\
            DEFVAR LF@usedLength%%\n\
            DEFVAR LF@string1%%\n\
            DEFVAR LF@string2%%\n\
            DEFVAR LF@strcmpInfo%% ## 0 - equal, 1 - str1 > str2, 2 - str1 < str2\n\
            \
            POPS LF@string2%%\n\
            POPS LF@string1%%\n\
            STRLEN LF@str1Length%% LF@string1%%\n\
            STRLEN LF@str2Length%% LF@string2%%\n\
            DEFVAR LF@strcmpBool%%\n\
            EQ LF@strcmpBool%% LF@str1Length%% LF@str2Length%%\n\
            JUMPIFEQ strcmpLengthsEqual LF@strcmpBool%% bool@true\n\
            LT LF@strcmpBool%% LF@str1Length%% LF@str2Length%%\n\
            JUMPIFEQ strcmpLength1Smaller LF@strcmpBool%% bool@true\n\
            \
            LABEL strcmpLength1Greater\n\
            MOVE LF@usedLength%% LF@str2Length%%\n\
            MOVE LF@strcmpInfo%% int@1\n\
            JUMP strcmpCycle\n\
            LABEL strcmpLength1Smaller\n\
            MOVE LF@usedLength%% LF@str1Length%%\n\
            MOVE LF@strcmpInfo%% int@-1\n\
            JUMP strcmpCycle\n\
            LABEL strcmpLengthsEqual\n\
            MOVE LF@usedLength%% LF@str1Length%%\n\
            MOVE LF@strcmpInfo%% int@0\n\
            \
            LABEL strcmpCycle\n\
            DEFVAR LF@strcmpChar1%%\n\
            DEFVAR LF@strcmpChar2%%\n\
            DEFVAR LF@strcmpIndex%%\n\
            MOVE LF@strcmpIndex%% int@0\n\
            \
            JUMPIFEQ strcmpEnd LF@usedLength%% int@0\n\
            STRI2INT LF@strcmpChar1%% LF@string1%% LF@strcmpIndex%%\n\
            STRI2INT LF@strcmpChar2%% LF@string2%% LF@strcmpIndex%%\n\
            DPRINT LF@strcmpIndex%%\n\
            EQ LF@strcmpBool%% LF@strcmpChar1%% LF@strcmpChar2%%\n\
            JUMPIFEQ strcmpCycleDiff LF@strcmpBool%% bool@false\n\
            ADD LF@strcmpIndex%% LF@strcmpIndex%% int@1\n\
            SUB LF@usedLength%% LF@usedLength%% int@1\n\
            JUMP strcmpCycle\n\
            \
            LABEL strcmpCycleDiff\n\
            LT LF@strcmpBool%% LF@strcmpChar1%% LF@strcmpChar2%%\n\
            JUMPIFEQ strcmpString1Less LF@strcmpBool%% bool@true\n\
            MOVE LF@retVal%% int@1\n\
            POPFRAME\n\
            RETURN\n\
            \
            LABEL strcmpString1Less\n\
            MOVE LF@retVal%% int@-1\n\
            POPFRAME\n\
            RETURN\n\
            \
            LABEL strcmpEnd\n\
            MOVE LF@retVal%% LF@strcmpInfo%%\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjord\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@string%%\n\
            DEFVAR LF@index%%\n\
            DEFVAR LF@strLen%%\n\
            \
            POPS LF@index%%\n\
            POPS LF@string%%\n\
            STRLEN LF@strLen%% LF@string%%\n\
            \
            DEFVAR LF@indexCheck%%\n\
            EQ LF@indexCheck%% LF@strLen%% int@0\n\
            JUMPIFEQ ordCheckFailed LF@indexCheck%% bool@true\n\
            LT LF@indexCheck%% LF@index%% LF@strLen%%\n\
            JUMPIFEQ ordCheckFailed LF@indexCheck%% bool@false\n\
            LT LF@indexCheck%% LF@index%% int@0\n\
            JUMPIFEQ ordCheckFailed LF@indexCheck%% bool@true\n\
            \
            STRI2INT LF@retVal%% LF@string%% LF@index%%\n\
            POPFRAME\n\
            RETURN\n\
            \
            LABEL ordCheckFailed\n\
            MOVE LF@retVal%% int@0\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjchr\n\ 
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@char%%\n\
            POPS LF@char%%\n\
            INT2CHAR LF@retVal%% LF@char%%\n\
            \
            POPFRAME\n\
        RETURN\n\
    ");