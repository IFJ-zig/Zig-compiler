/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/

#ifndef SYNTAX_H
#define SYNTAX_H

#include "errors_enum.h"
#include "tokens.h"


int Syntax_analyzer(List *L);
int code();
int program();
int seekHeaders();
int skipFunctionBody();
int checkImport();
int function_analysis();
int function_call();
int return_type();
int data_type();
int param_list();

#endif // SYNTAX_H