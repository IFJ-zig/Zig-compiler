/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/

#ifndef HEXP_H__
#define HEXP_H__

#define TABLE_SIZE 15

#include <stdlib.h>

#include "errors_enum.h"
#include "stack.h"
#include "tokens.h"

int precedentTableTranslator(int token);
int getOperation(int tokenStack, int tokenInput);
int expressionParser(List *tokenList);
int getTokenPrec();
int tryToMatchRule(t_Stack *stack);

#endif // HEXP_H__