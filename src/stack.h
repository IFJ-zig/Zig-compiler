/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/

#ifndef HSTACK_H__
#define HSTACK_H__

#include <stdbool.h>
#include <stdlib.h>

#include "ast.h"
#include "errors_enum.h"
#include "tokens.h"

typedef enum
{
	TERMINAL,
	NON_TERMINAL,
	PRECEDENT_LESS,
} stackElementType;

typedef struct Stack
{
	struct StackItem *top;
	int size;
} t_Stack;

typedef struct StackItem
{
	stackElementType type;
	Token *token;
	struct StackItem *next;
	struct StackItem *prev;
	ast_node_exp_t *node;
} t_StackItem;

// Function to initialize the stack
void stackInit(t_Stack *stack);

// Function to push an item onto the stack
int stackPush(t_Stack *stack, stackElementType type, Token *tkn, ast_node_exp_t *node);

// Function to pop an item from the stack
void stackPop(t_Stack *stack);

// Function to get the top item of the stack without popping it
t_StackItem *stackTop(t_Stack *stack);

// Function to clear the stack
void stackClear(t_Stack *stack);

// Function to push a precedent less item onto the stack after closest terminal
int stackPushPrecedentLess(t_Stack *stack);


//top terminal
t_StackItem *topTerminal(t_Stack *stack);

//print stack
void printStack(t_Stack *stack);
char mapTokenToChar(KeyWord kw);
#endif // HSTACK_H__