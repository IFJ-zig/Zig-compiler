/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/

#include "stack.h"

void stackInit(t_Stack *stack) {
	stack->top = NULL;
	stack->size = 0;
}

int stackPush(t_Stack *stack, stackElementType type, KeyWord keyword) {
	t_StackItem *newItem = (t_StackItem *)malloc(sizeof(t_StackItem));
	if (newItem == NULL) {
		return INTERNAL_COMPILER_ERROR;
	}
	newItem->type = type;
	newItem->keyword = keyword;
	newItem->next = stack->top;
	newItem->prev = NULL;
	if (stack->top != NULL) {
		stack->top->prev = newItem;
	}
	stack->top = newItem;
	stack->size++;

	return 0;
}

void stackPop(t_Stack *stack) {
	if (stack->top == NULL) {
		return;
	}
	t_StackItem *temp = stack->top;
	stack->top = stack->top->next;
	if (stack->top != NULL) {
		stack->top->prev = NULL;
	}
	free(temp);
	stack->size--;
}

t_StackItem *stackTop(t_Stack *stack) {
	return stack->top;
}

void stackClear(t_Stack *stack) {
	while (stack->top != NULL) {
		stackPop(stack);
	}
}

int stackPushPrecedentLess(t_Stack *stack) {
	t_StackItem *temp = stack->top;
	while (temp != NULL && temp->type != TERMINAL) {
		temp = temp->next;
	}
	if (temp == NULL) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	t_StackItem *newItem = (t_StackItem *)malloc(sizeof(t_StackItem));
	if (newItem == NULL) {
		return INTERNAL_COMPILER_ERROR;
	}
	newItem->type = PRECENDENT_LESS;

	newItem->prev = temp->prev;
	newItem->next = temp;

	newItem->prev->next = newItem;
	temp->prev = newItem;

	stack->size++;
	return 0;
}

t_StackItem *topTerminal(t_Stack *stack) {
	t_StackItem *temp = stack->top;
	while (temp != NULL && temp->type != TERMINAL) {
		temp = temp->next;
	}
	return temp;
}

void stackDestroy(t_Stack *stack) {
	stackClear(stack);
	free(stack);
}
