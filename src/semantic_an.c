/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
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

void exitScope() {
	htab_removeLast(list);
	//ALLOCATE SYMBOLS IN AST TREE AND KEEP THIS AS IT WAS! IT WILL BE FINE, TRUST
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
	symbol->paramCount = 0;
	symbol->params = NULL;
	symbol->depth = getCurrentDepth(list);
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

//Entering function, get 3 tokens from which extract paramName and paramType, this functions expects to already be shifted into the function scope
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
	defineSymbol(paramName.s, type, false, isNullable); //Function can also return a nullable thing, figure this out later
	return 0;
}

bool isValidParamType(KeyWord kw) {
	if (kw == dtint || kw == dtstr || kw == dtflt)
		return true;
	return false;
}


//This function creates a memore leak, because the symbols created for params are never freed, fix before final version TODO
int assignFunctionParameter(symbol_t *function, Token paramName, Token paramType, bool isNullable) {
	fprintf(stderr, "  ┕Function %s has a parameter %s of type %s isNullable=%s\n", function->key, paramName.s, getTokenName(paramType), isNullable ? "true" : "false");
	function->paramCount++;
	function->params = realloc(function->params, function->paramCount * sizeof(symbol_t *));
	symbol_t *param = malloc(sizeof(symbol_t));
	param->key = paramName.s;
	param->type = kwToVarType(paramType.kw);
	param->isNullable = isNullable;
	param->isConst = false;
	param->isDefined = false;
	param->depth = -1;
	function->params[function->paramCount - 1] = param;
	if(!strcmp(function->key, "main")){
		fprintf(stderr, "Error: main must not take any parameters!\n");
		return PARAM_ERROR;
	}				
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