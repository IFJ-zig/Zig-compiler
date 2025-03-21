/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains implementation of support functions used for lexical analysis
 *  @file  Lexem_an.c
 *  @author Ivo Puchnar, xpuchn02
 *  @brief Implementation file for lexical analysis
 */

#include "tokens.h"
#include "errors_enum.h"

Token *allocateToken(Token tkn) {
	Token *T = (Token *)malloc(sizeof(Token));
	if (T == NULL) {
		exit(INTERNAL_COMPILER_ERROR);
	}
	T->kw = tkn.kw;
	T->i = tkn.i;
	T->f = tkn.f;

	// Ensure independent copy of the string
	if (tkn.s != NULL) {
		// strdup allocates memory and copies the string
		T->s = strdup(tkn.s);
		// Clean up allocated memory if strdup fails
		if (T->s == NULL) {
			free(T);
			exit(INTERNAL_COMPILER_ERROR);
		}
	} else {
		T->s = NULL;
	}

	return T;
}


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

char *getTokenName(Token T) {
	switch (T.kw) {
		case LEXEM:
			return "LEXEM";
		case INTERNAL:
			return "INTERNAL";
		case num:
			return "num";
		case text:
			return "text";
		case decim:
			return "decim";
		case id:
			return "id";
		case dtint:
			return "dtint";
		case dtstr:
			return "dtstr";
		case dtflt:
			return "dtflt";
		case dtvoid:
			return "dtvoid";
		case constant:
			return "constant";
		case variable:
			return "variable";
		case _null:
			return "_null";
		case _if:
			return "_if";
		case _else:
			return "_else";
		case _while:
			return "_while";
		case _fn:
			return "_fn";
		case _return:
			return "_return";
		case _pub:
			return "_pub";
		case _main:
			return "_main";
		case inbuild:
			return "inbuild";
		case _import:
			return "_import";
		case colon:
			return "colon";
		case comma:
			return "comma";
		case next:
			return "next";
		case underscore:
			return "underscore";
		case dot:
			return "dot";
		case plus:
			return "plus";
		case minus:
			return "minus";
		case multiply:
			return "multiply";
		case division:
			return "division";
		case question_mark:
			return "question_mark";
		case at:
			return "at";
		case vertical_bar:
			return "vertical_bar";
		case square_brackets:
			return "square_brackets";
		case equal:
			return "equal";
		case less:
			return "less";
		case more:
			return "more";
		case lequal:
			return "lequal";
		case mequal:
			return "mequal";
		case compare_equal:
			return "compare_equal";
		case nequal:
			return "nequal";
		case lbracket:
			return "lbracket";
		case rbracket:
			return "rbracket";
		case lblock:
			return "lblock";
		case rblock:
			return "rblock";
		case end:
			return "end";
		case EMPTY:
			return "EMPTY";
		default:
			return "UNKNOWN KEYWORD";
	}
}