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
	inord,
	inchr,
	inlen,
	inssub,
	inccat,
	inu2s,
	inscmp,
	//ord, chr, length, substring, concat, string, strcmp
	inres,
	inrei,
	inref,
	inwrt,
	ini2f,
	inf2i,
	//readstr, readi32, readf64, write, i2f,   f2i
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
	int *i;
	double *f;
	char *s;
	struct SToken *next;
} Token;

typedef struct SList
{
	Token *first;
	Token *active;
	Token *last;
} List;

void ListInit(List *L);

int LInsertLast(List *L, KeyWord key, int *a, double *b, char *c);

void LActFirst(List *L);

void LActNext(List *L);

Token *LGetFirst(List *L);

Token *LGetAct(List *L);

KeyWord LGetKeyWFirst(List *L);

KeyWord LGetKeyWAct(List *L);

int *LGetNumFirst(List *L);

int *LGetNumAct(List *L);

double *LGetFloatFirst(List *L);

double *LGetFloatAct(List *L);

char *LGetStrFirst(List *L);

char *LGetStrAct(List *L);

void LDeleteFirst(List *L);

void LDestroy(List *L);

int Lexem_analyzer(List *L);

#endif // TOKENS_H