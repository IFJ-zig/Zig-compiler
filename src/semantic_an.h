/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
*********************************************/

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdbool.h>

#include "errors_enum.h"
#include "tokens.h"
#include "symtable.h"

//Create a new symbol, returns 0 on success or REDEFINITION_ERROR is symbol of same name in scope already exists
int defineSymbol(char *name, int depth, char *value, varType type);

bool assignSymbol(char *name, int depth, char *value, varType type);
bool symbolExists(char *name, int depth);

/*
void enterScope(); //A function for going into bigger depth so syntax analyzer doesn't need to keep track of depth itself
void exitScope();
*/



#endif // SEMANTIC_H