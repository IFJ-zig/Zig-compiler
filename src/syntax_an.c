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
	int success;
	success = seekHeaders();
	if (success != 0)
		return success;
	LActFirst(tokenList);
	success = program();
	if (success != 0)
		return success;
	return 0;
};


int seekHeaders() {
	int success = checkImport();
	if (success != 0)
		return success;
	get_token();
	while (token->kw != end) {
		if (token->kw != _pub) {
			fprintf(stderr, "Error: Expected function definiton starting with `pub`\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		get_token();
		if (token->kw != _fn) {
			fprintf(stderr, "Error: Expected keyword 'fn' in header\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		get_token();
		if (token->kw != id && token->kw != _main) {
			fprintf(stderr, "Error: Expected function id in header\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		// TODO: SOME SEMANTHIC CHECKS
		if (token->kw == _main) {
			printf("ID: main\n");
		} else {
			printf("ID: %s\n", token->s);
		}

		get_token();
		if (token->kw != lbracket) {
			fprintf(stderr, "Error: Expected '(' after function id in header\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		// TODO: PLACEHOLDER FOR PARAM LIST CHECK
		while (token->kw != rbracket) {
			get_token();
		}

		if (token->kw != rbracket) {
			fprintf(stderr, "Error: Expected ')' after param list in function header\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}

		// TODO: PLACEHOLDER FOR RETURN TYPE
		get_token();
		get_token();

		// TODO: AGAIN SOME SEMANTHIC CHECKS
		success = skipFunctionBody();
		if (success != 0)
			return success;
		get_token();
	}
	// TODO: CHECK IF MAIN FUNCTION IS DEFINED
	printf("Headers OK\n");
	return 0;
};

int program() {
	int success;
	success = checkImport();
	if (success != 0)
		return success;

	get_token();
	return 0;
};

int code() {
	return 0;
}


int skipFunctionBody() {
	if (token->kw != lblock)
		return SYNTACTIC_ANALYSIS_ERROR;
	int braceCount = 0;
	do {
		if (token->kw == lblock) {
			++braceCount;
		} else if (token->kw == rblock) {
			--braceCount;
			if (braceCount == 0)
				break;
		}

		get_token();
	} while (braceCount != 0 && token);

	if (braceCount == 0) {
		return 0;
	} else {
		return 1;
	}
}


int checkImport() {
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

	return 0;
}