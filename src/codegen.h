#include "ast.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef CODEGEN
#define CODEGEN 1

typedef struct bst_items
{
	ast_node_exp_t **nodes; // pole uzlu
	int capacity;           // kapacita alokované paměti v počtu položek
	int size;               // aktuální velikost pole v počtu položek
} bst_items_t;

typedef struct bst_vars
{
	char **nodes; // pole uzlu
	int capacity;           // kapacita alokované paměti v počtu položek
	int size;               // aktuální velikost pole v počtu položek
} bst_vars_t;

void defAllVars(ast_default_node_t **nodes, unsigned int nodeCount); //
void printHeader();
void write(Token *token);
void variableDefinition(ast_node_var_def_t *var);
void variableAssignment(ast_node_var_assign_t *var);
void expression(ast_node_exp_t *exp);
void functionDef(ast_node_fn_def_t *fn);
void codebody(ast_default_node_t **nodes, unsigned int nodeCount);
void whileLoop(ast_node_while_t *loop);
void functionReturn(ast_node_fn_return_t *fn);
void functionCall(ast_node_fn_call_t *fn);
void removeChar(char *str, char garbage);
void stringToSlice(char *str);
void ifElse(ast_node_if_else_t *ifelse);

bst_vars_t *bst_init_vars();
void bst_add_var_to_items(char *var, bst_vars_t *items);




#endif