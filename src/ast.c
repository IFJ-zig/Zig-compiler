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

void printIndent(int depth) {
	for (int i = 0; i < depth * 4; i++) { // Each depth level indented by 4 spaces
        fputc(' ', stderr);
    }
}
// Recursive function to print the AST
void ast_printTree(ast_node_exp_t *node, int depth) {
    if (node == NULL) {
        printIndent(depth);
        fprintf(stderr, "NULL\n");
        return;
    }

    // Print the current node's token name
    printIndent(depth);
    fprintf(stderr, "%s\n", getTokenName(*node->token));

    // Determine if the node is unary or binary
    if (node->data_t.binary_op.left != NULL || node->data_t.binary_op.right != NULL) {
        // Binary node: Print left and right subtrees
        ast_printTree(node->data_t.binary_op.left, depth + 1);
        ast_printTree(node->data_t.binary_op.right, depth + 1);
    } else if (node->data_t.unary_op.exp != NULL) {
        // Unary node: Print the single operand
        ast_printTree(node->data_t.unary_op.exp, depth + 1);
    } else {
        // Leaf node
        printIndent(depth + 1);
        fprintf(stderr, "Leaf (no children)\n");
    }
}

// Top-level function to print the AST
void ast_printExp(ast_node_exp_t *expNode) {
    if (expNode == NULL) {
        fprintf(stderr, "Expression is NULL\n");
        return;
    }
    ast_printTree(expNode, 0); // Start printing from depth 0
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
		printIndent(depth);
		fprintf(stderr, "Function definition - %s, paramCount=%d\n", astRoot->data_t.fnDef->fnSymbol->key, astRoot->data_t.fnDef->fnSymbol->paramCount);
		for (unsigned int i = 0; i < astRoot->data_t.fnDef->bodyCount; i++) {
			ast_print(astRoot->data_t.fnDef->body[i], depth + 1);
		}
	} else if (astRoot->type == AST_NODE_FN_CALL) {
		printIndent(depth);
		fprintf(stderr, "Function call - %s, argCount(Node)=%d\n", astRoot->data_t.fnCall->fnSymbol->key, astRoot->data_t.fnCall->argCount);
		for(unsigned int i = 0; i < astRoot->data_t.fnCall->argCount; i++){
			varType type = astRoot->data_t.fnCall->args[i]->dataType;
			printIndent(depth+2);
			fprintf(stderr, "->arg[%d] = %s", i, type == INT ? "INT" : type == FLOAT ? "FLOAT" : type == STRING ? "STRING" : type == ANYTYPE ? "ANYTYPE" : "VOID");
		}
		fprintf(stderr, "\n");
	} else if (astRoot->type == AST_NODE_FN_RETURN) {
		printIndent(depth);
		fprintf(stderr, "Function return\n");
		printIndent(depth+1);
		fprintf(stderr, "->expression = ");
		if(astRoot->data_t.fnReturn->expression != NULL) {
			fprintf(stderr, "%d\n", astRoot->data_t.fnReturn->expression->token->kw);
			ast_printExp(astRoot->data_t.varDef->assignment->expression);
		} else {
			fprintf(stderr, "NULL\n");
		}
	} else if (astRoot->type == AST_NODE_EXP) {
		printIndent(depth);
		fprintf(stderr, "Expression\n");
	} else if (astRoot->type == AST_NODE_VAR_DEF) {
		printIndent(depth);
		fprintf(stderr, "Variable definition - %s\n", astRoot->data_t.varDef->symbol->key);
		if(astRoot->data_t.varDef->assignment != NULL) {
			printIndent(depth+1);
			fprintf(stderr, " Variable assignment\n");
			printIndent(depth+2);
			fprintf(stderr, "->expression = ");
			if(astRoot->data_t.varDef->assignment->expression != NULL) {
				ast_printExp(astRoot->data_t.varDef->assignment->expression);
			} else {
				fprintf(stderr, "NULL\n");
			}
		} else {
			printIndent(depth+1);
			fprintf(stderr, "->assignment = NULL\n");
		}
	} else if (astRoot->type == AST_NODE_VAR_ASSIGN) {
		printIndent(depth);
		fprintf(stderr, "Variable assignment\n");
		printIndent(depth+1);
		fprintf(stderr, "->expression = ");
		if(astRoot->data_t.varAssign->expression != NULL) {
			fprintf(stderr, "%d\n", astRoot->data_t.varAssign->expression->token->kw);
		} else {
			fprintf(stderr, "NULL\n");
		}
	} else if (astRoot->type == AST_NODE_IF_ELSE) {
		printIndent(depth);
		fprintf(stderr, "If-else - ifBlockCount=%d, elseBlockCount=%d\n", astRoot->data_t.ifElse->ifCount, astRoot->data_t.ifElse->elseCount);
		printIndent(depth+1);
		fprintf(stderr, "ifBlock\n");
		if(astRoot->data_t.ifElse->ifCount != 0){
			for (unsigned int i = 0; i < astRoot->data_t.ifElse->ifCount; i++) {
				ast_print(astRoot->data_t.ifElse->ifBlock[i], depth + 2);
			}
		}
		else{
			printIndent(depth+2);
			fprintf(stderr, "->Empty\n");
		}
		printIndent(depth+1);
		fprintf(stderr, "elseBlock\n");
		if(astRoot->data_t.ifElse->elseCount != 0){
			for (unsigned int i = 0; i < astRoot->data_t.ifElse->elseCount; i++) {
				ast_print(astRoot->data_t.ifElse->elseBlock[i], depth + 2);
			}
		}
		else{
			printIndent(depth+2);
			fprintf(stderr, "->Empty\n");
		}
	} else if (astRoot->type == AST_NODE_WHILE) {
		printIndent(depth);
		fprintf(stderr, "While\n");
		printIndent(depth+1);
		fprintf(stderr, "Block\n");
		if(astRoot->data_t.While->blockCount != 0){
			for (unsigned int i = 0; i < astRoot->data_t.While->blockCount; i++) {
				ast_print(astRoot->data_t.While->block[i], depth + 1);
			}
		}
		else{
			printIndent(depth+2);
			fprintf(stderr, "->Empty\n");
		}
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
	defaultNode->data_t.exp = (struct ast_node_exp *)expNode;
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
	newNode->noNullPayload = NULL;
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
