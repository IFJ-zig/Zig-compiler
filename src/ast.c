/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
*********************************************/

#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

#include "errors_enum.h"

void ast_init(ast_default_node_t *astRoot) {
	astRoot->type = AST_NODE_DEFAULT;
	astRoot->data_t.body_t.nodes = NULL;
	astRoot->data_t.body_t.nodeCount = 0;
}

void ast_insert(ast_default_node_t *astRoot, ast_default_node_t *node) {
    astRoot->data_t.body_t.nodeCount++;
    astRoot->data_t.body_t.nodes = (ast_default_node_t **)realloc(astRoot->data_t.body_t.nodes, astRoot->data_t.body_t.nodeCount * sizeof(ast_default_node_t *));
    if (astRoot->data_t.body_t.nodes == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    astRoot->data_t.body_t.nodes[astRoot->data_t.body_t.nodeCount - 1] = node;
}

ast_default_node_t *ast_create_node(ast_node_type type) {
    ast_default_node_t *newNode = (ast_default_node_t *)malloc(sizeof(ast_default_node_t));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    newNode->type = type;
    return newNode;
}

ast_node_fn_def_t *ast_createFnDefNode(symbol_t *fnSymbol) {
    ast_node_fn_def_t *newNode = (ast_node_fn_def_t *)malloc(sizeof(ast_node_fn_def_t));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    newNode->type = AST_NODE_FN_DEF;
    newNode->fnSymbol = fnSymbol;
    newNode->body = NULL;
    newNode->bodyCount = 0;
    return newNode;
}

ast_node_fn_call_t *ast_createFnCallNode(symbol_t *fnSymbol, ast_node_exp_t **args, unsigned int argCount) {
    ast_node_fn_call_t *newNode = (ast_node_fn_call_t *)malloc(sizeof(ast_node_fn_call_t));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    newNode->type = AST_NODE_FN_CALL;
    newNode->fnSymbol = fnSymbol;
    newNode->args = args;
    newNode->argCount = argCount;
    return newNode;
}

ast_node_fn_return_t *ast_createFnReturnNode(Token *token, varType returnType, ast_node_exp_t *expression) {
    ast_node_fn_return_t *newNode = (ast_node_fn_return_t *)malloc(sizeof(ast_node_fn_return_t));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    newNode->type = AST_NODE_FN_RETURN;
    newNode->token = token;
    newNode->returnType = returnType;
    newNode->expression = expression;
    return newNode;
}

ast_node_exp_t *ast_createExpNode(Token *token, varType dataType) {
    ast_node_exp_t *newNode = (ast_node_exp_t *)malloc(sizeof(ast_node_exp_t));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    newNode->type = AST_NODE_EXP;
    newNode->token = token;
    newNode->dataType = dataType;
    newNode->data_t.unary_op.exp = NULL;
    newNode->data_t.binary_op.left = NULL;
    newNode->data_t.binary_op.right = NULL;
    newNode->data_t.fnCall = NULL;
    return newNode;
}

ast_node_var_assign_t *ast_createVarAssignNode(symbol_t *symbol, ast_node_exp_t *expression) {
    ast_node_var_assign_t *newNode = (ast_node_var_assign_t *)malloc(sizeof(ast_node_var_assign_t));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    newNode->type = AST_NODE_VAR_ASSIGN;
    newNode->symbol = symbol;
    newNode->expression = expression;
    return newNode;
}

ast_node_var_def_t *ast_createVarDefNode(symbol_t *symbol, ast_node_var_assign_t *assignment) {
    ast_node_var_def_t *newNode = (ast_node_var_def_t *)malloc(sizeof(ast_node_var_def_t));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    newNode->type = AST_NODE_VAR_DEF;
    newNode->symbol = symbol;
    newNode->assignment = assignment;
    return newNode;
}

ast_node_if_else_t *ast_createIfElseNode(ast_node_exp_t *conditionExp) {
    ast_node_if_else_t *newNode = (ast_node_if_else_t *)malloc(sizeof(ast_node_if_else_t));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    newNode->type = AST_NODE_IF_ELSE;
    newNode->conditionExp = conditionExp;
    newNode->ifBlock = NULL;
    newNode->elseBlock = NULL;
    newNode->ifCount = 0;
    newNode->elseCount = 0;
    return newNode;
}

ast_node_while_t *ast_createWhileNode(ast_node_exp_t *conditionExp) {
    ast_node_while_t *newNode = (ast_node_while_t *)malloc(sizeof(ast_node_while_t));
    if (newNode == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    newNode->type = AST_NODE_WHILE;
    newNode->conditionExp = conditionExp;
    newNode->block = NULL;
    newNode->blockCount = 0;
    return newNode;
}