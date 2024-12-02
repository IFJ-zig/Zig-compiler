/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
*********************************************/

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdbool.h>

#include "errors_enum.h"
#include "symtable.h"
#include "tokens.h"
#include "ast.h"

void semanticInit();
void semanticDestroy();

//Create a new symbol, returns 0 on success or REDEFINITION_ERROR is symbol of same name in scope already exists
int defineSymbol(char *name, varType type, bool isConst, bool isNullable);
void undefineSymbol(char *name);

int assignSymbol(symbol_t *symbol, varType type);
symbol_t *getSymbol(char *name);
bool isValidParamType(KeyWord kw);
int assignFunctionParameter(symbol_t *function, Token paramName, Token paramType, bool isNullable);
int checkExprTypesCompatibility(ast_node_exp_t *expression, varType *expType);
bool nullableInExpr(ast_node_exp_t *expression);
bool nullKWInExpr(ast_node_exp_t *expression);
bool containsNonConst(ast_node_exp_t *expression);
int isValidIfExpression(ast_node_exp_t *expression);

int processParam(Token paramName, Token paramType, bool isNullable);
varType kwToVarType(KeyWord kw);

void enterScope(); //A function for going into bigger depth so syntax analyzer doesn't need to keep track of depth itself
int exitScope();


#endif // SEMANTIC_H