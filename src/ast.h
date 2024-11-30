/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
*********************************************/

#ifndef AST_T
#define AST_T

#include <stdbool.h>

#include "symtable.h" //varType
#include "tokens.h"   //Token

typedef enum
{
	AST_NODE_FN_DEF,
	AST_NODE_FN_RETURN,
	AST_NODE_FN_CALL,
	AST_NODE_EXP,
	AST_NODE_VAR_DEF,
	AST_NODE_VAR_ASSIGN,
	AST_NODE_IF_ELSE,
	AST_NODE_WHILE,
	AST_NODE_DEFAULT, //Default node for the AST
} ast_node_type;


typedef struct ast_node_fn_def
{
	ast_node_type type;
	struct ast_default_node **body;
	unsigned int bodyCount;
	symbol_t *fnSymbol;
} ast_node_fn_def_t;


typedef struct ast_node_fn_call
{
	ast_node_type type;
	symbol_t *fnSymbol;
	struct ast_node_exp **args;
	unsigned int argCount;
} ast_node_fn_call_t;


typedef struct ast_node_fn_return
{
	ast_node_type type;
	Token *token;
	varType returnType;
	struct ast_node_exp *expression;
} ast_node_fn_return_t;


typedef struct ast_node_exp
{
	ast_node_type type;
	Token *token;
	varType dataType;
	struct
	{
		struct
		{
			struct ast_node_exp *exp;
		} unary_op;
		struct
		{
			struct ast_node_exp *left;
			struct ast_node_exp *right;
		} binary_op;
		struct ast_node_fn_call *fnCall;
	} data_t;
} ast_node_exp_t;


typedef struct ast_node_var_assign
{
	ast_node_type type;
	symbol_t *symbol;
	struct ast_node_exp *expression;
} ast_node_var_assign_t;


typedef struct ast_node_var_def
{
	ast_node_type type;
	symbol_t *symbol;
	struct ast_node_var_assign *assignment;
} ast_node_var_def_t;


typedef struct ast_node_if_else
{
	symbol_t *noNullPayload;
	ast_node_type type;
	struct ast_node_exp *conditionExp;
	struct ast_default_node **ifBlock;
	struct ast_default_node **elseBlock;
	bool elseBlockActive;
	unsigned int ifCount;
	unsigned int elseCount;
} ast_node_if_else_t;


typedef struct ast_node_while
{
	ast_node_type type;
	struct ast_node_exp *conditionExp;
	symbol_t *noNullPayload;
	struct ast_default_node **block;
	unsigned int blockCount;
} ast_node_while_t;


typedef struct ast_default_node
{
	ast_node_type type;
	struct ast_default_node *activeNode;
	union
	{
		struct ast_node_fn_def *fnDef;
		struct ast_node_fn_call *fnCall;
		struct ast_node_fn_return *fnReturn;
		struct ast_node_exp *exp;
		struct ast_node_var_assign *varAssign;
		struct ast_node_var_def *varDef;
		struct ast_node_if_else *ifElse;
		struct ast_node_while *While;

		struct body
		{
			struct ast_default_node **nodes;
			unsigned int nodeCount;
		} body_t;
	} data_t;
} ast_default_node_t;


void ast_init(ast_default_node_t *astRoot);
void ast_insert(ast_default_node_t *astRoot, ast_default_node_t *node);

ast_default_node_t *ast_createFnDefNode(symbol_t *fnSymbol);
ast_default_node_t *ast_createFnCallNode(symbol_t *fnSymbol);
ast_default_node_t *ast_createFnReturnNode(Token *token, varType returnType, ast_node_exp_t *expression);
ast_node_exp_t *ast_createExpNode(Token *token, varType dataType);
ast_node_var_assign_t *ast_createVarAssignNode(symbol_t *symbol, ast_node_exp_t *expression);
ast_default_node_t *ast_createVarDefNode(symbol_t *symbol, ast_node_var_assign_t *assignment);
ast_default_node_t *ast_createIfElseNode(ast_node_exp_t *conditionExp);
ast_default_node_t *ast_createWhileNode(ast_node_exp_t *conditionExp);

void printIndent(int depth);
void ast_print(ast_default_node_t *astRoot, int depth);
void ast_printExp(ast_node_exp_t *expNode, int indent);
void ast_destroy(ast_default_node_t *astRoot);

void ast_insertParam(ast_default_node_t *astRoot, ast_node_exp_t *expNode);

ast_default_node_t *ast_wrapVarAssignNode(ast_node_var_assign_t *assignNode);
ast_default_node_t *ast_wrapExpNode(ast_node_exp_t *expNode);


#endif // AST_T