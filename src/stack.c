/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains implementation of stack used in precedence analysis 
 *  @file stack.c
 *  @author Adam Vožda, xvozdaa00
 *  @brief Stack used in precedence analysis 
*/

#include "stack.h"

// Initialize the stack
void stackInit(t_Stack *stack) {
	stack->top = NULL;
	stack->size = 0;
}

// Push an item onto the stack
int stackPush(t_Stack *stack, stackElementType type, Token *tkn, ast_node_exp_t *node) {
	t_StackItem *newItem = (t_StackItem *)malloc(sizeof(t_StackItem));
	if (newItem == NULL) {
		return INTERNAL_COMPILER_ERROR;
	}

	newItem->type = type;
	newItem->token = tkn;
	newItem->prev = stack->top;
	newItem->next = NULL;
	newItem->node = node;
	if (stack->top != NULL) {
		stack->top->next = newItem;
	}
	stack->top = newItem;
	stack->size++;

	return 0;
}

// Pop an item from the stack
void stackPop(t_Stack *stack) {
	if (stack->top == NULL) {
		return;
	}
	t_StackItem *temp = stack->top;
	stack->top = temp->prev;
	if (stack->top != NULL) {
		stack->top->next = NULL;
	}
	free(temp);
	stack->size--;
}

// Get the top item of the stack
t_StackItem *stackTop(t_Stack *stack) {
	return stack->top;
}

// Clear the stack
void stackClear(t_Stack *stack) {
	while (stack->top != NULL) {
		stackPop(stack);
	}
}

// Push a PRECEDENT_LESS item after the top terminal item in the stack
int stackPushPrecedentLess(t_Stack *stack) {
	t_StackItem *temp = topTerminal(stack);
	if (temp == NULL) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	t_StackItem *newItem = (t_StackItem *)malloc(sizeof(t_StackItem));
	if (newItem == NULL) {
		return INTERNAL_COMPILER_ERROR;
	}
	newItem->type = PRECEDENT_LESS;

	newItem->next = temp->next;
	newItem->prev = temp;

	if (temp->next != NULL) {
		newItem->next->prev = newItem;
	} else {
		stack->top = newItem;
	}

	temp->next = newItem;
	stack->size++;
	return 0;
}

// Get the top terminal item in the stack
t_StackItem *topTerminal(t_Stack *stack) {
	t_StackItem *temp = stack->top;
	while (temp != NULL && temp->type != TERMINAL) {
		temp = temp->prev;
	}
	return temp;
}

// Print the stack
void printStack(t_Stack *stack) {
	t_StackItem *temp = stack->top;
	while (temp != NULL && temp->prev != NULL) {
		temp = temp->prev;
	}

	while (temp != NULL) {
		if (temp->type == TERMINAL) {
			fprintf(stderr, "%c", mapTokenToChar(temp->token->kw));
		} else if (temp->type == NON_TERMINAL) {
			fprintf(stderr, "E");
		} else {
			fprintf(stderr, "<");
		}
		temp = temp->next;
	}
	fprintf(stderr, "\n");
}

// Map a token keyword to a character
char mapTokenToChar(KeyWord kw) {
	if (kw == num || kw == decim || kw == text || kw == id || kw == _null) {
		return 'x';
	}

	switch (kw) {
		case plus:
			return '+';
		case minus:
			return '-';
		case multiply:
			return '*';
		case division:
			return '/';
		case less:
			return '<';
		case more:
			return '>';
		case compare_equal:
			fprintf(stderr, "=");
			return '=';
		case nequal:
			fprintf(stderr, "!");
			return '=';
		case lequal:
			fprintf(stderr, "<");
			return '=';
		case mequal:
			fprintf(stderr, ">");
			return '=';
		case lbracket:
			return '(';
		case rbracket:
			return ')';
		case comma:
		case next:
		case lblock:
			return '$';
		default:
			return '?';
	}
}