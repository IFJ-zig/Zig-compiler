/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#ifndef TOKENS_H
#define TOKENS_H

#include <stddef.h> //NULL
#include <stdio.h>  // printf
#include <stdlib.h> // malloc, free
#include <string.h> //strcat,cmp,cpy,len


typedef enum
{
	LEXEM,
	INTERNAL,
	//pro chybu v lexemu
	num,
	text,
	decim,
	id,
	//čislo, text, desetiné, názvy
	dtint,
	dtstr,
	dtflt,
	dtvoid,
	constant,
	variable,
	_null,
	//i32, u8,    f64,   void,   const, var,  null
	_if,
	_else,
	_while,
	_fn,
	_return,
	_pub,
	_main,
	inbuild,
	_import,
	//if, else,  while, fn,   return,pub,     main,   ifj,    import
	colon,
	comma,
	next,
	underscore,
	dot,
	//:,    ,    ;,    _,    .
	plus,
	minus,
	multiply,
	division,
	//+,  -,     *,    /
	question_mark,
	at,
	vertical_bar,
	square_brackets,
	//?,    @,     |,      []
	equal,
	less,
	more,
	lequal,
	mequal,
	compare_equal,
	nequal,
	//=,   <,    >,    <=,     >=,     ==,    !=,
	lbracket,
	rbracket,
	lblock,
	rblock,
	end,
	//(,    ),      {,     },     konec
	EMPTY = -1,
} KeyWord;

typedef struct SToken
{
	KeyWord kw;
	int i;
	double f;
	char *s;
} Token;

KeyWord LGetKeyWAct(Token T);

int LGetNumAct(Token T);

double LGetFloatAct(Token T);

char *LGetStrAct(Token *T);

Token get_token();

char *getTokenName(Token T);

Token *allocateToken(Token tkn);

#endif // TOKENS_H