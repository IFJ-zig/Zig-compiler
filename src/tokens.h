/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains declarations of functions used for lexical analysis
 *  @file  tokens.h
 *  @author Ivo Puchnar, xpuchn02
 *  @brief Declaration file for lexical analysis
 */

#ifndef TOKENS_H
#define TOKENS_H

#include <stddef.h> //NULL
#include <stdio.h>  // printf
#include <stdlib.h> // malloc, free
#include <string.h> //strcat,cmp,cpy,len

/**
 * @enum KeyWord
 * @brief Enum for the type of the lexical token.
 */
typedef enum{
	LEXEM,
	INTERNAL,
	//for errors in lexical analysis
	num,
	text,
	decim,
	id,
	//whole numbers, text, decimal numbers, identifiers
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
	//(,    ),      {,     },     end of file
	EMPTY = -1,
} KeyWord;

/**
 * @struct Token
 * @brief Struct for the lexical token.
 */
typedef struct SToken{
	KeyWord kw;			//type of returned lexical token
	int i;				//value of a whole number constant
	double f;			//value of a decimal number constant
	char *s;			//address of dynamically allocated memory for a string of characters
} Token;

/**
 * @brief Returns the type of given token
 * 
 * @param T Unknown token
 * @return KeyWord Type of lexical token
 */
KeyWord LGetKeyWAct(Token T);

/**
 * @brief Returns the whole number value of given token.
 * 
 * @param T Token of type num.
 * @note if you ask for number of other types, you will get default 0
 * @return int Whole number value
 */
int LGetNumAct(Token T);

/**
 * @brief Returns the decimal number value of given token.
 * 
 * @param T Token of type decim.
 * @note if you ask for number of other types, you will get default 0
 * @return double Decimal number value
 */
double LGetFloatAct(Token T);

/**
 * @brief Returns the address of dynamically allocated string
 * 
 * @param T Token of type text, id, num or decim.
 * @note if you ask for string address of other types, you will get default NULL
 * @return char* Address of string
 */
char *LGetStrAct(Token *T);

/**
 * @brief Creates a lexical token
 * 
 * @note The function performs lexical analysis. It is a finite state machine that takes characters from the input and creates tokens based on them, which it passes to the syntactic analysis.
 * @return Token Lexical token
 */
Token get_token();

/**
 * @brief Returns an address of a constant string according to the type of the given token.
 * 
 * @param T Unknown token
 * @note used to turn enum value to a word
 * @return char* Address of string
 */
char *getTokenName(Token T);

/**
 * @brief allocates the token and its contents
 * 
 * @param tkn A lexical token
 * @return Token* Address of allocated Token
 */
Token *allocateToken(Token tkn);

#endif // TOKENS_H