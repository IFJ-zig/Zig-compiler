/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains declarations of functions used to generate IFJcode24
 *  @file codegen.h
 *  @author Daniel Bohata, xbohatd00
 *  @brief Header file for code generation for IFJcode24
 * 
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

void defAllVars(ast_default_node_t **nodes, unsigned int nodeCount); //
void printString(char *s); //
void stringToSlice(char *str);//
void write(Token *token); //
void functionDef(ast_node_fn_def_t *fn);//
void removeChar(char *str, char ch); //
void functionCall(ast_node_fn_call_t *fn);//
void functionReturn(ast_node_fn_return_t *fn);//
array_vars_t *arrayInitVars();//
void arrayAddVarToItems(char *var, array_vars_t *items);//
void variableDefinition(ast_node_var_def_t *var);//
void variableAssignment(ast_node_var_assign_t *var);//
array_items_t *arrayInitItems();//
void arrayAddNodeToItems(ast_node_exp_t *node, array_items_t *items);//
void arrayPostorder(ast_node_exp_t *tree, array_items_t *items);//
void expression(ast_node_exp_t *exp);//
void whileLoop(ast_node_while_t *loop);//
void ifElse(ast_node_if_else_t *ifelse);//
void codebody(ast_default_node_t **nodes, unsigned int nodeCount);//
void printHeader();//

#endif
