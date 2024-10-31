/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
*********************************************/

#include "semantic_an.h"

static htabs_l *list;

void semanticInit(){
    list = symtable_init();
    enterScope(list);
}

void enterScope(){
    htab_t *t = htab_init(getCurrentDepth(list));
    htab_insertLast(list, t);
}

void exitScope(){
    htab_removeLast(list);
}

int defineSymbol(char *name, varType type){
    symbol_t *symbol = htab_find(list->last, name);
    if(symbol != NULL)
        return REDEFINITION_ERROR;
    symbol = htab_define(list->last, name);
    symbol->type = type;
    return 0;
}

symbol_t *getSymbol(char *name){
    symbol_t *symbol = NULL;
    htab_t *t = list->last;
    while(symbol == NULL && t != NULL){
        symbol = htab_find(t, name);
        t = t->previous;
    }
    return symbol;
}

bool assignSymbol(char *name, char *value, KeyWord kw){
    value = value;  //just so compiler shuts up
                    //i dont think we need to track the actual value of symbols since we're not building the AST
                    //keeping track now might make codegen easier but who gaf?
    symbol_t *symbol = getSymbol(name);
    if(symbol == NULL)
        return false;   //Symbol does not exist
    if(symbol->type == INT){
        if(kw == dtint)
            return true;
    }
    else if(symbol->type == FLOAT){
        if(kw == dtflt || kw == dtint)
            return true;
    }
    else if(symbol->type == STRING)
        if(kw == dtstr)
            return true;
    return false;
}

//Entering function, get 3 tokens from which extract paramName and paramType, this functions expects to already be shifted into the function scope
void processParam(Token paramName, Token paramType){
	if(!isValidParamType(paramType.kw)){
		fprintf(stderr, "Error: %s is not a valid parameter type\n", paramType.s);
        exit(PARAM_ERROR);
    }
    varType type;
    if(paramType.kw == dtint)
        type = INT;
    if(paramType.kw == dtflt)
        type = FLOAT;
    if(paramType.kw == dtstr)
        type = STRING;
    defineSymbol(paramName.s, type);
}

bool isValidParamType(KeyWord kw){
    if(kw == dtint || kw == dtstr || kw == dtflt)
        return true;
    return false;
}