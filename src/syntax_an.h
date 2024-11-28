/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/

#ifndef SYNTAX_H
#define SYNTAX_H

#include <stdbool.h>

#include "ast.h"
#include "errors_enum.h"
#include "expression_parser.h"
#include "semantic_an.h"
#include "symtable.h" //For varType
#include "tokens.h"
#include "codegen.h"

int syntax_analyzer(/*List *L*/);
int code();
int program();
int seekHeaders();
int skip_function_body();
int checkImport();
int function_analysis();
int function_call(bool expectReturn, ast_default_node_t *fnCallNode);
int return_type();
int data_type();
int param_list();
int while_syntax();
int if_else();
int inbuild_function(bool expectNext);
int return_syntax();
int call_or_assignment();
int variable_definition(bool isConst);
int empty_variable();
int read_token();
void loadIFJ24();


//Helper functions
bool isValidReturnType(KeyWord kw);
symbol_t *skip_expression_get_symbol();
#endif // SYNTAX_H