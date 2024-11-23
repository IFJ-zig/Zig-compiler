/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#include "tokens.h"

#include "errors_enum.h"


KeyWord LGetKeyWAct(Token T) {
	return T.kw;
}

int LGetNumAct(Token T) {
	return T.i;
}

double LGetFloatAct(Token T) {
	return T.f;
}

char *LGetStrAct(Token *T) {
	return T->s;
}