#ifndef PRECEDENT_TABLE_H
#define PRECEDENT_TABLE_H
#define TABLE_SIZE 15
#define EMPTY -1
#include <stdbool.h>
#include <string.h> //strcat,cmp,cpy,len

#include "errors_enum.h"
#include "tokens.h"

typedef struct StackItem // Item of stack
{
	struct StackItem *next;
	int ranking;
	int dataType;
	bool stillCons;
	Token *attributes;
	int data; // Expected token value (for example NONTERMINAL, PRECEDENT_LESS, TOKEN_NUMBER...)
} t_StackItem;

typedef struct Stack // Stack structure
{
	t_StackItem *first;
} t_Stack;

void stackInit(t_Stack *s);                                                                     // Initialize stack
int topTerminal(t_Stack *s);                                                                    // Returns first terminal value from top of stack
t_StackItem popStack(t_Stack *s);                                                               // Returns data from top of stack and delete it in stack
t_StackItem topStack(t_Stack *s);                                                               // Return top from stack without deleting top
int pushStack(t_Stack *s, int data, int dataType, Token *attribute, bool isConst, int ranking); // Pushing new top to stack with new token value
int pushAfterTopTerminal(t_Stack *s);                                                           // Pushing PRECEDENT_LESS value after first terminal in stack
bool isInStack(t_Stack *s, int data);                                                           // Returns true, when data value is in stack, false in opposite situation
void deleteStack(t_Stack *s);                                                                   // Deletes stack
void printStack(t_Stack *s);                                                                    // Prints all data from stack

#endif // PRECEDENT_TABLE_H