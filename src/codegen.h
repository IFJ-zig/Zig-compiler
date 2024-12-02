/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains declarations of functions used to generate IFJcode24
 *  @file codegen.h
 *  @author Daniel Bohata, xbohatd00
 *  @brief Header file for code generation for IFJcode24
*/

#include "ast.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CODEGEN
#define CODEGEN 1

// Dynamic array of exp nodes
typedef struct array_items
{
	ast_node_exp_t **nodes; // exp node array
	int capacity;           // maximum capacity based on allocated memory
	int size;               // number of currently stored exp nodes
} array_items_t;

// Dynamic array of variable names
typedef struct array_vars
{
	char **nodes; 			// strings array
	int capacity;           // maximum capacity based on allocated memory
	int size;               // number of currently stored strings
} array_vars_t;

/**
 * 	Defines all variables inside a body of code, going into depth
 *	@brief Defines all variables in a body of code
 *	@param nodes Body of code to analyse
 *	@param nodeCount Number of nodes to analyse
*/
void defAllVars(ast_default_node_t **nodes, unsigned int nodeCount);

/**
 * 	Function used to create []u8 format from an input string
 * 	@brief Creates []u8 format from a string
 * 	@param s Input string 
 */
void printString(char *s);

/**
 * 	Inbuilt ifj.string function
 * 	@brief Inbuilt ifj.string function
 * 	@param s Input string
 */
void stringToSlice(char *str);

/**
 * 	Function used to write input token based on the token type
 * 	@brief Inbuilt write function
 * 	@param token Token to be written
 */
void write(Token *token);

/**
 * 	Function that defines functions in IFJcode24
 * 	@brief Creates functions in IFJcode24
 * 	@param fn Function definition node that contains the function's information
 */
void functionDef(ast_node_fn_def_t *fn);

/**
 * 	Function that removes a selected character from a string
 * 	@brief Removes a character from a string
 * 	@param str String from which the character will be removed
 * 	@param ch Character to remove
 */
void removeChar(char *str, char ch); 

/**
 * 	Function that creates a function call in IFJcode24
 * 	@brief Creates a function call in IFJcode24
 * 	@param fn Node that contains information about the function we want to call
 */
void functionCall(ast_node_fn_call_t *fn);

/**
 * 	Function that creates a return with value in retval% if the function has it
 * 	@brief Creates a function's return with value
 * 	@param fn Node that contains information about the return
 */
void functionReturn(ast_node_fn_return_t *fn);

/**
 * 	Function that returns an initialized var array
 * 	@brief Creates a var array
 * 	@return Created var array
 */
array_vars_t *arrayInitVars();

/**
 * 	Function that adds an item to a var array
 * 	@brief Adds an item to a var array
 * 	@param var Variable name to add to var array
 * 	@param items Var array to be added to
 */
void arrayAddVarToItems(char *var, array_vars_t *items);

/**
 * 	Function that creates a definition of a variable in IFJcode24
 * 	@brief Creates a variable definition in IFJcode24
 * 	@param var Node that contains information about the variable we want do define
 */
void variableDefinition(ast_node_var_def_t *var);

/**
 * 	Function that assigns a value to a variable in IFJcode24
 * 	@brief Assigns a value to a variable in IFJcode24
 * 	@param var Node that contains information about the value that we want to assign
 */
void variableAssignment(ast_node_var_assign_t *var);

/**
 * 	Function that creates and initializes an array of exp nodes
 * 	@brief Creates and initializes an array of exp nodes
 * 	@return Created exp node array
 */
array_items_t *arrayInitItems();

/**
 * 	Function that adds an exp node to an array of exp nodes
 * 	@brief Adds an exp node to an array of exp nodes
 * 	@param node Exp node to be added to an array
 * 	@param items Exp node array to be added to
 */
void arrayAddNodeToItems(ast_node_exp_t *node, array_items_t *items);

/**
 * 	Function that sorts a binary exp node tree into a post-order sorted list
 * 	Algorhithm from IAL classes
 * 	@brief Creates a post-ordered list from exp node binary tree
 * 	@param tree Exp node from which the sorting begins
 * 	@param items Exp node array to which is the sorted tree put into
 */
void arrayPostorder(ast_node_exp_t *tree, array_items_t *items);

/**
 * 	Function that evaluates the outcome of an expression tree
 * 	@brief Evaluates the outcome of an expression tree
 * 	@param exp Root node of binary exp tree to be analysed
 */
void expression(ast_node_exp_t *exp);

/**
 * 	Function that creates a while loop in IFJcode24
 * 	@brief Creates a while loop in IFJcode24
 * 	@param loop Node that includes information about the while loop we want to create
 */
void whileLoop(ast_node_while_t *loop);

/**
 * 	Function that creates an if-else statement in IFJcode24
 * 	@brief Creates an if-else statement in IFJcode24
 * 	@param ifelse Node that contains information about the if-else statement we want to create
 */
void ifElse(ast_node_if_else_t *ifelse);

/**
 * 	Function that calls the correct function based on current node in a body of code
 * 	@brief Calls the correct function based on current node in a body of code
 * 	@param nodes Body of code made of nodes
 * 	@param nodeCount Number of nodes in the body of code
 */
void codebody(ast_default_node_t **nodes, unsigned int nodeCount);

/**
 * 	Function that prints the header of an IFJcode24 file
 * 	Includes definition of the global throwaway variable GF@_%
 * 	Includes definitions of inbuilt functions
 * 	@brief Prints the header of an IFJcode24 file
 */
void printHeader();

#endif
