#include "codegen.h"
#include <stddef.h> 
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>

void printHeader() {
    printf(".IFJcode24\n");
    printf("JUMP main\n");