/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains headers of functions used for expression parsing
 *  @file  expression_parser.c
 *  @author Adam Vožda, xvozdaa00
 *  @brief Header file for expression parsing using precedence analysis
 */


#ifndef HEXP_H__
#define HEXP_H__

#define TABLE_SIZE 15

#include <stdlib.h>

#include "errors_enum.h"
#include "semantic_an.h"
#include "stack.h"
#include "syntax_an.h"
#include "tokens.h"

/**
 * @brief Translates a token to its precedence table value.
 * 
 * @param token The token to be translated.
 * @return The precedence table value corresponding to the token.
 */
int precedentTableTranslator(int token);

/**
 * @brief Determines the operation based on the tokens from the stack and input.
 * 
 * @param tokenStack The token from the stack.
 * @param tokenInput The token from the input.
 * @return The operation to be performed.
 */
int getOperation(int tokenStack, int tokenInput);

/**
 * @brief Parses an expression and generates an abstract syntax tree (AST).
 * 
 * @param tokenRead Indicates whether a token has been read.
 * @param resultPointer Pointer to the resulting AST node.
 * @return Status code indicating success or failure.
 */
int expressionParser(bool tokenRead, ast_node_exp_t **resultPointer);

/**
 * @brief Attempts to match a rule in the parsing stack.
 * 
 * @param stack The parsing stack.
 * @return Status code indicating success or failure.
 */
int tryToMatchRule(t_Stack *stack);

/**
 * @brief Reduces a bracket non-terminal in the parsing stack.
 * 
 * @param stack The parsing stack.
 * @return Status code indicating success or failure.
 */
int reduceBracketNonTerminal(t_Stack *stack);

/**
 * @brief Creates a value expression node in the parsing stack.
 * 
 * @param stack The parsing stack.
 * @return Status code indicating success or failure.
 */
int createValueExp(t_Stack *stack);

/**
 * @brief Creates a binary expression node in the parsing stack.
 * 
 * @param stack The parsing stack.
 * @return Status code indicating success or failure.
 */
int createBinaryExp(t_Stack *stack);

/**
 * @brief Creates a unary expression node in the parsing stack.
 * 
 * @param stack The parsing stack.
 * @return Status code indicating success or failure.
 */
int createUnaryExp(t_Stack *stack);

#endif // HEXP_H__