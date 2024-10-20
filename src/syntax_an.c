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
	seekHeaders();
	LActFirst(tokenList);
	program();
	return 0;
};


int seekHeaders() {
	get_token();
	if (token->kw != constant) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != inbuild) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	get_token();
	if (token->kw != compare_equal) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != at) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != _import) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != lbracket) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != text && strcmp(token->s, "ifj24.zig")) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != rbracket) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != next) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	printf("Headers OK\n");
	return 0;
};

int program() {
	return 0;
};

int code() {
	return 0;
}
