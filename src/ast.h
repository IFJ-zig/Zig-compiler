/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
*********************************************/

#ifndef AST_T
#define AST_T

#include "tokens.h" //Token
#include "symtable.h" //varType
#include <stdbool.h>

typedef enum {
    AST_NODE_FN_PARAM,
    AST_NODE_FN_DEF,
    AST_NODE_FN_RETURN,
    AST_NODE_FN_CALL,   
    AST_NODE_EXP,       
    AST_NODE_VAR_DEF,
    AST_NODE_VAR_ASSIGN,
    AST_NODE_IF_ELSE,
    AST_NODE_WHILE,
    AST_NODE_DEFAULT,   //Default node for the AST
} ast_node_type;

typedef struct ast_node_fn_param{
    ast_node_type type;
    char *name;         //Same as symbol->key
    symbol_t *symbol;
} ast_node_fn_param_t;


typedef struct ast_node_fn_def{
    Token *token;
    ast_node_type type;
    ast_node_fn_param_t **params;
    unsigned int paramCount;
    varType returnType;

} ast_node_fn_def_t;


typedef struct ast_node_fn_call{
    ast_node_type type;
    Token *token;
    char *name;
    ast_node_exp_t **args;
    unsigned int argCount;
} ast_node_fn_call_t;


typedef struct ast_node_fn_return{
    ast_node_type type;
    Token *token;
    varType returnType;
    ast_node_exp_t *expression;
    unsigned int returnCount;
} ast_node_fn_return_t;


typedef struct ast_node_exp{
    ast_node_type type;
    Token *token;
    varType dataType;
    struct{
        struct{
            struct ast_node_exp *left;
            struct ast_node_exp *right;
        } binary_op;
        ast_node_fn_call_t *fnCall;
    } data_t
} ast_node_exp_t;


typedef struct ast_node_var_assign{
    ast_node_type type;
    Token *token;
    char *name;
    symbol_t *symbol;
    ast_node_exp_t *expression;
    unsigned int assignCount;
} ast_node_var_assign_t;


typedef struct ast_node_var_def{
    ast_node_type type;
    Token *token;
    char *name;
    symbol_t *symbol;
    ast_node_var_assign_t *assignment;
} ast_node_var_def_t;


typedef struct ast_node_if_else{
    ast_node_type type;
    ast_node_exp_t *conditionExp;
    ast_default_node_t **ifBlock;
    ast_default_node_t **elseBlock;
    unsigned int ifCount;
    unsigned int elseCount;
} ast_node_if_else_t;


typedef struct ast_node_while{
    ast_node_type type;
    ast_node_exp_t *conditionExp;
    ast_default_node_t **block;
    unsigned int blockCount;
} ast_node_while_t;


typedef struct ast_default_node{
    ast_node_type type;
    union {
        ast_node_fn_param_t *fnParam;
        ast_node_fn_def_t *fnDef;

        struct body{
            ast_default_node_t **nodes;
            unsigned int nodeCount;
        } body_t;
    } data_t;
} ast_default_node_t;


/*
void ast_init(ast_t *ast);
void ast_insert(ast_t *ast, bool insertLeft, Token *token);   //Diverging from IAL2, we don't need a key since this will not be a balanced tree and determining whether to go right or left will be done somewhere else
void ast_destroy(ast_t *ast);
void ast_destroy_node(ast_node_t *node); //Helper function for ast_destroy so that we don't need to use a stack here
*/
#endif // AST_T