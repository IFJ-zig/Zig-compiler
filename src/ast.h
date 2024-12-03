/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains declaration of functions used for abstract syntax tree and its nodes
 *  @file  ast.h
 *  @author Tadeáš Horák, xhorakt00
 *  @brief Declaration file for abstract syntax tree
 */

#ifndef AST_T
#define AST_T

#include <stdbool.h>

#include "symtable.h" //varType
#include "tokens.h"   //Token

/**
 * @brief Enum for the type of the AST node
 */
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

/**
 * @brief Struct for the function definition node
 */
typedef struct ast_node_fn_def
{
	ast_node_type type;
	struct ast_default_node **body;
	unsigned int bodyCount;
	symbol_t *fnSymbol;
} ast_node_fn_def_t;

/**
 * @brief Struct for the function call node
 */
typedef struct ast_node_fn_call
{
	ast_node_type type;
	symbol_t *fnSymbol;
	struct ast_node_exp **args;
	unsigned int argCount;
} ast_node_fn_call_t;

/**
 * @brief Struct for the function return node
 */
typedef struct ast_node_fn_return
{
	ast_node_type type;
	Token *token;
	varType returnType;
	struct ast_node_exp *expression;
} ast_node_fn_return_t;


/**
 * @brief Struct for the expression node
 */
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


/**
 * @brief Struct for the variable assignment node
 */
typedef struct ast_node_var_assign
{
	ast_node_type type;
	symbol_t *symbol;
	struct ast_node_exp *expression;
} ast_node_var_assign_t;


/**
 * @brief Struct for the variable definition node
 */
typedef struct ast_node_var_def
{
	ast_node_type type;
	symbol_t *symbol;
	struct ast_node_var_assign *assignment;
} ast_node_var_def_t;


/**
 * @brief Struct for the if-else node
 */
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


/**
 * @brief Struct for the while node
 */
typedef struct ast_node_while
{
	ast_node_type type;
	struct ast_node_exp *conditionExp;
	symbol_t *noNullPayload;
	struct ast_default_node **block;
	unsigned int blockCount;
} ast_node_while_t;


/**
 * @brief Struct for the default and root node
 */
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

/**
 * @brief Initialize the AST
 * 
 * @param astRoot The root node of the AST
 */
void ast_init(ast_default_node_t *astRoot);

/**
 * @brief Insert a node into the AST
 * 
 * @param astRoot The root node of the AST
 * @param node The node to be inserted
 */
void ast_insert(ast_default_node_t *astRoot, ast_default_node_t *node);

/**
 * @brief Create a function definition node
 * 
 * @param fnSymbol The function symbol to be inserted into the node
 * 
 * @return Returns a pointer to the newly created node
 */
ast_default_node_t *ast_createFnDefNode(symbol_t *fnSymbol);

/**
 * @brief Create a function call node
 * 
 * @param fnSymbol The function symbol to be inserted into the node
 * 
 * @return Returns a pointer to the newly created node
 */
ast_default_node_t *ast_createFnCallNode(symbol_t *fnSymbol);

/**
 * @brief Create a function return node
 * 
 * @param token The token to be inserted into the node
 * @param returnType The return type of the function
 * @param expression Expression inside the return statement
 * 
 * @return Returns a pointer to the newly created node
 */
ast_default_node_t *ast_createFnReturnNode(Token *token, varType returnType, ast_node_exp_t *expression);

/**
 * @brief Create an expression node
 * 
 * @param token The token to be inserted into the node (contains keywords)
 * @param dataType The data type of the expression
 * 
 * @return Returns a pointer to the newly created expression node
 */
ast_node_exp_t *ast_createExpNode(Token *token, varType dataType);

/**
 * @brief Create a variable definition node
 * 
 * @param symbol The symbol to be inserted into the node
 * @param expression value assignment into the variable
 * 
 * @return Returns a pointer to the newly created node
 */
ast_node_var_assign_t *ast_createVarAssignNode(symbol_t *symbol, ast_node_exp_t *expression);

/**
 * @brief Create a variable definition node
 * 
 * @param symbol The symbol to be inserted into the node
 * @param assignment value assignment into the variable
 * 
 * @note assignment can be NULL if variable is only defined
 * 
 * @return Returns a pointer to the newly created node
 */
ast_default_node_t *ast_createVarDefNode(symbol_t *symbol, ast_node_var_assign_t *assignment);

/**
 * @brief Create an if-else node
 * 
 * @param conditionExp The condition for the if-else statement
 * 
 * @return Returns a pointer to the newly created node
 */
ast_default_node_t *ast_createIfElseNode(ast_node_exp_t *conditionExp);

/**
 * @brief Create a while node
 * 
 * @param conditionExp The condition for the while loop
 * 
 * @return Returns a pointer to the newly created node
 */
ast_default_node_t *ast_createWhileNode(ast_node_exp_t *conditionExp);


/**
 * @brief Prints spaces to stderr to allow for a better visual representation of the AST
 * 
 * @param indentSize The number of tabs to be printed (4 spaces per tab)
 */
void printIndent(int indentSize);

/**
 * @brief Prints the whole AST, including expressions
 * 
 * @param astRoot The root node of the AST
 * @param depth The depth inside the AST, used for indentation, needed to be able to call the function recursively, by default set to 0
 * 
 * @note set depth to 0 when calling the function
 */
void ast_print(ast_default_node_t *astRoot, int depth);

/**
 * @brief Prints the expression node
 * 
 * @param expNode The expression node to be printed
 * @param indent The indentation level, used for better visual representation inside the AST
 */
void ast_printExp(ast_node_exp_t *expNode, int indent);

/**
 * @brief Destroys the AST
 * 
 * @param expNode The root node of the AST
 */
void ast_destroy(ast_default_node_t *astRoot);

/**
 * @brief Inserts a parameter into the function call node
 * 
 * @param astRoot The root node of the AST
 * @param expNode The expression node to be inserted
 * 
 * @note this function will increase the size of memory allocated for the function call node -> args
 */
void ast_insertParam(ast_default_node_t *astRoot, ast_node_exp_t *expNode);

/**
 * @brief wraps the variable assignment node into the default node for insertion into the AST
 * 
 * @param assignNode The variable assignment node to be wrapped
 * 
 * @note this function exists because sometimes we need only the variable assignment node eg when dealing with a variable definition node with assignment
 * 
 * @return Returns a pointer to the newly created node
 */
ast_default_node_t *ast_wrapVarAssignNode(ast_node_var_assign_t *assignNode);


#endif // AST_T