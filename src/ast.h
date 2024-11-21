/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
*********************************************/

#ifndef AST_T
#define AST_T

#include "tokens.h" //Token
#include <stdbool.h>

typedef struct ast_node
{
    Token *token;
    struct SNode *left;
    struct SNode *right;
} ast_node_t;

typedef struct ast {
    ast_node_t *root;
    ast_node_t *activeNode;
    int nodeCount; 
} ast_t;

void ast_init(ast_t *ast);
void ast_insert(ast_t *ast, bool insertLeft, Token *token);   //Diverging from IAL2, we don't need a key since this will not be a balanced tree and determining whether to go right or left will be done somewhere else
void ast_destroy(ast_t *ast);
void ast_destroy_node(ast_node_t *node); //Helper function for ast_destroy so that we don't need to use a stack here

#endif // AST_T