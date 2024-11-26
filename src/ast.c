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

void ast_insertToRoot(ast_default_node_t *astRoot, ast_default_node_t *node) {
    astRoot->data_t.body_t.nodeCount++;
    astRoot->data_t.body_t.nodes = (ast_default_node_t **)realloc(astRoot->data_t.body_t.nodes, astRoot->data_t.body_t.nodeCount * sizeof(ast_default_node_t *));
    if (astRoot->data_t.body_t.nodes == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    astRoot->data_t.body_t.nodes[astRoot->data_t.body_t.nodeCount - 1] = node;
}

void ast_insertToFnDef(ast_node_fn_def_t *fnDef, ast_default_node_t *node) {
    fnDef->bodyCount++;
    fnDef->body = (ast_default_node_t **)realloc(fnDef->body, (fnDef->fnSymbol->paramCount + 1) * sizeof(ast_default_node_t *));
    if (fnDef->body == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    fnDef->body[fnDef->bodyCount - 1] = node;
}

void ast_insertToFnCall(ast_node_fn_call_t *fnCall, ast_node_exp_t *node){
    fnCall->argCount++;
    fnCall->args = (ast_node_exp_t **)realloc(fnCall->args, fnCall->argCount * sizeof(ast_node_exp_t *));
    if (fnCall->args == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    fnCall->args[fnCall->argCount - 1] = node;
}

void ast_insertToFnReturn(ast_node_fn_return_t *fnReturn, ast_node_exp_t *node){
    fnReturn->returnCount++;
    fnReturn->expression = (ast_node_exp_t **)realloc(fnReturn->expression, fnReturn->returnCount * sizeof(ast_node_exp_t *));
    if (fnReturn->expression == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    fnReturn->expression[fnReturn->returnCount - 1] = node;
}

void ast_insertToIf(ast_node_if_else_t *ifBlock, ast_default_node_t *node){
    ifBlock->ifCount++;
    ifBlock->ifBlock = (ast_default_node_t **)realloc(ifBlock->ifBlock, ifBlock->ifCount * sizeof(ast_default_node_t *));
    if (ifBlock->ifBlock == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    ifBlock->ifBlock[ifBlock->ifCount - 1] = node;
}

void ast_insertToElse(ast_node_if_else_t *elseBlock, ast_default_node_t *node){
    elseBlock->elseCount++;
    elseBlock->elseBlock = (ast_default_node_t **)realloc(elseBlock->elseBlock, elseBlock->elseCount * sizeof(ast_default_node_t *));
    if (elseBlock->elseBlock == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    elseBlock->elseBlock[elseBlock->elseCount - 1] = node;
}

void ast_insertToWhile(ast_node_while_t *whileBlock, ast_default_node_t *node){
    whileBlock->blockCount++;
    whileBlock->block = (ast_default_node_t **)realloc(whileBlock->block, whileBlock->blockCount * sizeof(ast_default_node_t *));
    if (whileBlock->block == NULL) {
        fprintf(stderr, "Error: Memory allocation for ast failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    whileBlock->block[whileBlock->blockCount - 1] = node;
}


/*
void ast_insert(ast_t *ast, bool insertLeft, Token *token){
    ast_node_t *newNode = (ast_node_t *)malloc(sizeof(ast_node_t));
    if (newNode == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    newNode->token = token;
    newNode->left = NULL;
    newNode->right = NULL;

    if (ast->root == NULL)
    {
        ast->root = newNode;
        ast->activeNode = newNode;
    }
    else
    {
        if (insertLeft)
        {
            ast->activeNode->left = newNode;
        }
        else
        {
            ast->activeNode->right = newNode;
        }
    }
    ast->activeNode = newNode;
    ast->nodeCount++;
}

void ast_destroy(ast_t *ast){
    if(ast->root != NULL){
        ast_destroy_node(ast->root);
    }
    free(ast);
}

void ast_destroy_node(ast_node_t *node){
    if(node->left != NULL){
        ast_destroy_node(node->left);
    }
    if(node->right != NULL){
        ast_destroy_node(node->right);
    }
    free(node);
}
*/