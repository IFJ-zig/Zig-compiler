/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/
#include "syntax_an.h"

//TODO: Semantic integration leaking like a sieve, a fix would be nice but probably not necessary for the project


Token token;
bool tokenWasGiven = 0;

int read_token() {
	token = get_token();
	if (token.kw == LEXEM) {
		fprintf(stderr, "Error: Lexical error\n");
	}
	return 0;
}

int rewind_stdin() {
	if (freopen(NULL, "r", stdin) != NULL) {
		return 0;
	} else {
		return 1;
		fprintf(stderr, "\nUnable to rewind stdin.\n");
	}
}

int syntax_analyzer() {
	int statusCode;
	statusCode = seekHeaders();

	if (statusCode != 0)
		return statusCode;
	if (rewind_stdin()) {
		return INTERNAL_COMPILER_ERROR;
	}

	statusCode = program();
	if (statusCode != 0)
		return statusCode;
	return 0;
};


int seekHeaders() {
	int statusCode = checkImport();
	if (statusCode != 0)
		return statusCode;
	read_token();
	while (token.kw != end) {

		if (token.kw != _pub) {
			fprintf(stderr, "Error: Expected function definiton starting with `pub`\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		read_token();
		if (token.kw != _fn) {
			fprintf(stderr, "Error: Expected keyword 'fn' in header\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		read_token();
		if (token.kw != id && token.kw != _main) {
			fprintf(stderr, "Error: Expected function id in header\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		char *fnName;
		if (token.kw == _main) {
			fnName = "main";
		} else {
			fnName = token.s;
		}
		//Save the name of functions into the symtable at depth 0
		if (defineSymbol(fnName, FUNCTION, false, false) == REDEFINITION_ERROR) {
			fprintf(stderr, "Error: Function %s is already defined\n", token.s);
			return REDEFINITION_ERROR;
		}
		symbol_t *fnSymbol = getSymbol(fnName);
		if (token.kw == _main) {
			fprintf(stderr, "ID: main\n");
		} else {
			fprintf(stderr, "ID: %s\n", token.s);
		}
		free(token.s);


		read_token();
		if (token.kw != lbracket) {
			fprintf(stderr, "Error: Expected '(' after function id in header\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		read_token();
		while (token.kw != rbracket) { //Process the parameters
			if (token.kw == id) {
				fprintf(stderr, "Param ID: %s\n", token.s);
				Token paramID = token;
				read_token();
				if (token.kw != colon) {
					fprintf(stderr, "Error: Expected ':' after parameter id\n");
					return SYNTACTIC_ANALYSIS_ERROR;
				}
				data_type();

				assignFunctionParameter(fnSymbol, paramID, token, false); //TODO: isNullable is hardcoded to false for now, code doesn't support optionals yet. This MUST be changed before final version!
				read_token();
				if (token.kw != comma && token.kw != rbracket) {
					fprintf(stderr, "Error: Expected ',' or ')' after parameter data type\n");
					return SYNTACTIC_ANALYSIS_ERROR;
				}
				if (token.kw == comma)
					read_token();
			} else {
				fprintf(stderr, "Error: Expected parameter id, got %d\n", comma);
				return SYNTACTIC_ANALYSIS_ERROR;
			}
		}
		fprintf(stderr, "Function %s has %d parameters\n", fnSymbol->key, fnSymbol->paramCount);
		fprintf(stderr, "Param list OK\n\n");

		if (token.kw != rbracket) {
			fprintf(stderr, "Error: Expected ')' after param list in function header\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		read_token();
		if (isValidReturnType(token.kw)) {
			fnSymbol->returnType = kwToVarType(token.kw); //Save the return type of the function
		} else {
			fprintf(stderr, "Error: Invalid return type of function %s\n", fnSymbol->key);
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		read_token();

		statusCode = skip_function_body();
		if (statusCode != 0)
			return statusCode;
		read_token();
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
		read_token();
		if (token.kw == _pub) {
			statusCode = function_analysis();
			if (statusCode != 0)
				return statusCode;
		} else if (token.kw == end) {
			fprintf(stderr, "\nCompilation successfully finished \n");
			return 0;
		} else {
			fprintf(stderr, "Error: Expected function definition or end of program, but found %d\n", token.kw);
			return 1;
		}
	}

	return 0;
};

int skip_function_body() {
	if (token.kw != lblock)
		return SYNTACTIC_ANALYSIS_ERROR;
	int braceCount = 0;
	do {
		if (token.kw == lblock) {
			++braceCount;
		} else if (token.kw == rblock) {
			--braceCount;
			if (braceCount == 0)
				break;
		}

		read_token();
	} while (braceCount != 0 && token.kw != end);

	if (braceCount == 0) {
		return 0;
	} else {
		return 1;
	}
}


int checkImport() {
	read_token();
	if (token.kw != constant) {
		fprintf(stderr, "Error: Expected keyword 'constant' as start of an import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	read_token();
	if (token.kw != inbuild) {
		fprintf(stderr, "Error: Expected identifier `ifj` for import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	read_token();
	if (token.kw != equal) {
		fprintf(stderr, "Error: Expected '=' after import identifier\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	read_token();
	if (token.kw != at) {
		fprintf(stderr, "Error: Expected '@' before import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	read_token();
	if (token.kw != _import) {
		fprintf(stderr, "Error: Expected keyword 'import' in import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	read_token();
	if (token.kw != lbracket) {

		return SYNTACTIC_ANALYSIS_ERROR;
	}
	read_token();
	if (token.kw != text && strcmp(token.s, "ifj24.zig")) {
		fprintf(stderr, "Error: Expected 'ifj24.zig' as name of the package\n");
		free(token.s);
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	free(token.s);
	read_token();
	if (token.kw != rbracket) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	read_token();
	if (token.kw != next) {
		fprintf(stderr, "Error: Expected ';'\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	return 0;
}

int function_analysis() {
	enterScope();
	int statusCode = 0;

	read_token();
	if (token.kw != _fn) {
		fprintf(stderr, "Error: Expected fn keyword\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	read_token();
	if (token.kw != id && token.kw != _main) {
		fprintf(stderr, "Error: Expected function id\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	char *fnName;
	if (token.kw == _main) {
		fnName = "main";
	} else {
		fnName = token.s;
	}

	fprintf(stderr, "Analysis ID: %s\n", fnName);

	//The function name should already be in the symtable at depth 0 since it was done in seekHeaders, lets just check if it's still there
	if (getSymbol(fnName) == NULL) {
		fprintf(stderr, "Error: Function %s is not defined, this function should've been defined in seekHeaders!\n", fnName);
		return UNDEFINED_FUNCTION_OR_VARIABLE_ERROR;
	} else {
		fprintf(stderr, "Function %s successfully found in symtable, depth=%d, return type=%s\n", fnName, getSymbol(fnName)->depth, getSymbol(fnName)->returnType == INT ? "INT" : getSymbol(fnName)->returnType == FLOAT ? "FLOAT"
																																										   : getSymbol(fnName)->returnType == STRING      ? "STRING"
																																																						  : "VOID");
	}

	statusCode = param_list(); //done semantic
	if (statusCode != 0)
		return statusCode;

	statusCode = return_type(); //semantic done in seekHeaders
	if (statusCode != 0)
		return statusCode;

	free(token.s);
	read_token();
	if (token.kw != lblock) {
		fprintf(stderr, "Error: Expected '{' after function definition\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	read_token();
	while (token.kw != rblock) {
		statusCode = code();
		if (statusCode != 0) {
			return statusCode;
		}
		if (tokenWasGiven) {
			tokenWasGiven = false;
		} else {
			read_token();
		}
	}


	// TODO: SEMANTHIC CHECKS FOR RETURN STATEMENT
	exitScope();
	return 0;
}

int param_list() {
	//Expect to already be in the function scope
	int statusCode;
	read_token();
	if (token.kw != lbracket) {
		fprintf(stderr, "Error: Expected parameter list after function id\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	while (token.kw != rbracket) {
		fprintf(stderr, "PARAM LIST\n");
		read_token();
		if (token.kw == rbracket) {
			break;
		}
		if (token.kw != id) {
			fprintf(stderr, "Error: Expected parameter id %d\n", token.kw);
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		Token paramID = token;
		read_token();
		if (token.kw != colon) {
			fprintf(stderr, "Error: Expected ':' after parameter id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}

		statusCode = data_type();
		if (statusCode != 0)
			return statusCode;
		processParam(paramID, token, false); //TODO: isNullable is hardcoded to false for now, code doesn't support optionals yet. This MUST be changed before final version!
		fprintf(stderr, "Param %s of type %s loaded into symtable at depth %d\n", paramID.s, token.kw == dtint ? "INT" : token.kw == dtflt ? "FLOAT"
																																		   : "STRING",
				getSymbol(paramID.s)->depth);
		read_token();
		if (token.kw != comma && token.kw != rbracket) {
			fprintf(stderr, "Error: Expected ',' or ')' after parameter data type\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
	}

	return 0;
}

int data_type() {
	read_token();
	if (token.kw == square_brackets) {
		read_token();
	}
	if (token.kw != dtint && token.kw != dtstr && token.kw != dtflt) {
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
	read_token();
	if (!isValidReturnType(token.kw)) {
		fprintf(stderr, "Error: Expected return type\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	return 0;
}

int code() {
	if (!tokenWasGiven) {
		tokenWasGiven = false;
	} else {
		read_token();
	}

	int statusCode;
	switch (token.kw) {
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
			statusCode = call_or_assignment(); //TODO: assignment done, call is missing
			if (statusCode != 0)
				return statusCode;
			break;
		case _if:
			statusCode = if_else(); //semantic done,
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
			fprintf(stderr, "Error: Expected command but found: %d\n", token.kw);
			return 1;
	}

	return 0;
}

int if_else() {
	int statusCode;
	enterScope();

	statusCode = expressionParser();

	if (statusCode != 0)
		return statusCode;
	if (token.kw == vertical_bar) {
		read_token();
		if (token.kw != id) {
			fprintf(stderr, "Error: Expected ID after unwrapped value\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}

		defineSymbol(token.s, INT, false, false);
		read_token();
		if (token.kw != vertical_bar) {
			fprintf(stderr, "Error: Expected '|' after unwrapped value id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		read_token();
	}

	if (token.kw != lblock) {
		fprintf(stderr, "Error: Expected '{' after if condition\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	read_token();

	while (token.kw != rblock) {
		statusCode = code();
		if (statusCode != 0)
			return statusCode;
		read_token();
	}
	exitScope(); //Exit the scope of the if statement has to be here since we can't have the unwrapped value reach the else block
	read_token();
	if (token.kw == _else) {
		enterScope();
		read_token();
		if (token.kw != lblock) {
			fprintf(stderr, "Error: Expected '{' after else\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		read_token();
		while (token.kw != rblock) {
			statusCode = code();
			if (statusCode != 0)
				return statusCode;
			read_token();
		}
		exitScope();
	} else {
		tokenWasGiven = true;
	}
	return 0;
}

int skip_expression() {
	int statusCode;
	while (token.kw != rbracket) {
		read_token();
		if (token.kw == lbracket) {
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
	while (token.kw != rbracket) {
		read_token();
		if (token.kw == lbracket) {
			statusCode = skip_expression();
			if (statusCode != 0)
				return 0;
		}
		if (token.kw == id) {
			symbol_t *symbol = getSymbol(token.s);
			if (symbol == NULL) {
				fprintf(stderr, "Error: Variable %s has not been defined\n", token.s);
			} else
				foundSymbol = symbol;
		}
	}
	return foundSymbol;
}

int return_syntax() {
	// TODO: SEMANTHIC CHECKS, BOTTOM UP EXPRESSION ANALYSIS AND CODE GENERATION

	//skip placeholder
	while (token.kw != next && token.kw != end && token.kw != LEXEM) {
		read_token();
	}
	return 0;
}

int inbuild_function() {
	int statusCode;
	read_token();
	if (token.kw != dot) {
		fprintf(stderr, "Error: Unexpected library usage\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	read_token();
	if (token.kw != id) {
		fprintf(stderr, "Error: Expected library call\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	statusCode = function_call(true);
	if (statusCode != 0)
		return statusCode;

	return 0;
}

int variable_definition(bool isConst) {
	bool isNullable = false; //TODO: isNullable is hardcoded to false for now, code doesn't support optionals yet. This MUST be changed before final version!
	int statusCode;
	read_token();
	if (token.kw != id) {
		fprintf(stderr, "Error: Expected variable id\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	Token varID = token;
	read_token();
	if (token.kw == colon) { //Nice definition with variable type
		statusCode = data_type();
		if (statusCode != 0)
			return statusCode;
		defineSymbol(varID.s, kwToVarType(token.kw), isConst, isNullable);
		read_token();
	}

	if (token.kw != equal) {
		fprintf(stderr, "Error: Expected '=' after variable type\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	statusCode = defineSymbol(varID.s, INT, isConst, isNullable);
	if (statusCode != 0)
		return statusCode;

	statusCode = expressionParser();

	if (statusCode != 0)
		return statusCode;

	return 0;
};
int call_or_assignment() {
	// TODO: SEMANTHIC ANALYSIS
	// I need semanthic analysis to define if id is a function or variable

	symbol_t *sym = getSymbol(token.s);
	if (sym == NULL) {
		fprintf(stderr, "Error: Variable %s has not been defined\n", token.s);
		return UNDEFINED_FUNCTION_OR_VARIABLE_ERROR;
	}
	if (sym->type == FUNCTION) {
		return function_call(true);
	} else {
		read_token();
		if (token.kw != equal) {
			fprintf(stderr, "Error: Expected '=' after variable id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		int statusCode = expressionParser();
		if (statusCode != 0)
			return statusCode;
	}

	return 0;
};

int function_call(bool expectNext) {
	int unclosedBrackets = 0;
	while (token.kw != end) {
		if (token.kw == lbracket) {
			unclosedBrackets++;
		} else if (token.kw == rbracket) {
			unclosedBrackets--;
			if (unclosedBrackets == 0) {
				if (expectNext) {
					read_token();
					if (token.kw != next) {
						fprintf(stderr, "Error: Expected ';' after function call\n");
						return SYNTACTIC_ANALYSIS_ERROR;
					}
				}
				return 0;
			}
		}
		read_token();
	}
	return SYNTACTIC_ANALYSIS_ERROR;
}

int while_syntax() {
	int statusCode;
	enterScope();

	statusCode = expressionParser();

	if (statusCode != 0)
		return statusCode;
	if (token.kw == vertical_bar) {
		read_token();
		if (token.kw != id) {
			fprintf(stderr, "Error: Expected ID after unwrapped value\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}

		defineSymbol(token.s, INT, false, false);
		read_token();
		if (token.kw != vertical_bar) {
			fprintf(stderr, "Error: Expected '|' after unwrapped value id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		read_token();
	}
	if (token.kw != lblock) {
		fprintf(stderr, "Error: Expected '{' after while condition\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	read_token();
	while (token.kw != rblock) {
		statusCode = code();
		if (statusCode != 0)
			return statusCode;
		read_token();
	}
	exitScope();
	return 0;
}