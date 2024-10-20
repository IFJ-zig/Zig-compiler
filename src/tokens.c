/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#include "tokens.h"

#include "errors_enum.h"

void ListInit(List *L) {
	//nastaví pointry na null
	L->first = NULL;
	L->active = NULL;
	L->last = NULL;
}

int LInsertLast(List *L, KeyWord key, int *i, double *f, char *s) {
	//přidá token na konec, pokud je list prázdný, je přidaný i prvním
	Token *new = (Token *)malloc(sizeof(Token));
	if (new == NULL) {
		return LEXEM_ERROR;
	} else {
		new->kw = key;
		new->i = i;
		new->f = f;
		new->s = s;
		if (L->last != NULL) {
			new->next = L->last->next;
			L->last->next = new;
			L->last = new;
		} else {
			new->next = L->last;
			L->first = new;
			L->last = new;
		}
	}
	return 0;
}

void LActFirst(List *L) {
	//zaktivuje první prvek
	L->active = L->first;
}

void LActNext(List *L) {
	//pokud je list aktivní, přesune aktivitu na následující prvek
	if (L->active != NULL) {
		L->active = L->active->next;
	}
}

Token *LGetFirst(List *L) {
	//vrátí první token nebo NULL
	return L->first;
}

Token *LGetAct(List *L) {
	//vrátí první token nebo NULL
	return L->active;
}

KeyWord LGetKeyWFirst(List *L) {
	return L->first->kw;
}

KeyWord LGetKeyWAct(List *L) {
	return L->active->kw;
}

int *LGetNumFirst(List *L) {
	return L->first->i;
}

int *LGetNumAct(List *L) {
	return L->active->i;
}

double *LGetFloatFirst(List *L) {
	return L->first->f;
}

double *LGetFloatAct(List *L) {
	return L->active->f;
}

char *LGetStrFirst(List *L) {
	return L->first->s;
}

char *LGetStrAct(List *L) {
	return L->active->s;
}

void LDeleteFirst(List *L) {
	//uvolní token a případné parametry
	if (L->first != NULL) {
		if (L->first == L->active) {
			L->active = NULL;
		}
		if (L->first == L->last) {
			L->last = NULL;
		}
		Token *Elem = L->first;
		L->first = L->first->next;
		if (Elem->i != NULL) {
			free(Elem->i);
		}
		if (Elem->f != NULL) {
			free(Elem->f);
		}
		if (Elem->s != NULL) {
			free(Elem->s);
		}
		free(Elem);
	}
}

void LDestroy(List *L) {
	while (L->first != NULL) {
		L->active = L->first->next;
		LDeleteFirst(L);
		L->first = L->active;
	}
	L->last = NULL;
}