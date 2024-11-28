/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/

#ifndef HEXP_H__
#define HEXP_H__

#define TABLE_SIZE 15

#include <stdlib.h>

#include "errors_enum.h"
#include "semantic_an.h"
#include "stack.h"
#include "syntax_an.h"
#include "tokens.h"

int precedentTableTranslator(int token);
int getOperation(int tokenStack, int tokenInput);
int expressionParser(bool tokenRead, ast_node_exp_t **resultPointer);
int tryToMatchRule(t_Stack *stack);
int reduceBracketNonTerminal(t_Stack *stack);
int createValueExp(t_Stack *stack);
int createBinaryExp(t_Stack *stack);
int createUnaryExp(t_Stack *stack);

#endif // HEXP_H__