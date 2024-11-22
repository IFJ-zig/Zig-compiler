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
	newItem->prev = stack->top;
	newItem->next = NULL;
	if (stack->top != NULL) {
		stack->top->next = newItem;
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
	stack->top = temp->prev;
	if (stack->top != NULL) {
		stack->top->next = NULL;
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

t_StackItem *topTerminal(t_Stack *stack) {
	t_StackItem *temp = stack->top;
	while (temp != NULL && temp->type != TERMINAL) {
		temp = temp->prev;
	}
	return temp;
}


void printStack(t_Stack *stack) {
	t_StackItem *temp = stack->top;
	while (temp != NULL && temp->prev != NULL) {
		temp = temp->prev;
	}

	while (temp != NULL) {
		if (temp->type == TERMINAL) {
			printf("%c", mapTokenToChar(temp->keyword));
		} else if (temp->type == NON_TERMINAL) {
			printf("E");
		} else {
			printf("<");
		}
		temp = temp->next;
	}
	printf("\n");
}


char mapTokenToChar(KeyWord kw) {
	if (kw == num || kw == decim || kw == text || kw == id) {
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
			printf("=");
			return '=';
		case nequal:
			printf("!");
			return '=';
		case lequal:
			printf("<");
			return '=';
		case mequal:
			printf(">");
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