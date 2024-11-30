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

void semanticInit();
void semanticDestroy();

//Create a new symbol, returns 0 on success or REDEFINITION_ERROR is symbol of same name in scope already exists
int defineSymbol(char *name, varType type, bool isConst, bool isNullable);
void undefineSymbol(char *name);

bool assignSymbol(char *name, KeyWord kw);
symbol_t *getSymbol(char *name);
bool isValidParamType(KeyWord kw);
void assignFunctionParameter(symbol_t *function, Token paramName, Token paramType, bool isNullable);

int processParam(Token paramName, Token paramType, bool isNullable);
varType kwToVarType(KeyWord kw);

void enterScope(); //A function for going into bigger depth so syntax analyzer doesn't need to keep track of depth itself
void exitScope();


#endif // SEMANTIC_H