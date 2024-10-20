/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/
#include "syntax_an.h"

Token *token;
List *tokenList;

int get_token() {
	token = LGetAct(tokenList);
	LActNext(tokenList);
	return 0;
}

int Syntax_analyzer(List *L) {
	tokenList = L;
	LActFirst(tokenList);
	code();
	return 0;
};

int code() {
	get_token();
}