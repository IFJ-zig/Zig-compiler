/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#ifndef TOKENS_H
#define TOKENS_H

#include <stdio.h> // printf
#include <stdlib.h> // malloc, free
#include <stddef.h> //NULL
#include <string.h> //strcat,cmp,cpy,len
#include <time.h> //time

typedef enum {num,   text, decim,    id,
            //čislo, text, desetiné, názvy
            dtint,     dtstr,  dtdbl, dtnic, dtintn, dtstrn, dtdbln,
            //integer, string, Double, nil,  int?,   str?,   double?
            inres,   inrei, inred, inwrt, ini2d,  ind2i,
            //reads, readi, readD, write, int2dbl,dbl2int
            inlen,   inssub,    inord, inchr,
            //length, substring, ord,   chr
            kdyz, jinak, loop,  funk, back,   konst, prom,
            //if, else,  while, func, return, let,   var
            rovno, less, more, lequal, mequal, equal, nequal, choice,
            //=,   <,    >,    <=,     >=,     ==,    !=,     ??
            plus, minus, krat, deleno, nonnil,
            //+,  -,     *,    /,      !
            dvojt, line, next, rettyp, pass,
            //:,     ,   \n,   ->,     _,
            lzavor, pzavor, lblok, pblok, end
            //(,    ),      {,     },     konec
}KeyWord;

typedef struct SToken{
    KeyWord kw;
    int *i;
    double *f;
    char *s;
    struct SToken *next;
}Token;

typedef struct SList{
	Token *first;
    Token *active;
    Token *last;
}List;

void ListInit(List *L);

int LInsertLast(List *L, KeyWord key, int *a, double *b, char *c);

void LActFirst(List *L);

void LActNext(List *L);

Token* LGetFirst(List *L);

Token* LGetAct(List *L);

KeyWord LGetKeyWFirst(List *L);

KeyWord LGetKeyWAct(List *L);

int* LGetNumFirst(List *L);

int* LGetNumAct(List *L);

double* LGetFloatFirst(List *L);

double* LGetFloatAct(List *L);

char* LGetStrFirst(List *L);

char* LGetStrAct(List *L);

void LDeleteFirst(List *L);

void LDestroy(List *L);

int Lexem_analyzer(List *L);

#endif
