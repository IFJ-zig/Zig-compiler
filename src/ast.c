/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
*********************************************/

#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

#include "errors_enum.h"


void ast_init(ast_default_node_t *ast) {
	ast->type = AST_NODE_DEFAULT;
	ast->data_t.body_t.nodes = NULL;
	ast->data_t.body_t.nodeCount = 0;
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
}*/
