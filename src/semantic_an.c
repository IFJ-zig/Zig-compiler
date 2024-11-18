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

int defineSymbol(char *name, varType type, bool isConst, bool isNullable){
    symbol_t *symbol = htab_find(list->last, name);
    if(symbol != NULL)
        return REDEFINITION_ERROR;
    symbol = htab_define(list->last, name);
    symbol->type = type;
    symbol->isDefined = false;
    symbol->isConst = isConst;
    symbol->isNullable = isNullable;
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

bool assignSymbol(char *name, KeyWord kw){
    symbol_t *symbol = getSymbol(name);
    if(symbol == NULL)
        return false;   //Symbol does not exist
    symbol->isDefined = true;
    if(symbol->isNullable == true){
        if(kw == _null)
            return true;
    }
    if(symbol->type == INT){
        if(kw == dtint){
            return true;
        }
    }
    else if(symbol->type == FLOAT){
        if(kw == dtflt || kw == dtint){
            return true;
        }
    }
    else if(symbol->type == STRING)
        if(kw == dtstr){
            return true;
        }
    return false;
}

//Entering function, get 3 tokens from which extract paramName and paramType, this functions expects to already be shifted into the function scope
int processParam(Token paramName, Token paramType){
	if(!isValidParamType(paramType.kw)){
		fprintf(stderr, "Error: %s is not a valid parameter type\n", paramType.s);
        return(PARAM_ERROR);
    }
    varType type;
    if(paramType.kw == dtint)
        type = INT;
    if(paramType.kw == dtflt)
        type = FLOAT;
    if(paramType.kw == dtstr)
        type = STRING;
    defineSymbol(paramName.s, type, false, false); //Function can also return a nullable thing, figure this out later
    return 0;
}

bool isValidParamType(KeyWord kw){
    if(kw == dtint || kw == dtstr || kw == dtflt)
        return true;
    return false;
}