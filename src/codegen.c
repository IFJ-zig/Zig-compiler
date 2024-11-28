#include "codegen.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

unsigned int labelCounter = 0;
//unsigned int ifCounter = 0;

void stringToSlice(char *s) {
	printf("PUSHS string@");
	for (size_t i = 0; i < strlen(s); i++) {
		if ((s[i] >= 0 && s[i] <= 32) || s[i] == 35 || s[i] == 92) {
			printf("\\%03d", s[i]);
		} else {
			printf("%c", s[i]);
		}
	}
	printf("\n");
}

void write(Token *token) {

	switch (token->kw) {
		case num:
			printf("WRITE int@%d\n", token->i);

			break;
		case text:
			printf("WRITE string@");
			for (size_t i = 0; i < strlen(token->s); i++) {
				if ((token->s[i] >= 0 && token->s[i] <= 32) || token->s[i] == 35 || token->s[i] == 92) {
					printf("\\%03d", token->s[i]);
				} else {
					printf("%c", token->s[i]);
				}
			}
			printf("\n");

			break;
		case decim:
			printf("WRITE float@%a\n", token->f);
			break;
		case _null:
			printf("WRITE nil@nil\n");
			break;
		case id:
			printf("WRITE LF@%s\n", token->s);
			break;

		default:
			break;
	}
}

void functionDef(ast_node_fn_def_t *fn) {
	printf("LABEL %s\n", fn->fnSymbol->key);
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");
	for (int i = 0; i < fn->fnSymbol->paramCount; i++) {
		printf("DEFVAR LF@%s\n", fn->fnSymbol->params[i]->key);
		printf("POPS LF@%s\n", fn->fnSymbol->params[i]->key);
	}
	codebody(fn->body, fn->bodyCount);
	printf("POPFRAME\n");
	if (strcmp(fn->fnSymbol->key, "main") == 0) {
		printf("EXIT int@0\n");
	} else {
		printf("RETURN\n");
	}
}

void removeChar(char *str, char garbage) {

	char *src, *dst;
	for (src = dst = str; *src != '\0'; src++) {
		*dst = *src;
		if (*dst != garbage)
			dst++;
	}
	*dst = '\0';
}

void functionCall(ast_node_fn_call_t *fn) {
	for (unsigned int i = 0; i < fn->argCount; i++) {
		if (fn->args[i]->token->kw == id) {
			printf("PUSHS LF@%s\n", fn->args[i]->token->s);
		} else if (fn->args[i]->token->kw == num) {
			printf("PUSHS int@%d\n", fn->args[i]->token->i);
		} else if (fn->args[i]->token->kw == decim) {
			printf("PUSHS float@%a\n", fn->args[i]->token->f);
		} else if (fn->args[i]->token->kw == text) {
			printf("PUSHS string@");
			for (size_t j = 0; j < strlen(fn->args[i]->token->s); j++) {
				if ((fn->args[i]->token->s[j] >= 0 && fn->args[i]->token->s[j] <= 32) || fn->args[i]->token->s[j] == 35 || fn->args[i]->token->s[j] == 92) {
					printf("\\%03d", fn->args[i]->token->s[j]);
				} else {
					printf("%c", fn->args[i]->token->s[j]);
				}
			}
			printf("\n");
		}
	}
	if (strncmp(fn->fnSymbol->key, "ifj.", 4) == 0) {
		if (strcmp(fn->fnSymbol->key, "ifj.write") == 0) {
			write(fn->args[0]->token);
		} else if (strcmp(fn->fnSymbol->key, "ifj.string") == 0) {
			stringToSlice(fn->args[0]->token->s);
		} else {
			char *ifj = (char *)fn->fnSymbol->key;
			removeChar(ifj, '.');
			printf("CALL %s\n", ifj);
		}
	} else {
		printf("CALL %s\n", fn->fnSymbol->key);
	}
}

void functionReturn(ast_node_fn_return_t *fn) {
	if (fn->expression->token->kw == id) {
		printf("MOVE LF@retval%% LF@%s\n", fn->expression->token->s);
	} else if (fn->expression->token->kw == dtint) {
		printf("MOVE LF@retval%% int@%d\n", fn->expression->token->i);
	} else if (fn->expression->token->kw == dtflt) {
		printf("MOVE LF@retval%% float@%a\n", fn->expression->token->f);
	} else if (fn->expression->token->kw == dtstr) {
		printf("MOVE LF@retval%% string@%s\n", fn->expression->token->s);
	}
}

void variableDefinition(ast_node_var_def_t *var) {
	printf("DEFVAR LF@%s\n", var->symbol->key);
	if (var->assignment != NULL) {
		variableAssignment(var->assignment);
	}
}

void variableAssignment(ast_node_var_assign_t *var) {
	expression(var->expression);
	printf("POPS LF@%s\n", var->symbol->key);
}
//TODO: make it realloc
bst_items_t *bst_init_items() {
	bst_items_t *items = (bst_items_t *)malloc(sizeof(bst_items_t));
	items->capacity = 32;
	items->nodes = (ast_node_exp_t **)malloc(sizeof(ast_node_exp_t *) * items->capacity);
	items->size = 0;
	return items;
}

void bst_add_node_to_items(ast_node_exp_t *node, bst_items_t *items) {
	items->nodes[items->size] = node;
	items->size++;
}

void bst_postorder(ast_node_exp_t *tree, bst_items_t *items) {

	// Ukončení rekurze
	if (tree == NULL) {

		return;
	}

	// Dle pravidel postorderu
	bst_postorder(tree->data_t.binary_op.left, items);
	bst_postorder(tree->data_t.binary_op.right, items);
	bst_add_node_to_items(tree, items);
}


void expression(ast_node_exp_t *exp) {
	bst_items_t *items = bst_init_items();
	bst_postorder(exp, items);
	bool foundi32 = false;
	bool foundf64 = false;
	for (int i = 0; i < items->size; i++) {
		if (items->nodes[i]->token->kw == num) {
			foundi32 = true;
		} else if (items->nodes[i]->token->kw == decim) {
			foundf64 = true;
		} else if (items->nodes[i]->token->kw == id) {
			if (items->nodes[i]->dataType == INT) {
				foundi32 = true;
			} else if (items->nodes[i]->dataType == FLOAT) {
				foundf64 = true;
			} else if (items->nodes[i]->dataType == FUNCTION) {
				if (items->nodes[i]->data_t.fnCall->fnSymbol->returnType == INT) {
					foundi32 = true;
				} else if (items->nodes[i]->data_t.fnCall->fnSymbol->returnType == FLOAT) {
					foundf64 = true;
				}
			}
		}
	}
	if (!foundi32) {
		(void)foundi32;
	}

	for (int i = 0; i < items->size; i++) {
		switch (items->nodes[i]->token->kw) {
			case num:
				printf("PUSHS int@%d\n", items->nodes[i]->token->i);
				if (foundf64) {
					printf("INT2FLOATS\n");
				}
				break;

			case decim:
				printf("PUSHS float@%a\n", items->nodes[i]->token->f);
				break;
			case id:
				if (items->nodes[i]->dataType == FUNCTION) {
					functionCall(items->nodes[i]->data_t.fnCall);
					printf("PUSHS TF@retval%%\n");
				} else {

					printf("PUSHS LF@%s\n", items->nodes[i]->token->s);
					if (items->nodes[i]->dataType == INT && foundf64) {
						printf("INT2FLOATS\n");
					}
				}
				break;
			case _null:
				printf("PUSHS nil@nil\n");
				break;
			case plus:
				printf("ADDS\n");
				break;
			case minus:
				printf("SUBS\n");
				break;
			case multiply:
				printf("MULS\n");
				break;
			case division:
				if (!foundf64) {
					printf("IDIVS\n");
				} else {
					printf("DIVS\n");
				}
				break;

			default:
				break;
		}
	}
}

void whileLoop(ast_node_while_t *loop) {


	unsigned int internalLabelCounter = labelCounter;
	labelCounter++;

	printf("LABEL WHILE%d\n", internalLabelCounter);
	expression(loop->conditionExp->data_t.binary_op.left);
	expression(loop->conditionExp->data_t.binary_op.right);

	switch (loop->conditionExp->token->kw) {
		case compare_equal:
			printf("EQS\n");
			printf("PUSHS bool@true\n");
			printf("JUMPIFNEQS WHILEEND%d\n", internalLabelCounter);
			break;
		case nequal:
			printf("EQS\n");
			printf("PUSHS bool@true\n");
			printf("JUMPIFEQS WHILEEND%d\n", internalLabelCounter);
			break;
		case less:
			printf("LTS\n");
			printf("PUSHS bool@true\n");
			printf("JUMPIFNEQS WHILEEND%d\n", internalLabelCounter);
			break;
		case more:
			printf("GTS\n");
			printf("PUSHS bool@true\n");
			printf("JUMPIFNEQS WHILEEND%d\n", internalLabelCounter);
			break;
		case lequal:
			printf("GTS\n");
			printf("PUSHS bool@true\n");
			printf("JUMPIFEQS WHILEEND%d\n", internalLabelCounter);
			break;
		case mequal:
			printf("LTS\n");
			printf("PUSHS bool@true\n");
			printf("JUMPIFEQS WHILEEND%d\n", internalLabelCounter);
			break;
		default:
			break;
	}
	codebody(loop->block, loop->blockCount);
	printf("JUMP WHILE%d\n", internalLabelCounter);
	printf("LABEL WHILEEND%d\n", internalLabelCounter);
}

void ifElse(ast_node_if_else_t *ifelse) {
	if (ifelse->noNullPayload == NULL) {
		unsigned int internalLabelCounter = labelCounter;
		labelCounter++;
		printf("LABEL IF%d\n", internalLabelCounter);
		expression(ifelse->conditionExp->data_t.binary_op.left);
		expression(ifelse->conditionExp->data_t.binary_op.right);
		switch (ifelse->conditionExp->token->kw) {
			case compare_equal:
				printf("EQS\n");
				printf("PUSHS bool@true\n");
				printf("JUMPIFNEQS ELSE%d\n", internalLabelCounter);
				break;
			case nequal:
				printf("EQS\n");
				printf("PUSHS bool@true\n");
				printf("JUMPIFEQS ELSE%d\n", internalLabelCounter);
				break;
			case less:
				printf("LTS\n");
				printf("PUSHS bool@true\n");
				printf("JUMPIFNEQS ELSE%d\n", internalLabelCounter);
				break;
			case more:
				printf("GTS\n");
				printf("PUSHS bool@true\n");
				printf("JUMPIFNEQS ELSE%d\n", internalLabelCounter);
				break;
			case lequal:
				printf("GTS\n");
				printf("PUSHS bool@true\n");
				printf("JUMPIFEQS ELSE%d\n", internalLabelCounter);
				break;
			case mequal:
				printf("LTS\n");
				printf("PUSHS bool@true\n");
				printf("JUMPIFEQS ELSE%d\n", internalLabelCounter);
				break;
			default:
				break;
		}
		codebody(ifelse->ifBlock, ifelse->ifCount);
		printf("JUMP IFEND%d\n", internalLabelCounter);
		printf("LABEL ELSE%d\n", internalLabelCounter);
		codebody(ifelse->elseBlock, ifelse->elseCount);
		printf("LABEL IFEND%d\n", internalLabelCounter);
	} else {
		unsigned int internalLabelCounter = labelCounter;
		labelCounter++;
		printf("LABEL IF%d\n", internalLabelCounter);
		expression(ifelse->conditionExp);
		printf("PUSHS nil@nil\n");
		printf("EQS\n");
		printf("JUMPIFEQS ELSE%d\n", internalLabelCounter);
		printf("DEFVAR LF@%s\n", ifelse->noNullPayload->key);
		printf("MOVE LF@%s LF@%s\n", ifelse->noNullPayload->key, ifelse->conditionExp->token->s);
		codebody(ifelse->ifBlock, ifelse->ifCount);
		printf("JUMP IFEND%d\n", internalLabelCounter);
		printf("LABEL ELSE%d\n", internalLabelCounter);
		codebody(ifelse->elseBlock, ifelse->elseCount);
		printf("LABEL IFEND%d\n", internalLabelCounter);
	}
}
void codebody(ast_default_node_t **nodes, unsigned int nodeCount) {
    
	for(unsigned int i = 0; i < nodeCount; i++) {
        //printf("halal\n");
		switch ((*nodes)->type) {
			case AST_NODE_FN_DEF:
				functionDef((*nodes)->data_t.fnDef);
				break;
			case AST_NODE_FN_RETURN:
				functionReturn((*nodes)->data_t.fnReturn);
				break;
			case AST_NODE_FN_CALL:
                //printf("halalE\n");
				functionCall((*nodes)->data_t.fnCall);
				break;
			case AST_NODE_EXP:
				expression((*nodes)->data_t.exp);
				break;
			case AST_NODE_VAR_DEF:
				variableDefinition((*nodes)->data_t.varDef);
				break;
			case AST_NODE_VAR_ASSIGN:
				variableAssignment((*nodes)->data_t.varAssign);
				break;
			case AST_NODE_IF_ELSE:
				ifElse((*nodes)->data_t.ifElse);
				break;
			case AST_NODE_WHILE:
				whileLoop((*nodes)->data_t.While);
				break;
			case AST_NODE_DEFAULT:
				//codebody((*nodes)->data_t.body_t.nodes);
				break;
		}
		nodes++;
	}
}

void printHeader() {
	printf(".IFJcode24\n");
	printf("JUMP main\n");
	printf("\
        \
        LABEL ifjreadi32\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@readValue%%\n\
            DEFVAR LF@readValueType%%\n\
            READ LF@readValue%% int\n\
            TYPE LF@readValueType%% LF@readValue%%\n\
            JUMPIFNEQ correct_input_int LF@readValueType%% nil@nil\n\
            MOVE LF@retVal%% nil@nil\n\
            POPFRAME\n\
            RETURN\n\
            LABEL correct_input_int\n\
            MOVE LF@retVal%% LF@readValue%%\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjreadf64\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@readValue%%\n\
            DEFVAR LF@readValueType%%\n\
            READ LF@readValue%% float\n\
            TYPE LF@readValueType%% LF@readValue%%\n\
            JUMPIFNEQ correct_input_float LF@readValueType%% nil@nil\n\
            MOVE LF@retVal%% nil@nil\n\
            POPFRAME\n\
            RETURN\n\
            LABEL correct_input_float\n\
            MOVE LF@retVal%% LF@readValue%%\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjreadstr\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@readValue%%\n\
            DEFVAR LF@readValueType%%\n\
            READ LF@readValue%% string\n\
            TYPE LF@readValueType%% LF@readValue%%\n\
            JUMPIFNEQ correct_input_string LF@readValueType%% nil@nil\n\
            MOVE LF@retVal%% nil@nil\n\
            POPFRAME\n\
            RETURN\n\
            LABEL correct_input_string\n\
            MOVE LF@retVal%% LF@readValue%%\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifji2f\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@param%%\n\
            POPS LF@param%%\n\
            INT2FLOAT LF@retVal%% LF@param%%\n\
            \
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjf2i\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@param%%\n\
            POPS LF@param%%\n\
            FLOAT2INT LF@retVal%% LF@param%%\n\
            \
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjlength\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@param%%\n\
            POPS LF@param%%\n\
            STRLEN LF@retVal%% LF@param%%\n\
            \
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjconcat\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@param1%%\n\
            DEFVAR LF@param2%%\n\
            POPS LF@param2%%\n\
            POPS LF@param1%%\n\
            CONCAT LF@retVal%% LF@param1%% LF@param2%%\n\
            \
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjsubstring\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            MOVE LF@retVal%% string@\n\
            DEFVAR LF@string%%\n\
            DEFVAR LF@startIndex%%\n\
            DEFVAR LF@endIndex%%\n\
            DEFVAR LF@strLength%%\n\
            POPS LF@endIndex%%\n\
            POPS LF@startIndex%%\n\
            POPS LF@string%%\n\
            STRLEN LF@strLength%% LF@string%%\n\
            DEFVAR LF@indexCheck%%\n\
            LT LF@indexCheck%% LF@startIndex%% int@0\n\
            JUMPIFEQ substrCheckFailed LF@indexCheck%% bool@true\n\
            LT LF@indexCheck%% LF@endIndex%% int@0\n\
            JUMPIFEQ substrCheckFailed LF@indexCheck%% bool@true\n\
            GT LF@indexCheck%% LF@startIndex%% LF@endIndex%%\n\
            JUMPIFEQ substrCheckFailed LF@indexCheck%% bool@true\n\
            LT LF@indexCheck%% LF@startIndex%% LF@strLength%%\n\
            JUMPIFNEQ substrCheckFailed LF@indexCheck%% bool@true\n\
            GT LF@indexCheck%% LF@endIndex%% LF@strLength%%\n\
            JUMPIFEQ substrCheckFailed LF@indexCheck%% bool@true\n\
            DEFVAR LF@substrChar%%\n\
            DEFVAR LF@substrCounter%%\n\
            SUB LF@substrCounter%% LF@endIndex%% LF@startIndex%%\n\
            LABEL substrWhileCycle\n\
            JUMPIFEQ substrCheckPassed LF@substrCounter%% int@0\n\
            GETCHAR LF@substrChar%% LF@string%% LF@startIndex%%\n\
            CONCAT LF@retVal%% LF@retVal%% LF@substrChar%%\n\
            ADD LF@startIndex%% LF@startIndex%% int@1\n\
            SUB LF@substrCounter%% LF@substrCounter%% int@1\n\
            JUMP substrWhileCycle\n\
            LABEL substrCheckFailed\n\
            MOVE LF@retVal%% nil@nil\n\
            LABEL substrCheckPassed\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjstrcmp\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@str1Length%%\n\
            DEFVAR LF@str2Length%%\n\
            DEFVAR LF@usedLength%%\n\
            DEFVAR LF@string1%%\n\
            DEFVAR LF@string2%%\n\
            DEFVAR LF@strcmpInfo%% ## 0 - equal, 1 - str1 > str2, 2 - str1 < str2\n\
            \
            POPS LF@string2%%\n\
            POPS LF@string1%%\n\
            STRLEN LF@str1Length%% LF@string1%%\n\
            STRLEN LF@str2Length%% LF@string2%%\n\
            DEFVAR LF@strcmpBool%%\n\
            EQ LF@strcmpBool%% LF@str1Length%% LF@str2Length%%\n\
            JUMPIFEQ strcmpLengthsEqual LF@strcmpBool%% bool@true\n\
            LT LF@strcmpBool%% LF@str1Length%% LF@str2Length%%\n\
            JUMPIFEQ strcmpLength1Smaller LF@strcmpBool%% bool@true\n\
            \
            LABEL strcmpLength1Greater\n\
            MOVE LF@usedLength%% LF@str2Length%%\n\
            MOVE LF@strcmpInfo%% int@1\n\
            JUMP strcmpCycle\n\
            LABEL strcmpLength1Smaller\n\
            MOVE LF@usedLength%% LF@str1Length%%\n\
            MOVE LF@strcmpInfo%% int@-1\n\
            JUMP strcmpCycle\n\
            LABEL strcmpLengthsEqual\n\
            MOVE LF@usedLength%% LF@str1Length%%\n\
            MOVE LF@strcmpInfo%% int@0\n\
            \
            LABEL strcmpCycle\n\
            DEFVAR LF@strcmpChar1%%\n\
            DEFVAR LF@strcmpChar2%%\n\
            DEFVAR LF@strcmpIndex%%\n\
            MOVE LF@strcmpIndex%% int@0\n\
            \
            JUMPIFEQ strcmpEnd LF@usedLength%% int@0\n\
            STRI2INT LF@strcmpChar1%% LF@string1%% LF@strcmpIndex%%\n\
            STRI2INT LF@strcmpChar2%% LF@string2%% LF@strcmpIndex%%\n\
            DPRINT LF@strcmpIndex%%\n\
            EQ LF@strcmpBool%% LF@strcmpChar1%% LF@strcmpChar2%%\n\
            JUMPIFEQ strcmpCycleDiff LF@strcmpBool%% bool@false\n\
            ADD LF@strcmpIndex%% LF@strcmpIndex%% int@1\n\
            SUB LF@usedLength%% LF@usedLength%% int@1\n\
            JUMP strcmpCycle\n\
            \
            LABEL strcmpCycleDiff\n\
            LT LF@strcmpBool%% LF@strcmpChar1%% LF@strcmpChar2%%\n\
            JUMPIFEQ strcmpString1Less LF@strcmpBool%% bool@true\n\
            MOVE LF@retVal%% int@1\n\
            POPFRAME\n\
            RETURN\n\
            \
            LABEL strcmpString1Less\n\
            MOVE LF@retVal%% int@-1\n\
            POPFRAME\n\
            RETURN\n\
            \
            LABEL strcmpEnd\n\
            MOVE LF@retVal%% LF@strcmpInfo%%\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjord\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@string%%\n\
            DEFVAR LF@index%%\n\
            DEFVAR LF@strLen%%\n\
            \
            POPS LF@index%%\n\
            POPS LF@string%%\n\
            STRLEN LF@strLen%% LF@string%%\n\
            \
            DEFVAR LF@indexCheck%%\n\
            EQ LF@indexCheck%% LF@strLen%% int@0\n\
            JUMPIFEQ ordCheckFailed LF@indexCheck%% bool@true\n\
            LT LF@indexCheck%% LF@index%% LF@strLen%%\n\
            JUMPIFEQ ordCheckFailed LF@indexCheck%% bool@false\n\
            LT LF@indexCheck%% LF@index%% int@0\n\
            JUMPIFEQ ordCheckFailed LF@indexCheck%% bool@true\n\
            \
            STRI2INT LF@retVal%% LF@string%% LF@index%%\n\
            POPFRAME\n\
            RETURN\n\
            \
            LABEL ordCheckFailed\n\
            MOVE LF@retVal%% int@0\n\
            POPFRAME\n\
        RETURN\n\
        \
        LABEL ifjchr\n\
            CREATEFRAME\n\
            PUSHFRAME\n\
            \
            DEFVAR LF@retVal%%\n\
            DEFVAR LF@char%%\n\
            POPS LF@char%%\n\
            INT2CHAR LF@retVal%% LF@char%%\n\
            \
            POPFRAME\n\
        RETURN\n\
    ");
}