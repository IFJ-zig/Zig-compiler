#include "ast.h"

#ifndef CODEGEN
#define CODEGEN 1

void printHeader();
void write(Token *token);
void variableDefinition(ast_node_var_def_t *var);
void variableAssignment(ast_node_var_assign_t *var);
void expression(ast_node_exp_t *exp);
void functionDef(ast_node_fn_def_t *fn);
void codebody(ast_default_node_t **nodes, unsigned int nodeCount);
void whileLoop(ast_node_while_t *loop);


typedef struct bst_items
{
	ast_node_exp_t **nodes; // pole uzlu
	int capacity;           // kapacita alokované paměti v počtu položek
	int size;               // aktuální velikost pole v počtu položek
} bst_items_t;

#endif