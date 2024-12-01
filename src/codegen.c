/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains functions used to generate IFJcode24
 *  @file codegen.c
 *  @author Daniel Bohata, xbohatd00
 *  @brief Code generation for IFJcode24
 * 
*/

#include "codegen.h"

// Global variable used to differentiate between different while cycles and ifElses
unsigned int labelCounter = 0;

// Global variable used for pointing to the current function's array of already defined variables
array_vars_t *vars = NULL;

/**
 * 	Defines all variables inside a body of code, going into depth
 *	@brief Defines all variables in a body of code
 *	@param nodes Body of code to analyse
 *	@param nodeCount Number of nodes to analyse
*/
void defAllVars(ast_default_node_t **nodes, unsigned int nodeCount) {

	// Checks for all nodes that could have a variable definition in them and defines those variables, adds them to the array
	for (unsigned int i = 0; i < nodeCount; i++) {

		if (nodes[i]->type == AST_NODE_VAR_DEF) {

			ast_node_var_def_t *var = nodes[i]->data_t.varDef;
			bool foundVar = false;
			for (int x = 0; x < vars->size; x++) {

				if (strcmp(vars->nodes[x], var->symbol->key) == 0) {
					foundVar = true;
				}
			}

			if (!foundVar) {

				arrayAddVarToItems((char *)var->symbol->key, vars);
				printf("DEFVAR LF@%s\n", var->symbol->key);
			}
		} else if (nodes[i]->type == AST_NODE_IF_ELSE) {

			if(nodes[i]->data_t.ifElse->noNullPayload != NULL) {

				bool foundVar = false;
				for (int x = 0; x < vars->size; x++) {

					if (strcmp(vars->nodes[x], nodes[i]->data_t.ifElse->noNullPayload->key) == 0) {
						foundVar = true;
					}
				}

				if (!foundVar) {

					arrayAddVarToItems((char *)nodes[i]->data_t.ifElse->noNullPayload->key, vars);
					printf("DEFVAR LF@%s\n", nodes[i]->data_t.ifElse->noNullPayload->key);
				}
			}

			defAllVars(nodes[i]->data_t.ifElse->ifBlock, nodes[i]->data_t.ifElse->ifCount);
			defAllVars(nodes[i]->data_t.ifElse->elseBlock, nodes[i]->data_t.ifElse->elseCount);
		} else if (nodes[i]->type == AST_NODE_WHILE) {

			if(nodes[i]->data_t.While->noNullPayload != NULL) {

				bool foundVar = false;
				for (int x = 0; x < vars->size; x++) {

					if (strcmp(vars->nodes[x], nodes[i]->data_t.While->noNullPayload->key) == 0) {
						foundVar = true;
					}
				}

				if (!foundVar) {

					arrayAddVarToItems((char *)nodes[i]->data_t.While->noNullPayload->key, vars);
					printf("DEFVAR LF@%s\n", nodes[i]->data_t.While->noNullPayload->key);
				}
			}

			defAllVars(nodes[i]->data_t.While->block, nodes[i]->data_t.While->blockCount);
		}
	}
}

/**
 * 	Function used to create []u8 format from an input string
 * 	@brief Creates []u8 format from a string
 * 	@param s Input string 
 */
void printString(char *s){
	for (size_t i = 0; i < strlen(s); i++) {

		// Escape sequence in case the ASCII value is 0-32, 35 or 92 - as written in the assignment
		if ((s[i] >= 0 && s[i] <= 32) || s[i] == 35 || s[i] == 92) {

			printf("\\%03d", s[i]);
		} else {

			printf("%c", s[i]);
		}
	}
	printf("\n");
}

/**
 * 	Inbuilt ifj.string function
 * 	@brief Inbuilt ifj.string function
 * 	@param s Input string
 */
void stringToSlice(char *s) {

	printf("PUSHS string@");
	printString(s);
}

/**
 * 	Function used to write input token based on the token type
 * 	@brief Inbuilt write function
 * 	@param token Token to be written
 */
void write(Token *token) {

	switch (token->kw) {
		case num:

			printf("WRITE int@%d\n", token->i);
			break;
		case text:

			printf("WRITE string@");
			printString(token->s);
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

/**
 * 	Function that defines functions in IFJcode24
 * 	@brief Creates functions in IFJcode24
 * 	@param fn Function definition node that contains the function's information
 */
void functionDef(ast_node_fn_def_t *fn) {

	vars = arrayInitVars();

	printf("LABEL %s\n", fn->fnSymbol->key);
	printf("CREATEFRAME\n");
	printf("PUSHFRAME\n");

	// Definition of the function's parameters
	for (int i = fn->fnSymbol->paramCount-1; i >= 0; i--) {

		printf("DEFVAR LF@%s\n", fn->fnSymbol->params[i]->key);
		arrayAddVarToItems((char *)fn->fnSymbol->params[i]->key, vars);
		printf("POPS LF@%s\n", fn->fnSymbol->params[i]->key);
	}

	// Define all variables used inside the function
	defAllVars(fn->body, fn->bodyCount);

	// Analyse the body of the function
	codebody(fn->body, fn->bodyCount);

	printf("POPFRAME\n");

	// Main needs to finish with EXIT, not return
	if (strcmp(fn->fnSymbol->key, "main") == 0) {

		printf("EXIT int@0\n");
	} else {

		printf("RETURN\n");
	}

	free(vars->nodes);
	free(vars);
}

/**
 * 	Function that removes a selected character from a string
 * 	@brief Removes a character from a string
 * 	@param str String from which the character will be removed
 * 	@param ch Character to remove
 */
void removeChar(char *str, char ch) {

	int i = 0;
	int j = 0;

	while((size_t)i < strlen(str)){
		if (str[i] != ch) {
            str[j] = str[i];
			j++;
        }
		i++;
	}
	str[j] = '\0';
}

/**
 * 	Function that creates a function call in IFJcode24
 * 	@brief Creates a function call in IFJcode24
 * 	@param fn Node that contains information about the function we want to call
 */
void functionCall(ast_node_fn_call_t *fn) {

	// Write doesn't need arguments
	if (strcmp(fn->fnSymbol->key, "ifj.write") != 0){
		for (unsigned int i = 0; i < fn->argCount; i++) {

			expression(fn->args[i]);
		}
	}

	// Builtin functions are a special case
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

/**
 * 	Function that creates a return with value in retval% if the function has it
 * 	@brief Creates a function's return with value
 * 	@param fn Node that contains information about the return
 */
void functionReturn(ast_node_fn_return_t *fn) {

	printf("DEFVAR LF@retval%%\n");
	expression(fn->expression);
	printf("POPS LF@retval%%\n");
	printf("POPFRAME\n");
	printf("RETURN\n");
}

/**
 * 	Function that returns an initialized var array
 * 	@brief Creates a var array
 * 	@return Created var array
 */
array_vars_t *arrayInitVars(){

	array_vars_t *items = (array_vars_t *)malloc(sizeof(array_vars_t));
	items->capacity = 32;
	items->nodes = (char **)malloc(sizeof(char *) * items->capacity);
	items->size = 0;
	return items;
};

/**
 * 	Function that adds an item to a var array
 * 	@brief Adds an item to a var array
 * 	@param var Variable name to add to var array
 * 	@param items Var array to be added to
 */
void arrayAddVarToItems(char *var, array_vars_t *items) {

	if(items->size >= items->capacity-1) {

		items->capacity *= 2;
		items->nodes = (char **)realloc(items->nodes, sizeof(char *) * items->capacity);
	}
	items->nodes[items->size] = var;
	items->size++;
}

/**
 * 	Function that creates a definition of a variable in IFJcode24
 * 	@brief Creates a variable definition in IFJcode24
 * 	@param var Node that contains information about the variable we want do define
 */
void variableDefinition(ast_node_var_def_t *var) {

	bool foundVar = false;
	for (int i = 0; i < vars->size; i++) {
		if (strcmp(vars->nodes[i], var->symbol->key) == 0) {

			foundVar = true;
		}
	}

	if (!foundVar) {

		arrayAddVarToItems((char *)var->symbol->key, vars);
		printf("DEFVAR LF@%s\n", var->symbol->key);
	}

	// Variable definition can include assignment of a value
	if (var->assignment != NULL) {

		variableAssignment(var->assignment);
	}
}

/**
 * 	Function that assigns a value to a variable in IFJcode24
 * 	@brief Assigns a value to a variable in IFJcode24
 * 	@param var Node that contains information about the value that we want to assign
 */
void variableAssignment(ast_node_var_assign_t *var) {

	expression(var->expression);

	// Assignations into '_' get discarded by getting popped into GF@_%
	if((strcmp(var->symbol->key, "_" ) == 0)) {

		printf("POPS GF@_%%\n");
	} else {

		printf("POPS LF@%s\n", var->symbol->key);
	}
}

/**
 * 	Function that creates and initializes an array of exp nodes
 * 	@brief Creates and initializes an array of exp nodes
 * 	@return Created exp node array
 */
array_items_t *arrayInitItems() {

	array_items_t *items = (array_items_t *)malloc(sizeof(array_items_t));
	items->capacity = 32;
	items->nodes = (ast_node_exp_t **)malloc(sizeof(ast_node_exp_t *) * items->capacity);
	items->size = 0;
	return items;
}

/**
 * 	Function that adds an exp node to an array of exp nodes
 * 	@brief Adds an exp node to an array of exp nodes
 * 	@param node Exp node to be added to an array
 * 	@param items Exp node array to be added to
 */
void arrayAddNodeToItems(ast_node_exp_t *node, array_items_t *items) {

	if(items->size >= items->capacity-1) {

		items->capacity *= 2;
		items->nodes = (ast_node_exp_t **)realloc(items->nodes, sizeof(ast_node_exp_t *) * items->capacity);
	}
	items->nodes[items->size] = node;
	items->size++;
}

/**
 * 	Function that sorts a binary exp node tree into a post-order sorted list
 * 	Algorhithm from IAL classes
 * 	@brief Creates a post-ordered list from exp node binary tree
 * 	@param tree Exp node from which the sorting begins
 * 	@param items Exp node array to which is the sorted tree put into
 */
void arrayPostorder(ast_node_exp_t *tree, array_items_t *items) {

	if (tree == NULL) {

		return;
	}

	arrayPostorder(tree->data_t.binary_op.left, items);
	arrayPostorder(tree->data_t.binary_op.right, items);
	arrayAddNodeToItems(tree, items);
}

/**
 * 	Function that evaluates the outcome of an expression tree
 * 	@brief Evaluates the outcome of an expression tree
 * 	@param exp Root node of binary exp tree to be analysed
 */
void expression(ast_node_exp_t *exp) {

	// Create postorder
	array_items_t *items = arrayInitItems();
	arrayPostorder(exp, items);
	
	/* Check if there is a float anywhere, if there is we convert all ints to floats
	 * Also used to determine whether we use IDIV or DIV
	*/
	bool foundf64 = false;
	for (int i = 0; i < items->size; i++) {
		if (items->nodes[i]->token->kw == decim) {

			foundf64 = true;
		} else if (items->nodes[i]->token->kw == id) {
			if (items->nodes[i]->dataType == FLOAT) {

				foundf64 = true;
			} else if (items->nodes[i]->dataType == FUNCTION) {

				if (items->nodes[i]->data_t.fnCall->fnSymbol->returnType == FLOAT) {

					foundf64 = true;
				}
			}
		}
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
					if(strcmp(items->nodes[i]->data_t.fnCall->fnSymbol->key, "ifj.string") != 0) {

						printf("PUSHS TF@retval%%\n");
					}

					if(items->nodes[i]->data_t.fnCall->fnSymbol->returnType == INT && foundf64){

						printf("INT2FLOATS");
					}
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

				// Special case for when we encounter negative numbers in assignment
				if(items->nodes[i]->data_t.unary_op.exp != NULL) {

					printf("PUSHS int@0\n");
					expression(items->nodes[i]->data_t.unary_op.exp);
					printf("SUBS\n");
				} else {

					printf("SUBS\n");
				}
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
			case compare_equal:

				printf("EQS\n");
				printf("PUSHS bool@true\n");
				break;
			case nequal:

				printf("EQS\n");
				printf("PUSHS bool@true\n");
				break;
			case less:

				printf("LTS\n");
				printf("PUSHS bool@true\n");
				break;
			case more:

				printf("GTS\n");
				printf("PUSHS bool@true\n");
				break;
			case lequal:

				printf("GTS\n");
				printf("PUSHS bool@true\n");
				break;
			case mequal:

				printf("LTS\n");
				printf("PUSHS bool@true\n");
				break;
			default:
				break;
		}
	}
	free(items->nodes);
	free(items);
}

/**
 * 	Function that creates a while loop in IFJcode24
 * 	@brief Creates a while loop in IFJcode24
 * 	@param loop Node that includes information about the while loop we want to create
 */
void whileLoop(ast_node_while_t *loop) {
	
	// Counter for labeling while loops in case there are nested while loops or if-elses inside
	unsigned int internalLabelCounter = labelCounter;
	labelCounter++;

	// Two different types of while loops need to be differentiated
	if(loop->noNullPayload == NULL){

		printf("LABEL WHILE%d\n", internalLabelCounter);

		// Condition checking
		expression(loop->conditionExp);
		switch (loop->conditionExp->token->kw) {
			case compare_equal:

				printf("JUMPIFNEQS WHILEEND%d\n", internalLabelCounter);
				break;
			case nequal:

				printf("JUMPIFEQS WHILEEND%d\n", internalLabelCounter);
				break;
			case less:

				printf("JUMPIFNEQS WHILEEND%d\n", internalLabelCounter);
				break;
			case more:

				printf("JUMPIFNEQS WHILEEND%d\n", internalLabelCounter);
				break;
			case lequal:

				printf("JUMPIFEQS WHILEEND%d\n", internalLabelCounter);
				break;
			case mequal:

				printf("JUMPIFEQS WHILEEND%d\n", internalLabelCounter);
				break;
			default:
				break;
		}

		// While cycle body of code
		codebody(loop->block, loop->blockCount);

		printf("JUMP WHILE%d\n", internalLabelCounter);
		printf("LABEL WHILEEND%d\n", internalLabelCounter);
	} else {

		printf("LABEL WHILE%d\n", internalLabelCounter);

		// Null condition check 
		expression(loop->conditionExp);
		printf("PUSHS nil@nil\n");
		printf("EQS\n");
		printf("PUSHS bool@true\n");
		printf("JUMPIFEQS WHILEEND%d\n", internalLabelCounter);

		// While cycle body of code
		printf("MOVE LF@%s LF@%s\n", loop->noNullPayload->key, loop->conditionExp->token->s);
		codebody(loop->block, loop->blockCount);

		printf("JUMP WHILE%d\n", internalLabelCounter);
		printf("LABEL WHILEEND%d\n", internalLabelCounter);
	}
}

/**
 * 	Function that creates an if-else statement in IFJcode24
 * 	@brief Creates an if-else statement in IFJcode24
 * 	@param ifelse Node that contains information about the if-else statement we want to create
 */
void ifElse(ast_node_if_else_t *ifelse) {

	// Counter for labeling if-elses in case there are nested while loops or if-elses inside
	unsigned int internalLabelCounter = labelCounter;
	labelCounter++;
	
	// We have to differentiate between two types of if-elses
	if (ifelse->noNullPayload == NULL) {

		printf("LABEL IF%d\n", internalLabelCounter);

		// Condition checking
		expression(ifelse->conditionExp);
		switch (ifelse->conditionExp->token->kw) {
			case compare_equal:

				printf("JUMPIFNEQS ELSE%d\n", internalLabelCounter);
				break;
			case nequal:

				printf("JUMPIFEQS ELSE%d\n", internalLabelCounter);
				break;
			case less:

				printf("JUMPIFNEQS ELSE%d\n", internalLabelCounter);
				break;
			case more:

				printf("JUMPIFNEQS ELSE%d\n", internalLabelCounter);
				break;
			case lequal:

				printf("JUMPIFEQS ELSE%d\n", internalLabelCounter);
				break;
			case mequal:

				printf("JUMPIFEQS ELSE%d\n", internalLabelCounter);
				break;
			default:
				break;
		}

		// Body of code of the if part
		codebody(ifelse->ifBlock, ifelse->ifCount);

		printf("JUMP IFEND%d\n", internalLabelCounter);
		printf("LABEL ELSE%d\n", internalLabelCounter);

		// Body of code of the else part
		codebody(ifelse->elseBlock, ifelse->elseCount);

		printf("LABEL IFEND%d\n", internalLabelCounter);
	} else {

		printf("LABEL IF%d\n", internalLabelCounter);

		// Null condition check
		expression(ifelse->conditionExp);
		printf("PUSHS nil@nil\n");
		printf("EQS\n");
		printf("PUSHS bool@true\n");
		printf("JUMPIFEQS ELSE%d\n", internalLabelCounter);

		// Body of code of the if part
		printf("MOVE LF@%s LF@%s\n", ifelse->noNullPayload->key, ifelse->conditionExp->token->s);
		codebody(ifelse->ifBlock, ifelse->ifCount);

		printf("JUMP IFEND%d\n", internalLabelCounter);
		printf("LABEL ELSE%d\n", internalLabelCounter);

		// Body of code of the else part
		codebody(ifelse->elseBlock, ifelse->elseCount);
		printf("LABEL IFEND%d\n", internalLabelCounter);
	}
}

/**
 * 	Function that calls the correct function based on current node in a body of code
 * 	@brief Calls the correct function based on current node in a body of code
 * 	@param nodes Body of code made of nodes
 * 	@param nodeCount Number of nodes in the body of code
 */
void codebody(ast_default_node_t **nodes, unsigned int nodeCount) {
    
	for(unsigned int i = 0; i < nodeCount; i++) {
		switch ((*nodes)->type) {

			case AST_NODE_FN_DEF:

				functionDef((*nodes)->data_t.fnDef);
				break;
			case AST_NODE_FN_RETURN:

				functionReturn((*nodes)->data_t.fnReturn);
				break;
			case AST_NODE_FN_CALL:

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
			default:
				break;
		}
		nodes++;
	}
}

/**
 * 	Function that prints the header of an IFJcode24 file
 * 	Includes definition of the global throwaway variable GF@_%
 * 	Includes definitions of inbuilt functions
 * 	@brief Prints the header of an IFJcode24 file
 */

/**
 * I used % in the names of the variables in order to make sure that they are diiferent from variable names in the source code
 * I used % because it's forbidden to use % in variable names in zig
 */ 
void printHeader() {

	printf(".IFJcode24\n");
	printf("JUMP main\n");
	printf("DEFVAR GF@_%%\n");
	printf("\
LABEL ifjreadi32\n\
	CREATEFRAME\n\
	PUSHFRAME\n\
	DEFVAR LF@retval%%\n\
	DEFVAR LF@readValue%%\n\
	DEFVAR LF@readValueType%%\n\
	READ LF@readValue%% int\n\
	TYPE LF@readValueType%% LF@readValue%%\n\
	JUMPIFNEQ correct_input_int LF@readValueType%% nil@nil\n\
	MOVE LF@retval%% nil@nil\n\
	POPFRAME\n\
	RETURN\n\
	LABEL correct_input_int\n\
	MOVE LF@retval%% LF@readValue%%\n\
	POPFRAME\n\
RETURN\n\
\
LABEL ifjreadf64\n\
	CREATEFRAME\n\
	PUSHFRAME\n\
	DEFVAR LF@retval%%\n\
	DEFVAR LF@readValue%%\n\
	DEFVAR LF@readValueType%%\n\
	READ LF@readValue%% float\n\
	TYPE LF@readValueType%% LF@readValue%%\n\
	JUMPIFNEQ correct_input_float LF@readValueType%% nil@nil\n\
	MOVE LF@retval%% nil@nil\n\
	POPFRAME\n\
	RETURN\n\
	LABEL correct_input_float\n\
	MOVE LF@retval%% LF@readValue%%\n\
	POPFRAME\n\
RETURN\n\
\
LABEL ifjreadstr\n\
	CREATEFRAME\n\
	PUSHFRAME\n\
	DEFVAR LF@retval%%\n\
	DEFVAR LF@readValue%%\n\
	DEFVAR LF@readValueType%%\n\
	READ LF@readValue%% string\n\
	TYPE LF@readValueType%% LF@readValue%%\n\
	JUMPIFNEQ correct_input_string LF@readValueType%% nil@nil\n\
	MOVE LF@retval%% nil@nil\n\
	POPFRAME\n\
	RETURN\n\
	LABEL correct_input_string\n\
	MOVE LF@retval%% LF@readValue%%\n\
	POPFRAME\n\
RETURN\n\
\
LABEL ifji2f\n\
	CREATEFRAME\n\
	PUSHFRAME\n\
	\
	DEFVAR LF@retval%%\n\
	DEFVAR LF@param%%\n\
	POPS LF@param%%\n\
	INT2FLOAT LF@retval%% LF@param%%\n\
	\
	POPFRAME\n\
RETURN\n\
\
LABEL ifjf2i\n\
	CREATEFRAME\n\
	PUSHFRAME\n\
	\
	DEFVAR LF@retval%%\n\
	DEFVAR LF@param%%\n\
	POPS LF@param%%\n\
	FLOAT2INT LF@retval%% LF@param%%\n\
	\
	POPFRAME\n\
RETURN\n\
\
LABEL ifjlength\n\
	CREATEFRAME\n\
	PUSHFRAME\n\
	\
	DEFVAR LF@retval%%\n\
	DEFVAR LF@param%%\n\
	POPS LF@param%%\n\
	STRLEN LF@retval%% LF@param%%\n\
	\
	POPFRAME\n\
RETURN\n\
\
LABEL ifjconcat\n\
	CREATEFRAME\n\
	PUSHFRAME\n\
	\
	DEFVAR LF@retval%%\n\
	DEFVAR LF@param1%%\n\
	DEFVAR LF@param2%%\n\
	POPS LF@param2%%\n\
	POPS LF@param1%%\n\
	CONCAT LF@retval%% LF@param1%% LF@param2%%\n\
	\
	POPFRAME\n\
RETURN\n\
\
LABEL ifjsubstring\n\
	CREATEFRAME\n\
	PUSHFRAME\n\
	\
	DEFVAR LF@retval%%\n\
	MOVE LF@retval%% string@\n\
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
	CONCAT LF@retval%% LF@retval%% LF@substrChar%%\n\
	ADD LF@startIndex%% LF@startIndex%% int@1\n\
	SUB LF@substrCounter%% LF@substrCounter%% int@1\n\
	JUMP substrWhileCycle\n\
	LABEL substrCheckFailed\n\
	MOVE LF@retval%% nil@nil\n\
	LABEL substrCheckPassed\n\
	POPFRAME\n\
RETURN\n\
\
LABEL ifjstrcmp\n\
	CREATEFRAME\n\
	PUSHFRAME\n\
	\
	DEFVAR LF@retval%%\n\
	DEFVAR LF@str1Length%%\n\
	DEFVAR LF@str2Length%%\n\
	DEFVAR LF@usedLength%%\n\
	DEFVAR LF@string1%%\n\
	DEFVAR LF@string2%%\n\
	DEFVAR LF@strcmpInfo%% ## 0 - equal, 1 - str1 > str2, 2 - str1 < str2\n\
	DEFVAR LF@strcmpChar1%%\n\
	DEFVAR LF@strcmpChar2%%\n\
	DEFVAR LF@strcmpIndex%%\n\
	MOVE LF@strcmpIndex%% int@0\n\
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
	\
	\
	JUMPIFEQ strcmpEnd LF@usedLength%% int@0\n\
	STRI2INT LF@strcmpChar1%% LF@string1%% LF@strcmpIndex%%\n\
	STRI2INT LF@strcmpChar2%% LF@string2%% LF@strcmpIndex%%\n\
	EQ LF@strcmpBool%% LF@strcmpChar1%% LF@strcmpChar2%%\n\
	JUMPIFEQ strcmpCycleDiff LF@strcmpBool%% bool@false\n\
	ADD LF@strcmpIndex%% LF@strcmpIndex%% int@1\n\
	SUB LF@usedLength%% LF@usedLength%% int@1\n\
	JUMP strcmpCycle\n\
	\
	LABEL strcmpCycleDiff\n\
	LT LF@strcmpBool%% LF@strcmpChar1%% LF@strcmpChar2%%\n\
	JUMPIFEQ strcmpString1Less LF@strcmpBool%% bool@true\n\
	MOVE LF@retval%% int@1\n\
	POPFRAME\n\
	RETURN\n\
	\
	LABEL strcmpString1Less\n\
	MOVE LF@retval%% int@-1\n\
	POPFRAME\n\
	RETURN\n\
	\
	LABEL strcmpEnd\n\
	MOVE LF@retval%% LF@strcmpInfo%%\n\
	POPFRAME\n\
RETURN\n\
\
LABEL ifjord\n\
	CREATEFRAME\n\
	PUSHFRAME\n\
	\
	DEFVAR LF@retval%%\n\
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
	STRI2INT LF@retval%% LF@string%% LF@index%%\n\
	POPFRAME\n\
	RETURN\n\
	\
	LABEL ordCheckFailed\n\
	MOVE LF@retval%% int@0\n\
	POPFRAME\n\
RETURN\n\
\
LABEL ifjchr\n\
	CREATEFRAME\n\
	PUSHFRAME\n\
	\
	DEFVAR LF@retval%%\n\
	DEFVAR LF@char%%\n\
	POPS LF@char%%\n\
	INT2CHAR LF@retval%% LF@char%%\n\
	\
	POPFRAME\n\
RETURN\n\
");
}