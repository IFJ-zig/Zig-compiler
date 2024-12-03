/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák - xhorakt00
*********************************************/

#include "semantic_an.h"

static htabs_l *list;

void semanticInit() {
	list = symtable_init();
	htab_t *t = htab_init(getCurrentDepth(list));
	t->previous = NULL;
	t->depth = 0;
	htab_insertLast(list, t);
}

void semanticDestroy() {
	while (list->tablesCount > 0) {
		htab_removeLast(list);
	}
	free(list);
}


void enterScope() {
	htab_t *t = htab_init(getCurrentDepth(list));
	t->depth = getCurrentDepth(list) + 1;
	htab_insertLast(list, t);
}

int exitScope() {
	for(int i = 0; i < HASH_TABLE_SIZE; i++){
		htab_itm_t *itm = list->last->arr_ptr[i];
		while(itm != NULL){
			if(itm->symbol.isUsed == false){
				fprintf(stderr, "Error: Variable %s at depth %d is unused\n", itm->symbol.key, itm->symbol.depth);
				return UNUSED_VARIABLE_ERROR;
			}
			if(itm->symbol.isChanged == false && itm->symbol.isConst == false){
				fprintf(stderr, "Error: Variable %s at depth %d is unchanged\n", itm->symbol.key, itm->symbol.depth);
				return UNUSED_VARIABLE_ERROR;
			}
			itm = itm->next;
		}
	}

	htab_removeLast(list);
	return 0;
}

int defineSymbol(char *name, varType type, bool isConst, bool isNullable) {
	symbol_t *symbol = htab_find(list->last, name);
	if (symbol != NULL) {
		fprintf(stderr, "Error: Symbol '%s' is already defined at depth %d\n", name, getCurrentDepth(list));
		return REDEFINITION_ERROR;
	}
	symbol = htab_define(list->last, name);
	symbol->type = type;
	symbol->isDefined = false;
	symbol->isConst = isConst;
	symbol->isNullable = isNullable;
	symbol->isUsed = false;
	symbol->isChanged = false;
	symbol->paramCount = 0;
	symbol->params = NULL;
	symbol->hasReturn = false;
	symbol->depth = getCurrentDepth(list);
	symbol->isMutable = true;
	fprintf(stderr, "Symbol '%s' defined at depth %d, isConst=%s, isNullable=%s, type=%s\n", name, getCurrentDepth(list), isConst ? "true" : "false", isNullable ? "true" : "false", type == INT ? "INT" : type == FLOAT ? "FLOAT"
																																																   : type == STRING      ? "STRING"
																																																   : type == FUNCTION    ? "FUNCTION"
																																																	: type == UNDEFINED		? "UNDEFINED"
																																																						 : "VOID");																																												 
	return 0;
}

void undefineSymbol(char *name) {
	htab_undefine(list->last, name);
}


symbol_t *getSymbol(char *name) {
	if(name == NULL){
		fprintf(stderr, "Error getSymbol: Symbol name is NULL\n");
		return NULL;
	}
	symbol_t *symbol = NULL;
	htab_t *t = list->last;
	while (symbol == NULL && t != NULL) {
		symbol = htab_find(t, name);
		t = t->previous;
	}
	if(symbol != NULL)
		symbol->isUsed = true;
	return symbol;
}

int assignSymbol(symbol_t *symbol, varType type) {
	if(symbol->type == UNDEFINED){
		fprintf(stderr, "Variable %s is of undefined type, assigning type %d\n", symbol->key, type);
		symbol->type = type;
	}
	if(type == VOID && symbol->isNullable == true)
		return 0;
	if(type == INT && symbol->type == FLOAT)
		return 0;
	else if (symbol->type != type)
		return TYPE_COMPATIBILITY_ERROR;
	return 0;
}

int processParam(Token paramName, Token paramType, bool isNullable) {
	if (!isValidParamType(paramType.kw)) {
		fprintf(stderr, "Error: %s is not a valid parameter type\n", paramType.s);
		return (PARAM_ERROR);
	}
	varType type;
	if (paramType.kw == dtint)
		type = INT;
	if (paramType.kw == dtflt)
		type = FLOAT;
	if (paramType.kw == dtstr)
		type = STRING;
	return defineSymbol(paramName.s, type, false, isNullable);
}

bool isValidParamType(KeyWord kw) {
	if (kw == dtint || kw == dtstr || kw == dtflt)
		return true;
	return false;
}


int assignFunctionParameter(symbol_t *function, Token paramName, Token paramType, bool isNullable) {
	fprintf(stderr, "    ┕Function %s has a parameter %s of type %s isNullable=%s\n", function->key, paramName.s, getTokenName(paramType), isNullable ? "true" : "false");
	function->paramCount++;
	function->params = realloc(function->params, function->paramCount * sizeof(symbol_t *));
	symbol_t *param = malloc(sizeof(symbol_t));
	param->key = paramName.s;
	param->type = kwToVarType(paramType.kw);
	param->isNullable = isNullable;
	param->isConst = false;
	param->isDefined = false;
	param->depth = -1;
	param->isMutable = false;
	function->params[function->paramCount - 1] = param;
	if(!strcmp(function->key, "main")){
		fprintf(stderr, "Error: main must not take any parameters!\n");
		return PARAM_ERROR;
	}				
	return 0;
}

bool nullKWInExpr(ast_node_exp_t *expression){
	if(expression->token->kw == _null){
		return true;
	}
	if(expression->data_t.binary_op.left != NULL || expression->data_t.binary_op.right != NULL){
		if(nullKWInExpr(expression->data_t.binary_op.left))
			return true;
		if(nullKWInExpr(expression->data_t.binary_op.right))
			return true;
	}
	return false;
}

bool nullableInExpr(ast_node_exp_t *expression){
	if(expression->token->kw == id){
		symbol_t *sym = getSymbol(expression->token->s);
		if(sym == NULL){
			fprintf(stderr, "Error: Variable %s has not been defined\n", expression->token->s);
			return false;
		}
		if(sym->isNullable){
			fprintf(stderr, "Variable %s is nullable\n", sym->key);
			return true;
		}
	}
	if(expression->data_t.binary_op.left != NULL || expression->data_t.binary_op.right != NULL){
		if(nullableInExpr(expression->data_t.binary_op.left))
			return true;
		if(nullableInExpr(expression->data_t.binary_op.right))
			return true;
	}
	return false;
}

bool containsNonConst(ast_node_exp_t *expression){
	if(expression->token->kw == id){
		symbol_t *sym = getSymbol(expression->token->s);
		if(sym == NULL){
			fprintf(stderr, "Error: Variable %s has not been defined\n", expression->token->s);
			return false;
		}
		if(!sym->isConst){
			fprintf(stderr, "Variable %s is not const\n", sym->key);
			return true;
		}
	}
	if(expression->data_t.binary_op.left != NULL || expression->data_t.binary_op.right != NULL){
		if(containsNonConst(expression->data_t.binary_op.left))
			return true;
		if(containsNonConst(expression->data_t.binary_op.right))
			return true;
	}
	return false;
}

int exprTypeCompatibility(ast_node_exp_t *expression, varType *type){
	if(expression->token->kw == id){
		symbol_t *sym = getSymbol(expression->token->s);
		if(sym == NULL){
			fprintf(stderr, "Error: Variable %s has not been defined\n", expression->token->s);
			return UNDEFINED_FUNCTION_OR_VARIABLE_ERROR;
		}
		varType newType;
		if(sym->type == FUNCTION)
			newType = sym->returnType;
		else
			newType = sym->type;
		if(*type == UNDEFINED){
			*type = newType;
			if(expression->data_t.binary_op.left != NULL || expression->data_t.binary_op.right != NULL){
				exprTypeCompatibility(expression->data_t.binary_op.left, type);
				exprTypeCompatibility(expression->data_t.binary_op.right, type);
			}
		}
		else if(*type != newType)
			return TYPE_COMPATIBILITY_ERROR;
	}
	return 0;
}

int checkExprTypesCompatibility(ast_node_exp_t *expression, varType *expType){
	//Ignoring nums because those can be converted to floats
	*expType = UNDEFINED;
	if(expression->token->kw == id){
		symbol_t *sym = getSymbol(expression->token->s);
		if(sym == NULL){
			fprintf(stderr, "Error: Variable %s has not been defined\n", expression->token->s);
			return UNDEFINED_FUNCTION_OR_VARIABLE_ERROR;
		}
		if(sym->type == FUNCTION)
			*expType = sym->returnType;
		else
			*expType = sym->type;
		fprintf(stderr, "checkEXPR Variable %s is of type %s\n", sym->key, *expType == INT ? "INT" : *expType == FLOAT ? "FLOAT" : *expType == STRING ? "STRING" : *expType == ANYTYPE ? "ANYTYPE" : "VOID");
	}
	int statusCode;
	if(expression->data_t.binary_op.left != NULL || expression->data_t.binary_op.right != NULL){
		statusCode = exprTypeCompatibility(expression->data_t.binary_op.left, expType);
		if(statusCode != 0)
			return statusCode;
		statusCode = exprTypeCompatibility(expression->data_t.binary_op.right, expType);
		if(statusCode != 0)
			return statusCode;
	}
	fprintf(stderr, "expr type compatibility ok\n");
	return 0;
}

varType kwToVarType(KeyWord kw) {
	if (kw == dtint || kw == num)
		return INT;
	if (kw == dtflt || kw == decim)
		return FLOAT;
	if (kw == dtstr || kw == text)
		return STRING;
	return VOID;
}

int isValidIfExpression(ast_node_exp_t *expression){
	KeyWord kw = expression->token->kw;	//Expression in if must be of a boolean type, so it must contain more, less, compare_equal, nequal or be an id
	if(kw == compare_equal || kw == nequal || kw == more || kw == less || kw == lequal || kw == mequal || kw == id)
		return 0;
	return TYPE_COMPATIBILITY_ERROR;
}