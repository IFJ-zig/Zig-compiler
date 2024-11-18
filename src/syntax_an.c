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

int syntax_analyzer(List *L) {
	tokenList = L;
	LActFirst(tokenList);
	int statusCode;
	statusCode = seekHeaders();
	if (statusCode != 0)
		return statusCode;
	LActFirst(tokenList);
	statusCode = program();
	if (statusCode != 0)
		return statusCode;
	return 0;
};


int seekHeaders() {
	int statusCode = checkImport();
	if (statusCode != 0)
		return statusCode;
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
			fprintf(stderr, "ID: main\n");
		} else {
			fprintf(stderr, "ID: %s\n", token->s);
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
		statusCode = skip_function_body();
		if (statusCode != 0)
			return statusCode;
		get_token();
	}
	// TODO: CHECK IF MAIN FUNCTION IS DEFINED
	fprintf(stderr, "Headers OK\n");
	return 0;
};

int program() {
	int statusCode;
	statusCode = checkImport();
	if (statusCode != 0)
		return statusCode;

	// TODO: SOME CODE GENERATION STUFF
	while (1) {
		get_token();
		if (token->kw == _pub) {
			statusCode = function_analysis();
			if (statusCode != 0)
				return statusCode;
		} else if (token->kw == end) {
			fprintf(stderr, "\nCompilation successfully finished \n");
			return 0;
		} else {
			fprintf(stderr, "Error: Expected function definition or end of program, but found %d\n", token->kw);
			return 1;
		}
	}

	return 0;
};

int skip_function_body() {
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
		fprintf(stderr, "Error: Expected keyword 'constant' as start of an import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != inbuild) {
		fprintf(stderr, "Error: Expected identifier `ifj` for import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	get_token();
	if (token->kw != equal) {
		fprintf(stderr, "Error: Expected '=' after import identifier\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != at) {
		fprintf(stderr, "Error: Expected '@' before import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != _import) {
		fprintf(stderr, "Error: Expected keyword 'import' in import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != lbracket) {

		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != text && strcmp(token->s, "ifj24.zig")) {
		fprintf(stderr, "Error: Expected 'ifj24.zig' as name of the package\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != rbracket) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != next) {
		fprintf(stderr, "Error: Expected ';'\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	return 0;
}

int function_analysis() {
	int statusCode = 0;

	get_token();
	if (token->kw != _fn) {
		fprintf(stderr, "Error: Expected fn keyword\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw != id && token->kw != _main) {
		fprintf(stderr, "Error: Expected function id\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	// TODO: SOME SEMANTHIC ANALYSIS STUFF HERE
	fprintf(stderr, "Analysis ID: %s\n", token->s);

	statusCode = param_list();
	if (statusCode != 0)
		return statusCode;

	statusCode = return_type();
	if (statusCode != 0)
		return statusCode;

	get_token();
	if (token->kw != lblock) {
		fprintf(stderr, "Error: Expected '{' after function definition\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	while (token->kw != rblock) {
		statusCode = code(true);
		if (statusCode != 0) {
			return statusCode;
		}
		get_token();
	}


	// TOD: SEMANTHIC CHECKS FOR RETURN STATEMENT

	return 0;
}

int param_list() {
	int statusCode;
	get_token();
	if (token->kw != lbracket) {
		fprintf(stderr, "Error: Expected parameter list after function id\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	while (token->kw != rbracket) {
		printf("PARAM LIST\n");
		get_token();
		if (token->kw == rbracket) {
			break;
		}
		if (token->kw != id) {
			fprintf(stderr, "Error: Expected parameter id %d\n", token->kw);
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		get_token();
		if (token->kw != colon) {
			fprintf(stderr, "Error: Expected ':' after parameter id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}

		statusCode = data_type();
		if (statusCode != 0)
			return statusCode;
		get_token();
		if (token->kw != comma && token->kw != rbracket) {
			fprintf(stderr, "Error: Expected ',' or ')' after parameter data type\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
	}

	return 0;
}

int data_type() {
	get_token();
	if (token->kw == square_brackets) {
		get_token();
	}
	if (token->kw != dtint && token->kw != dtstr && token->kw != dtflt) {
		fprintf(stderr, "Error: Expected data type\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	return 0;
}

int return_type() {
	get_token();
	if (token->kw != dtint && token->kw != dtstr && token->kw != dtflt && token->kw != dtvoid) {
		fprintf(stderr, "Error: Expected return type\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	return 0;
}

int code(bool tokenWasGiven) {
	if (tokenWasGiven == false) {
		get_token();
	}

	int statusCode;
	switch (token->kw) {
		case constant:
		case variable:
			statusCode = variable_definition();
			if (statusCode != 0)
				return statusCode;
			break;
		case id:
			statusCode = call_or_assignment();
			if (statusCode != 0)
				return statusCode;
			break;
		case _if:
			statusCode = if_else();
			if (statusCode != 0)
				return statusCode;
			break;
		case _while:
			statusCode = while_syntax();
			if (statusCode != 0)
				return statusCode;
			break;
		case _return:
			statusCode = return_syntax();
			if (statusCode != 0)
				return statusCode;
			break;
		case inbuild:
			statusCode = inbuild_function();
			if (statusCode != 0)
				return statusCode;
			break;
		default:
			fprintf(stderr, "Error: Expected command but found: %d\n", token->kw);
			return 1;
	}

	return 0;
}

int if_else() {
	int statusCode;
	get_token();
	if (token->kw != lbracket) {
		fprintf(stderr, "Error: Expected '(' after if\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	// TODO: PLACEHOLDER FOR BOTTOM UP SYNTAX ANALYSIS
	skip_expression();
	get_token();
	if (token->kw == vertical_bar) {
		//TODO: SEMANTHIC CHECKS
		get_token();
		get_token();
		if (token->kw != vertical_bar) {
			fprintf(stderr, "Error: Expected '|' after unwrapped value id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		get_token();
	}

	if (token->kw != lblock) {
		fprintf(stderr, "Error: Expected '{' after if condition\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	while (token->kw != rblock) {
		statusCode = code(true);
		if (statusCode != 0)
			return statusCode;
		get_token();
	}

	get_token();
	if (token->kw == _else) {
		get_token();
		if (token->kw != lblock) {
			fprintf(stderr, "Error: Expected '{' after else\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		get_token();
		while (token->kw != rblock) {
			statusCode = code(true);
			if (statusCode != 0)
				return statusCode;
			get_token();
		}
	}

	return 0;
}

int skip_expression() {
	int statusCode;
	while (token->kw != rbracket) {
		get_token();
		if (token->kw == lbracket) {
			statusCode = skip_expression();
			if (statusCode != 0)
				return statusCode;
		}
	}
	return 0;
}

int return_syntax() {
	// TODO: SEMANTHIC CHECKS, BOTTOM UP EXPRESSION ANALYSIS AND CODE GENERATION

	//skip placeholder
	while (token->kw != next) {
		get_token();
	}
	return 0;
}

int inbuild_function() {
	int statusCode;
	get_token();
	if (token->kw != dot) {
		fprintf(stderr, "Error: Unexpected library usage\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();

	switch (token->kw) {
		case inord:
		case inchr:
		case inlen:
		case inssub:
		case inccat:
		case inu2s:
		case inscmp:
		case inres:
		case inrei:
		case inref:
		case inwrt:
		case ini2f:
		case inf2i:
			statusCode = skip_expression();
			if (statusCode != 0)
				return statusCode;
			break;
		default:
			fprintf(stderr, "Error: Undefined library call\n");
			break;
	}
	get_token();
	if (token->kw != next) {
		fprintf(stderr, "Error: Expected ';'\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	return 0;
}

int variable_definition() {
	int statusCode;
	get_token();
	if (token->kw != id) {
		fprintf(stderr, "Error: Expected variable id\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	if (token->kw == colon) {
		statusCode = data_type();
		if (statusCode != 0)
			return statusCode;
		get_token();
	}

	if (token->kw != equal) {
		fprintf(stderr, "Error: Expected '=' after variable type\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	// PLACEHOLDER FOR BOTTOM UP EXPRESSION ANALYSIS
	while (token->kw != next) {
		get_token();
	}

	return 0;
};
int call_or_assignment() {
	// TODO: SEMANTHIC ANALYSIS
	// I need semanthic analysis to define if id is a function or variable
	while (token->kw != next) {
		get_token();
	}
	return 0;
};

int while_syntax() {
	int statusCode;
	get_token();
	if (token->kw != lbracket) {
		fprintf(stderr, "Error: Expected '(' after while\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	skip_expression();
	get_token();
	if (token->kw == vertical_bar) {
		//TODO: SEMANTHIC CHECKS
		get_token();
		get_token();
		if (token->kw != vertical_bar) {
			fprintf(stderr, "Error: Expected '|' after unwrapped value id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		get_token();
	}
	if (token->kw != lblock) {
		fprintf(stderr, "Error: Expected '{' after while condition\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	get_token();
	while (token->kw != rblock) {
		statusCode = code(true);
		if (statusCode != 0)
			return statusCode;
		get_token();
	}
	return 0;
}