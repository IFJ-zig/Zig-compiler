/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/
#include "syntax_an.h"

#include "semantic_an.h"

//TODO: Semantic integration leaking like a sieve, a fix would be nice but probably not necessary for the project


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

		//Save the name of functions into the symtable at depth 0
		if (defineSymbol(token->s, FUNCTION, false, false) == REDEFINITION_ERROR) {
			fprintf(stderr, "Error: Function %s is already defined\n", token->s);
			return REDEFINITION_ERROR;
		}
		symbol_t *fnSymbol = getSymbol(token->s);
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
		get_token();
		while (token->kw != rbracket) { //Process the parameters
			if (token->kw == id) {
				fprintf(stderr, "Param ID: %s\n", token->s);
				Token paramID = *token;
				get_token();
				if (token->kw != colon) {
					fprintf(stderr, "Error: Expected ':' after parameter id\n");
					return SYNTACTIC_ANALYSIS_ERROR;
				}
				data_type();

				assignFunctionParameter(fnSymbol, paramID, *token, false); //TODO: isNullable is hardcoded to false for now, code doesn't support optionals yet. This MUST be changed before final version!
				get_token();
				if (token->kw != comma && token->kw != rbracket) {
					fprintf(stderr, "Error: Expected ',' or ')' after parameter data type\n");
					return SYNTACTIC_ANALYSIS_ERROR;
				}
			} else
				get_token();
		}
		fprintf(stderr, "Function %s has %d parameters\n", fnSymbol->key, fnSymbol->paramCount);
		fprintf(stderr, "Param list OK\n\n");

		if (token->kw != rbracket) {
			fprintf(stderr, "Error: Expected ')' after param list in function header\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		get_token();
		if (isValidReturnType(token->kw)) {
			fnSymbol->returnType = kwToVarType(token->kw); //Save the return type of the function
		} else {
			fprintf(stderr, "Error: Invalid return type of function %s\n", fnSymbol->key);
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		get_token();

		statusCode = skip_function_body();
		if (statusCode != 0)
			return statusCode;
		get_token();
	}
	if (getSymbol("main") == NULL) {
		fprintf(stderr, "Error: Main function is not defined\n");
		return UNDEFINED_FUNCTION_OR_VARIABLE_ERROR; //TODO: Is this the correct error code?
	}
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
	enterScope();
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
	fprintf(stderr, "Analysis ID: %s\n", token->s);
	//The function name should already be in the symtable at depth 0 since it was done in seekHeaders, lets just check if it's still there
	if (getSymbol(token->s) == NULL) {
		fprintf(stderr, "Error: Function %s is not defined, this function should've been defined in seekHeaders!\n", token->s);
		return UNDEFINED_FUNCTION_OR_VARIABLE_ERROR;
	} else {
		fprintf(stderr, "Function %s successfully found in symtable, depth=%d, return type=%s\n", token->s, getSymbol(token->s)->depth, getSymbol(token->s)->returnType == INT ? "INT" : getSymbol(token->s)->returnType == FLOAT ? "FLOAT"
																																												 : getSymbol(token->s)->returnType == STRING      ? "STRING"
																																																								  : "VOID");
	}

	statusCode = param_list(); //done semantic
	if (statusCode != 0)
		return statusCode;

	statusCode = return_type(); //semantic done in seekHeaders
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


	// TODO: SEMANTHIC CHECKS FOR RETURN STATEMENT
	exitScope();
	return 0;
}

int param_list() {
	//Expect to already be in the function scope
	int statusCode;
	get_token();
	if (token->kw != lbracket) {
		fprintf(stderr, "Error: Expected parameter list after function id\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	while (token->kw != rbracket) {
		fprintf(stderr, "PARAM LIST\n");
		get_token();
		if (token->kw == rbracket) {
			break;
		}
		if (token->kw != id) {
			fprintf(stderr, "Error: Expected parameter id %d\n", token->kw);
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		Token paramID = *token;
		get_token();
		if (token->kw != colon) {
			fprintf(stderr, "Error: Expected ':' after parameter id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}

		statusCode = data_type();
		if (statusCode != 0)
			return statusCode;
		processParam(paramID, *token, false); //TODO: isNullable is hardcoded to false for now, code doesn't support optionals yet. This MUST be changed before final version!
		fprintf(stderr, "Param %s of type %s loaded into symtable at depth %d\n", paramID.s, token->kw == dtint ? "INT" : token->kw == dtflt ? "FLOAT"
																																			 : "STRING",
				getSymbol(paramID.s)->depth);
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

bool isValidReturnType(KeyWord kw) {
	if (kw != dtint && kw != dtstr && kw != dtflt && kw != dtvoid)
		return false;
	return true;
}

int return_type() {
	get_token();
	if (!isValidReturnType(token->kw)) {
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
			statusCode = variable_definition(true); //semantic done, exp done
			if (statusCode != 0)
				return statusCode;
			break;
		case variable:
			statusCode = variable_definition(false); //semantic done, exp done
			if (statusCode != 0)
				return statusCode;
			break;
		case id:
			statusCode = call_or_assignment(); //TODO: this
			if (statusCode != 0)
				return statusCode;
			break;
		case _if:
			statusCode = if_else(); //semantic done
			if (statusCode != 0)
				return statusCode;
			break;
		case _while:
			statusCode = while_syntax(); //semantic done
			if (statusCode != 0)
				return statusCode;
			break;
		case _return:
			statusCode = return_syntax(); //No can do without precedent_an
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
	enterScope();
	get_token();
	if (token->kw != lbracket) {
		fprintf(stderr, "Error: Expected '(' after if\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	// TODO: PLACEHOLDER FOR BOTTOM UP SYNTAX ANALYSIS
	//skip_expression();
	symbol_t *mainSymbol = skip_expression_get_symbol();
	get_token();
	if (token->kw == vertical_bar) {
		get_token();
		if (mainSymbol->type == VOID) {
			fprintf(stderr, "skip_expression_get_type() has not found any IDs in the expression, however, unwrapped value was still created, this should've resulted in a compile error\n ");
		}
		defineSymbol(token->s, mainSymbol->type, mainSymbol->isConst, false);
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
	exitScope(); //Exit the scope of the if statement has to be here since we can't have the unwrapped value reach the else block
	get_token();
	if (token->kw == _else) {
		enterScope();
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
		exitScope();
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

//This function is a hack just to get if_else working without a working precedent_an
//Skips the expression and returns the type of the first ID it finds, VOID if no ID is found, or if the id does not exist in the symtable
//I am not particularly proud of this function, but it works for now
symbol_t *skip_expression_get_symbol() {
	int statusCode;
	symbol_t *foundSymbol = NULL;
	while (token->kw != rbracket) {
		get_token();
		if (token->kw == lbracket) {
			statusCode = skip_expression();
			if (statusCode != 0)
				return 0;
		}
		if (token->kw == id) {
			symbol_t *symbol = getSymbol(token->s);
			if (symbol == NULL) {
				fprintf(stderr, "Error: Variable %s has not been defined\n", token->s);
			} else
				foundSymbol = symbol;
		}
	}
	return foundSymbol;
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

int variable_definition(bool isConst) {
	bool isNullable = false; //TODO: isNullable is hardcoded to false for now, code doesn't support optionals yet. This MUST be changed before final version!
	int statusCode;
	get_token();
	if (token->kw != id) {
		fprintf(stderr, "Error: Expected variable id\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	Token varID = *token;
	get_token();
	if (token->kw == colon) { //Nice definition with variable type
		statusCode = data_type();
		if (statusCode != 0)
			return statusCode;
		defineSymbol(varID.s, kwToVarType(token->kw), isConst, isNullable);
		get_token();
	}

	if (token->kw != equal) {
		fprintf(stderr, "Error: Expected '=' after variable type\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	statusCode = defineSymbol(varID.s, INT, isConst, isNullable);
	if (statusCode != 0)
		return statusCode;

	statusCode = expressionParser(tokenList);

	if (statusCode != 0)
		return statusCode;

	return 0;
};
int call_or_assignment() {
	// TODO: SEMANTHIC ANALYSIS
	// I need semanthic analysis to define if id is a function or variable

	symbol_t *sym = getSymbol(token->s);
	if (sym == NULL) {
		fprintf(stderr, "Error: Variable %s has not been defined\n", token->s);
		return UNDEFINED_FUNCTION_OR_VARIABLE_ERROR;
	}
	if (sym->type == FUNCTION) {
		return function_call();
	} else {
		get_token();
		if (token->kw != equal) {
			fprintf(stderr, "Error: Expected '=' after variable id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		int statusCode = expressionParser(tokenList);
		if (statusCode != 0)
			return statusCode;
	}

	return 0;
};

int function_call() {
	while (token->kw != next) {
		if (token->kw == end) {
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		get_token();
	}
	return 0;
}

int while_syntax() {
	enterScope();
	int statusCode;
	get_token();
	if (token->kw != lbracket) {
		fprintf(stderr, "Error: Expected '(' after while\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	//Hack until precedent_an is working
	symbol_t *mainSymbol = skip_expression_get_symbol();
	get_token();
	if (token->kw == vertical_bar) {
		get_token();
		if (mainSymbol->type == VOID) {
			fprintf(stderr, "skip_expression_get_type() has not found any IDs in the expression, however, unwrapped value was still created, this should've resulted in a compile error\n ");
		}
		defineSymbol(token->s, mainSymbol->type, mainSymbol->isConst, false);
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
	exitScope();
	return 0;
}