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
	astRoot->activeNode = NULL;
}

char *indentNode(int depth) {
	depth *= 4;
	char *indent = (char *)malloc(depth + 1);
	if (indent == NULL) {
		fprintf(stderr, "Error: Memory allocation for ast failed\n");
		exit(INTERNAL_COMPILER_ERROR);
	}
	for (int i = 0; i < depth; i++) {
		indent[i] = ' ';
	}
	indent[depth] = '\0';
	return indent;
}

void ast_print(ast_default_node_t *astRoot, int depth) {
	//fprintf(stderr, "Printing AST %d %d\n", astRoot->type, astRoot->data_t.body_t.nodeCount);
	if(astRoot == NULL) {
		fprintf(stderr, "AST IS NULL!\n");
	}
	if (astRoot->type == AST_NODE_DEFAULT) {
		for (unsigned int i = 0; i < astRoot->data_t.body_t.nodeCount; i++) {
			ast_print(astRoot->data_t.body_t.nodes[i], depth);
		}
	} else if (astRoot->type == AST_NODE_FN_DEF) {
		fprintf(stderr, "%sFunction definition - %s\n", indentNode(depth), astRoot->data_t.fnDef->fnSymbol->key);
		for (unsigned int i = 0; i < astRoot->data_t.fnDef->bodyCount; i++) {
			ast_print(astRoot->data_t.fnDef->body[i], depth + 1);
		}
	} else if (astRoot->type == AST_NODE_FN_CALL) {
		fprintf(stderr, "%sFunction call - %s, argCount(Node)=%d\n", indentNode(depth), astRoot->data_t.fnCall->fnSymbol->key, astRoot->data_t.fnCall->argCount);
		for(unsigned int i = 0; i < astRoot->data_t.fnCall->argCount; i++){
			varType type = astRoot->data_t.fnCall->args[i]->dataType;
			fprintf(stderr, "%s->arg[%d] = %s", indentNode(depth+2), i, type == INT ? "INT" : type == FLOAT ? "FLOAT" : type == STRING ? "STRING" : type == ANYTYPE ? "ANYTYPE" : "VOID");
		}
		fprintf(stderr, "\n");
	} else if (astRoot->type == AST_NODE_FN_RETURN) {
		fprintf(stderr, "%sFunction return\n", indentNode(depth));
		fprintf(stderr, "%s->expression = ", indentNode(depth+1));
		if(astRoot->data_t.fnReturn->expression != NULL) {
			fprintf(stderr, "%d\n", astRoot->data_t.fnReturn->expression->token->kw);
		} else {
			fprintf(stderr, "NULL\n");
		}
	} else if (astRoot->type == AST_NODE_EXP) {
		fprintf(stderr, "%sExpression\n", indentNode(depth));
	} else if (astRoot->type == AST_NODE_VAR_DEF) {
		fprintf(stderr, "%sVariable definition\n", indentNode(depth));
		if(astRoot->data_t.varDef->assignment != NULL) {
			fprintf(stderr, "%s Variable assignment\n", indentNode(depth+1));
			fprintf(stderr, "%s->expression = ", indentNode(depth+2));
			if(astRoot->data_t.varDef->assignment->expression != NULL) {
				fprintf(stderr, "%d\n", astRoot->data_t.varDef->assignment->expression->token->kw);
			} else {
				fprintf(stderr, "NULL\n");
			}
		} else {
			fprintf(stderr, "%s->assignment = NULL\n", indentNode(depth+1));
		}
	} else if (astRoot->type == AST_NODE_VAR_ASSIGN) {
		fprintf(stderr, "%sVariable assignment\n", indentNode(depth));
		fprintf(stderr, "%s->expression = ", indentNode(depth+1));
		if(astRoot->data_t.varAssign->expression != NULL) {
			fprintf(stderr, "%d\n", astRoot->data_t.varAssign->expression->token->kw);
		} else {
			fprintf(stderr, "NULL\n");
		}
	} else if (astRoot->type == AST_NODE_IF_ELSE) {
		fprintf(stderr, "%sIf-else - ifBlockCount=%d, elseBlockCount=%d\n", indentNode(depth), astRoot->data_t.ifElse->ifCount, astRoot->data_t.ifElse->elseCount);
		fprintf(stderr, "%sifBlock\n", indentNode(depth+1));
		if(astRoot->data_t.ifElse->ifCount != 0){
			for (unsigned int i = 0; i < astRoot->data_t.ifElse->ifCount; i++) {
				ast_print(astRoot->data_t.ifElse->ifBlock[i], depth + 2);
			}
		}
		else
			fprintf(stderr, "%s->Empty\n", indentNode(depth+2));
		fprintf(stderr, "%selseBlock\n", indentNode(depth+1));
		if(astRoot->data_t.ifElse->elseCount != 0){
			for (unsigned int i = 0; i < astRoot->data_t.ifElse->elseCount; i++) {
				ast_print(astRoot->data_t.ifElse->elseBlock[i], depth + 2);
			}
		}
		else
			fprintf(stderr, "%s->Empty\n", indentNode(depth+2));
	} else if (astRoot->type == AST_NODE_WHILE) {
		fprintf(stderr, "%sWhile\n", indentNode(depth));
		fprintf(stderr, "%sBlock\n", indentNode(depth+1));
		if(astRoot->data_t.While->blockCount != 0){
			for (unsigned int i = 0; i < astRoot->data_t.While->blockCount; i++) {
				ast_print(astRoot->data_t.While->block[i], depth + 1);
			}
		}
		else
			fprintf(stderr, "%s->Empty\n", indentNode(depth+2));
	}
}

void ast_insert(ast_default_node_t *astRoot, ast_default_node_t *node) {
	switch (astRoot->type) {
		case AST_NODE_DEFAULT:
			astRoot->data_t.body_t.nodeCount++;
			astRoot->data_t.body_t.nodes = (ast_default_node_t **)realloc(astRoot->data_t.body_t.nodes, astRoot->data_t.body_t.nodeCount * sizeof(ast_default_node_t *));
			if (astRoot->data_t.body_t.nodes == NULL) {
				fprintf(stderr, "Error: Memory allocation for ast failed\n");
				exit(INTERNAL_COMPILER_ERROR);
			}
			astRoot->data_t.body_t.nodes[astRoot->data_t.body_t.nodeCount - 1] = node;
			break;
		case AST_NODE_FN_DEF:
			astRoot->data_t.fnDef->bodyCount++;
			astRoot->data_t.fnDef->body = (ast_default_node_t **)realloc(astRoot->data_t.fnDef->body, astRoot->data_t.fnDef->bodyCount * sizeof(ast_default_node_t *));
			if (astRoot->data_t.fnDef->body == NULL) {
				fprintf(stderr, "Error: Memory allocation for ast failed\n");
				exit(INTERNAL_COMPILER_ERROR);
			}
			astRoot->data_t.fnDef->body[astRoot->data_t.fnDef->bodyCount - 1] = node;
			break;
		case AST_NODE_WHILE:
			astRoot->data_t.While->blockCount++;
			astRoot->data_t.While->block = (ast_default_node_t **)realloc(astRoot->data_t.While->block, astRoot->data_t.While->blockCount * sizeof(ast_default_node_t *));
			if (astRoot->data_t.While->block == NULL) {
				fprintf(stderr, "Error: Memory allocation for ast failed\n");
				exit(INTERNAL_COMPILER_ERROR);
			}
			astRoot->data_t.While->block[astRoot->data_t.While->blockCount - 1] = node;
			break;
		case AST_NODE_IF_ELSE:
			if(astRoot->data_t.ifElse->elseBlockActive){
				astRoot->data_t.ifElse->elseCount++;
				astRoot->data_t.ifElse->elseBlock = (ast_default_node_t **)realloc(astRoot->data_t.ifElse->elseBlock, astRoot->data_t.ifElse->elseCount * sizeof(ast_default_node_t *));
				if (astRoot->data_t.ifElse->elseBlock == NULL) {
					fprintf(stderr, "Error: Memory allocation for ast failed\n");
					exit(INTERNAL_COMPILER_ERROR);
				}
				astRoot->data_t.ifElse->elseBlock[astRoot->data_t.ifElse->elseCount - 1] = node;
			} else {
				astRoot->data_t.ifElse->ifCount++;
				astRoot->data_t.ifElse->ifBlock = (ast_default_node_t **)realloc(astRoot->data_t.ifElse->ifBlock, astRoot->data_t.ifElse->ifCount * sizeof(ast_default_node_t *));
				if (astRoot->data_t.ifElse->ifBlock == NULL) {
					fprintf(stderr, "Error: Memory allocation for ast failed\n");
					exit(INTERNAL_COMPILER_ERROR);
				}
				astRoot->data_t.ifElse->ifBlock[astRoot->data_t.ifElse->ifCount - 1] = node;
			}
			break;
		default:
			fprintf(stderr, "Error: Inserting into node type %d not supported by this function\n", astRoot->type);
			//exit(INTERNAL_COMPILER_ERROR);
	}
}

void ast_insertParam(ast_default_node_t *astRoot, ast_node_exp_t *expNode) {
	astRoot->data_t.fnCall->args = (ast_node_exp_t **)realloc(astRoot->data_t.fnCall->args, (astRoot->data_t.fnCall->argCount + 1) * sizeof(ast_node_exp_t *));
	if (astRoot->data_t.fnCall->args == NULL) {
		fprintf(stderr, "Error: Memory allocation for ast failed\n");
		exit(INTERNAL_COMPILER_ERROR);
	}
	astRoot->data_t.fnCall->args[astRoot->data_t.fnCall->argCount] = expNode;
	astRoot->data_t.fnCall->argCount++;
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

ast_default_node_t *ast_createFnDefNode(symbol_t *fnSymbol) {
	ast_node_fn_def_t *newNode = (ast_node_fn_def_t *)malloc(sizeof(ast_node_fn_def_t));
	if (newNode == NULL) {
		fprintf(stderr, "Error: Memory allocation for ast failed\n");
		exit(INTERNAL_COMPILER_ERROR);
	}
	newNode->type = AST_NODE_FN_DEF;
	newNode->fnSymbol = fnSymbol;
	newNode->body = malloc(sizeof(ast_default_node_t *));
	newNode->bodyCount = 0;
	ast_default_node_t *defaultNode = ast_create_node(AST_NODE_DEFAULT);
	defaultNode->data_t.fnDef = newNode;
	defaultNode->type = AST_NODE_FN_DEF;
	return defaultNode;
}

ast_default_node_t *ast_createFnCallNode(symbol_t *fnSymbol) {
	ast_node_fn_call_t *newNode = (ast_node_fn_call_t *)malloc(sizeof(ast_node_fn_call_t));
	if (newNode == NULL) {
		fprintf(stderr, "Error: Memory allocation for ast failed\n");
		exit(INTERNAL_COMPILER_ERROR);
	}
	newNode->type = AST_NODE_FN_CALL;
	newNode->fnSymbol = fnSymbol;
	newNode->args = malloc(sizeof(ast_node_exp_t *));
	newNode->argCount = 0;
	ast_default_node_t *defaultNode = ast_create_node(AST_NODE_DEFAULT);
	defaultNode->data_t.fnCall = newNode;
	defaultNode->type = AST_NODE_FN_CALL;
	return defaultNode;
}

ast_default_node_t *ast_createFnReturnNode(Token *token, varType returnType, ast_node_exp_t *expression) {
	ast_node_fn_return_t *newNode = (ast_node_fn_return_t *)malloc(sizeof(ast_node_fn_return_t));
	if (newNode == NULL) {
		fprintf(stderr, "Error: Memory allocation for ast failed\n");
		exit(INTERNAL_COMPILER_ERROR);
	}
	newNode->type = AST_NODE_FN_RETURN;
	newNode->token = token;
	newNode->returnType = returnType;
	newNode->expression = expression;
	ast_default_node_t *defaultNode = ast_create_node(AST_NODE_DEFAULT);
	defaultNode->data_t.fnReturn = newNode;
	defaultNode->type = AST_NODE_FN_RETURN;
	return defaultNode;
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

ast_default_node_t *ast_wrapExpNode(ast_node_exp_t *expNode) {
	ast_default_node_t *defaultNode = ast_create_node(AST_NODE_DEFAULT);
	defaultNode->data_t.exp = (struct ast_node_fn_exp *)expNode;
	defaultNode->type = AST_NODE_EXP;
	return defaultNode;
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

ast_default_node_t *ast_wrapVarAssignNode(ast_node_var_assign_t *assignNode) {
	ast_default_node_t *defaultNode = ast_create_node(AST_NODE_DEFAULT);
	defaultNode->data_t.varAssign = assignNode;
	defaultNode->type = AST_NODE_VAR_ASSIGN;
	return defaultNode;
}

ast_default_node_t *ast_createVarDefNode(symbol_t *symbol, ast_node_var_assign_t *assignment) {
	ast_node_var_def_t *newNode = (ast_node_var_def_t *)malloc(sizeof(ast_node_var_def_t));
	if (newNode == NULL) {
		fprintf(stderr, "Error: Memory allocation for ast failed\n");
		exit(INTERNAL_COMPILER_ERROR);
	}
	newNode->type = AST_NODE_VAR_DEF;
	newNode->symbol = symbol;
	newNode->assignment = assignment;
	ast_default_node_t *defaultNode = ast_create_node(AST_NODE_DEFAULT);
	defaultNode->data_t.varDef = newNode;
	defaultNode->type = AST_NODE_VAR_DEF;
	return defaultNode;
}

ast_default_node_t *ast_createIfElseNode(ast_node_exp_t *conditionExp) {
	ast_node_if_else_t *newNode = (ast_node_if_else_t *)malloc(sizeof(ast_node_if_else_t));
	if (newNode == NULL) {
		fprintf(stderr, "Error: Memory allocation for ast failed\n");
		exit(INTERNAL_COMPILER_ERROR);
	}
	newNode->type = AST_NODE_IF_ELSE;
	newNode->conditionExp = conditionExp;
	newNode->ifBlock = malloc(sizeof(ast_default_node_t *));
	newNode->elseBlock = malloc(sizeof(ast_default_node_t *));
	newNode->ifCount = 0;
	newNode->elseCount = 0;
	newNode->elseBlockActive = false;
	ast_default_node_t *defaultNode = ast_create_node(AST_NODE_DEFAULT);
	defaultNode->data_t.ifElse = newNode;
	defaultNode->type = AST_NODE_IF_ELSE;
	return defaultNode;
}

ast_default_node_t *ast_createWhileNode(ast_node_exp_t *conditionExp) {
	ast_node_while_t *newNode = (ast_node_while_t *)malloc(sizeof(ast_node_while_t));
	if (newNode == NULL) {
		fprintf(stderr, "Error: Memory allocation for ast failed\n");
		exit(INTERNAL_COMPILER_ERROR);
	}
	newNode->type = AST_NODE_WHILE;
	newNode->conditionExp = conditionExp;
	newNode->block = malloc(sizeof(ast_default_node_t *));
	newNode->blockCount = 0;
	ast_default_node_t *defaultNode = ast_create_node(AST_NODE_DEFAULT);
	defaultNode->data_t.While = newNode;
	defaultNode->type = AST_NODE_WHILE;
	return defaultNode;
}
