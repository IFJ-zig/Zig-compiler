/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
*********************************************/

#include "semantic_an.h"

static htabs_l *list;

void semanticInit(){
    list = symtable_init();
    EnterScope(list);
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
    symbol->value = value;
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