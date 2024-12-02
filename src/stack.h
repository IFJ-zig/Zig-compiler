/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains declarations of functions used to manipulate stack for precedence analysis
 *  @file stack.h
 *  @author Adam Vožda, xvozdaa00
 *  @brief Header file for stack for precedence analysis
 */

#ifndef HSTACK_H__
#define HSTACK_H__

#include <stdbool.h>
#include <stdlib.h>

#include "ast.h"
#include "errors_enum.h"
#include "tokens.h"

/**
 * @enum stackElementType
 * @brief Enum representing the type of stack element.
 */
typedef enum
{
	TERMINAL,       /**< Terminal element */
	NON_TERMINAL,   /**< Non-terminal element */
	PRECEDENT_LESS, /**< Precedent-less element */
} stackElementType;

/**
 * @struct Stack
 * @brief Structure representing the stack.
 */
typedef struct Stack
{
	struct StackItem *top; /**< Pointer to the top item of the stack */
	int size;              /**< Current size of the stack */
} t_Stack;

/**
 * @struct StackItem
 * @brief Structure representing an item in the stack.
 */
typedef struct StackItem
{
	stackElementType type;  /**< Type of the stack element */
	Token *token;           /**< Pointer to the token associated with the stack item */
	struct StackItem *next; /**< Pointer to the next item in the stack */
	struct StackItem *prev; /**< Pointer to the previous item in the stack */
	ast_node_exp_t *node;   /**< Pointer to the AST node associated with the stack item */
} t_StackItem;

/**
 * @brief Initializes the stack.
 *
 * @param stack Pointer to the stack to be initialized.
 */
void stackInit(t_Stack *stack);

/**
 * @brief Pushes an item onto the stack.
 *
 * @param stack Pointer to the stack.
 * @param type Type of the stack element.
 * @param tkn Pointer to the token associated with the stack item.
 * @param node Pointer to the AST node associated with the stack item.
 * @return int Status code (0 for success, non-zero for failure).
 */
int stackPush(t_Stack *stack, stackElementType type, Token *tkn, ast_node_exp_t *node);

/**
 * @brief Pops the top item from the stack.
 *
 * @param stack Pointer to the stack.
 */
void stackPop(t_Stack *stack);

/**
 * @brief Returns the top item of the stack.
 *
 * @param stack Pointer to the stack.
 * @return t_StackItem* Pointer to the top item of the stack.
 */
t_StackItem *stackTop(t_Stack *stack);

/**
 * @brief Clears all items from the stack.
 *
 * @param stack Pointer to the stack.
 */
void stackClear(t_Stack *stack);

/**
 * @brief Pushes a precedent-less item onto the stack.
 *
 * @param stack Pointer to the stack.
 * @return int Status code (0 for success, non-zero for failure).
 */
int stackPushPrecedentLess(t_Stack *stack);

/**
 * @brief Returns the top terminal item of the stack.
 *
 * @param stack Pointer to the stack.
 * @return t_StackItem* Pointer to the top terminal item of the stack.
 */
t_StackItem *topTerminal(t_Stack *stack);

/**
 * @brief Prints the contents of the stack.
 *
 * @param stack Pointer to the stack.
 */
void printStack(t_Stack *stack);

/**
 * @brief Maps a token keyword to a character.
 *
 * @param kw Keyword to be mapped.
 * @return char Mapped character.
 */
char mapTokenToChar(KeyWord kw);

#endif // HSTACK_H__