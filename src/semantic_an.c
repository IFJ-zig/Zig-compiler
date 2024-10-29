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

int defineSymbol(char *name, char *value, varType type){
    symbol_t *symbol = htab_find(list->last, name);
    if(symbol != NULL)
        return REDEFINITION_ERROR;
    symbol = htab_define(list->last, name);
    symbol->type = type;
    //symbol->value = value;
    value = value; //just so compiler doesnt freak out, don't worry ill delete this
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
    defineSymbol(paramName.s, NULL, type);
}

bool isValidParamType(KeyWord kw){
    if(kw == dtint || kw == dtstr || kw == dtflt)
        return true;
    return false;

}