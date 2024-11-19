/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/

#ifndef SYNTAX_H
#define SYNTAX_H

#include <stdbool.h>

#include "errors_enum.h"
#include "precedent_an.h"
#include "tokens.h"

int syntax_analyzer(List *L);
int code(bool tokenWasGiven);
int program();
int seekHeaders();
int skip_function_body();
int checkImport();
int function_analysis();
int function_call();
int return_type();
int data_type();
int param_list();
int while_syntax();
int if_else();
int inbuild_function();
int return_syntax();
int call_or_assignment();
int variable_definition();
int skip_expression();
#endif // SYNTAX_H