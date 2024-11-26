/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/
#include "syntax_an.h"


Token token;
bool tokenWasGiven = 0;

int read_token() {
	token = get_token();
	if (token.kw == LEXEM) {
		fprintf(stderr, "Error: Lexical error\n");
		return LEXEM_ERROR;
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
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	while (token.kw != end) {

		if (token.kw != _pub) {
			fprintf(stderr, "Error: Expected function definiton starting with `pub`\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
		if (token.kw != _fn) {
			fprintf(stderr, "Error: Expected keyword 'fn' in header\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
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

		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
		if (token.kw != lbracket) {
			fprintf(stderr, "Error: Expected '(' after function id in header, got %s\n", getTokenName(token));
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
		while (token.kw != rbracket) { //Process the parameters
			if (token.kw == id) {
				fprintf(stderr, "Param ID: %s\n", token.s);
				Token paramID = token;
				statusCode = read_token();
				if (statusCode != 0) {
					return statusCode;
				}
				if (token.kw != colon) {
					fprintf(stderr, "Error: Expected ':' after parameter id, got %s\n", getTokenName(token));
					return SYNTACTIC_ANALYSIS_ERROR;
				}
				data_type();

				assignFunctionParameter(fnSymbol, paramID, token, false); //TODO: isNullable is hardcoded to false for now, code doesn't support optionals yet. This MUST be changed before final version!
				statusCode = read_token();
				if (statusCode != 0) {
					return statusCode;
				}
				if (token.kw != comma && token.kw != rbracket) {
					fprintf(stderr, "Error: Expected ',' or ')' after parameter data type, got %s\n", getTokenName(token));
					return SYNTACTIC_ANALYSIS_ERROR;
				}
				if (token.kw == comma)
					statusCode = read_token();
				if (statusCode != 0) {
					return statusCode;
				}
			} else {
				fprintf(stderr, "Error: Expected parameter id, got %s\n", getTokenName(token));
				return SYNTACTIC_ANALYSIS_ERROR;
			}
		}
		fprintf(stderr, "Function %s has %d parameters\n", fnSymbol->key, fnSymbol->paramCount);
		fprintf(stderr, "Param list OK\n\n");

		if (token.kw != rbracket) {
			fprintf(stderr, "Error: Expected ')' after param list in function header, got %s\n", getTokenName(token));
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
		if (isValidReturnType(token.kw)) {
			fnSymbol->returnType = kwToVarType(token.kw); //Save the return type of the function
		} else {
			fprintf(stderr, "Error: Invalid return type of function %s\n", fnSymbol->key);
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}

		statusCode = skip_function_body();
		if (statusCode != 0)
			return statusCode;
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
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
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
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

		int statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
	} while (braceCount != 0 && token.kw != end);

	if (braceCount == 0) {
		return 0;
	} else {
		return 1;
	}
}


int checkImport() {
	static bool imported = false;
	int statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != constant) {
		fprintf(stderr, "Error: Expected keyword 'constant' as start of an import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != inbuild) {
		fprintf(stderr, "Error: Expected identifier `ifj` for import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != equal) {
		fprintf(stderr, "Error: Expected '=' after import identifier\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != at) {
		fprintf(stderr, "Error: Expected '@' before import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != _import) {
		fprintf(stderr, "Error: Expected keyword 'import' in import\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != lbracket) {

		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != text && strcmp(token.s, "ifj24.zig")) {
		fprintf(stderr, "Error: Expected 'ifj24.zig' as name of the package\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != rbracket) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != next) {
		fprintf(stderr, "Error: Expected ';'\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	if (!imported) {
		loadIFJ24();
		imported = true;
	}
	return 0;
}

void loadIFJ24() {
	fprintf(stderr, "Loading IFJ24 functions\n");
	symbol_t *fnSymbol;
	defineSymbol("ifj.write", FUNCTION, false, false);
	fnSymbol = getSymbol("ifj.write");
	assignFunctionParameter(fnSymbol, (Token){.s = "value", .kw = id}, (Token){.kw = dtflt}, false);
	fnSymbol->params[0]->type = ANYTYPE; //Anytype is allowed, we don't have a keyword for that however, so a little hack because, because this is happens only once in the entire program

	defineSymbol("ifj.readstr", FUNCTION, false, true);
	fnSymbol = getSymbol("ifj.readstr");
	fnSymbol->returnType = STRING;

	defineSymbol("ifj.readi32", FUNCTION, false, true);
	fnSymbol = getSymbol("ifj.readi32");
	fnSymbol->returnType = INT;

	defineSymbol("ifj.readf64", FUNCTION, false, true);
	fnSymbol = getSymbol("ifj.readf64");
	fnSymbol->returnType = FLOAT;

	defineSymbol("ifj.string", FUNCTION, false, false);
	fnSymbol = getSymbol("ifj.string");
	assignFunctionParameter(fnSymbol, (Token){.s = "s", .kw = id}, (Token){.kw = dtstr}, false);
	fnSymbol->returnType = STRING;

	defineSymbol("ifj.concat", FUNCTION, false, false);
	fnSymbol = getSymbol("ifj.concat");
	assignFunctionParameter(fnSymbol, (Token){.s = "str1", .kw = id}, (Token){.kw = dtstr}, false);
	assignFunctionParameter(fnSymbol, (Token){.s = "str2", .kw = id}, (Token){.kw = dtstr}, false);
	fnSymbol->returnType = STRING;

	defineSymbol("ifj.length", FUNCTION, false, false);
	fnSymbol = getSymbol("ifj.length");
	assignFunctionParameter(fnSymbol, (Token){.s = "s", .kw = id}, (Token){.kw = dtstr}, false);
	fnSymbol->returnType = INT;

	defineSymbol("ifj.i2f", FUNCTION, false, false);
	fnSymbol = getSymbol("ifj.i2f");
	assignFunctionParameter(fnSymbol, (Token){.s = "i", .kw = id}, (Token){.kw = dtint}, false);
	fnSymbol->returnType = FLOAT;

	defineSymbol("ifj.f2i", FUNCTION, false, false);
	fnSymbol = getSymbol("ifj.f2i");
	assignFunctionParameter(fnSymbol, (Token){.s = "i", .kw = id}, (Token){.kw = dtflt}, false);
	fnSymbol->returnType = INT;

	defineSymbol("ifj.substring", FUNCTION, false, true);
	fnSymbol = getSymbol("ifj.substring");
	assignFunctionParameter(fnSymbol, (Token){.s = "s", .kw = id}, (Token){.kw = dtstr}, false);
	assignFunctionParameter(fnSymbol, (Token){.s = "i", .kw = id}, (Token){.kw = dtint}, false);
	assignFunctionParameter(fnSymbol, (Token){.s = "j", .kw = id}, (Token){.kw = dtint}, false);
	fnSymbol->returnType = STRING;

	defineSymbol("ifj.ord", FUNCTION, false, false);
	fnSymbol = getSymbol("ifj.ord");
	assignFunctionParameter(fnSymbol, (Token){.s = "s", .kw = id}, (Token){.kw = dtstr}, false);
	assignFunctionParameter(fnSymbol, (Token){.s = "i", .kw = id}, (Token){.kw = dtint}, false);
	fnSymbol->returnType = INT;

	defineSymbol("ifj.chr", FUNCTION, false, false);
	fnSymbol = getSymbol("ifj.chr");
	assignFunctionParameter(fnSymbol, (Token){.s = "i", .kw = id}, (Token){.kw = dtint}, false);
	fnSymbol->returnType = STRING;

	defineSymbol("ifj.strcmp", FUNCTION, false, false);
	fnSymbol = getSymbol("ifj.strcmp");
	assignFunctionParameter(fnSymbol, (Token){.s = "s1", .kw = id}, (Token){.kw = dtstr}, false);
	assignFunctionParameter(fnSymbol, (Token){.s = "s2", .kw = id}, (Token){.kw = dtstr}, false);
	fnSymbol->returnType = INT;
	fprintf(stderr, "Done loading IFJ24 functions\n\n");
}

int function_analysis() {
	enterScope();
	int statusCode = 0;

	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != _fn) {
		fprintf(stderr, "Error: Expected fn keyword\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
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

	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != lblock) {
		fprintf(stderr, "Error: Expected '{' after function definition\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	while (token.kw != rblock) {
		statusCode = code();
		if (statusCode != 0) {
			return statusCode;
		}
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
	}


	// TODO: SEMANTHIC CHECKS FOR RETURN STATEMENT
	exitScope();
	return 0;
}

int param_list() {
	//Expect to already be in the function scope
	int statusCode;
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != lbracket) {
		fprintf(stderr, "Error: Expected parameter list after function id\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	while (token.kw != rbracket) {
		fprintf(stderr, "PARAM LIST\n");
		statusCode = read_token();
		if (statusCode != 0) {
			free(token.s);
			return statusCode;
		}
		if (token.kw == rbracket) {
			break;
		}
		if (token.kw != id) {
			fprintf(stderr, "Error: Expected parameter id %d\n", token.kw);
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		Token paramID = token;
		statusCode = read_token();
		if (statusCode != 0) {
			free(token.s);
			return statusCode;
		}
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
		statusCode = read_token();
		if (statusCode != 0) {
			free(token.s);
			return statusCode;
		}
		if (token.kw != comma && token.kw != rbracket) {
			fprintf(stderr, "Error: Expected ',' or ')' after parameter data type\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
	}

	return 0;
}

int data_type() {
	int statusCode = read_token();
	if (statusCode != 0) {
		free(token.s);
		return statusCode;
	}
	if (token.kw == question_mark) {
		statusCode = read_token();
		if (statusCode != 0) {
			free(token.s);
			return statusCode;
		}
	}
	if (token.kw == square_brackets) {
		statusCode = read_token();
		if (statusCode != 0) {
			free(token.s);
			return statusCode;
		}
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
	int statusCode = read_token();
	if (statusCode != 0) {
		free(token.s);
		return statusCode;
	}
	if (!isValidReturnType(token.kw)) {
		fprintf(stderr, "Error: Expected return type\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	return 0;
}

int code() {
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

	statusCode = expressionParser(false);

	if (statusCode != 0) {
		return statusCode;
	}

	if (token.kw == vertical_bar) {
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}


		if (token.kw != id) {
			fprintf(stderr, "Error: Expected ID after unwrapped value\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}

		defineSymbol(token.s, INT, false, false);
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
		if (token.kw != vertical_bar) {
			fprintf(stderr, "Error: Expected '|' after unwrapped value id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
	}

	if (token.kw != lblock) {
		fprintf(stderr, "Error: Expected '{' after if condition\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}

	while (token.kw != rblock) {
		statusCode = code();
		if (statusCode != 0)
			return statusCode;
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
	}
	exitScope(); //Exit the scope of the if statement has to be here since we can't have the unwrapped value reach the else block
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != _else) {
		fprintf(stderr, "Error: Expected 'else' after if block\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	enterScope();
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != lblock) {
		fprintf(stderr, "Error: Expected '{' after else\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}

	while (token.kw != rblock) {
		statusCode = code();
		if (statusCode != 0)
			return statusCode;
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
	}
	exitScope();

	return 0;
}

int return_syntax() {
	int statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw == next) {
		return 0;
	}
	statusCode = expressionParser(true);
	if (statusCode != 0)
		return statusCode;
	return 0;
}

int inbuild_function() {
	int statusCode;
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != dot) {
		fprintf(stderr, "Error: Unexpected library usage\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
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
	int statusCode = 0;
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != id) {
		fprintf(stderr, "Error: Expected variable id\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	Token varID = token;
	fprintf(stderr, "Variable ID: %s\n", varID.s);
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	bool isDefined = false;
	if (token.kw == colon) { //Nice definition with variable type
		isDefined = true;
		statusCode = data_type();
		if (statusCode != 0)
			return statusCode;
		fprintf(stderr, "trying to define symbol %s\n", varID.s);
		defineSymbol(varID.s, kwToVarType(token.kw), isConst, isNullable);
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
	}

	if (token.kw != equal) {
		fprintf(stderr, "Error: Expected '=' after variable type\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	if (!isDefined)
		statusCode = defineSymbol(varID.s, INT, isConst, isNullable);
	if (statusCode != 0)
		return statusCode;

	statusCode = expressionParser(false);

	if (statusCode != 0)
		return statusCode;

	return 0;
};
int call_or_assignment() {
	int statusCode;
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
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
		if (token.kw != equal) {
			fprintf(stderr, "Error: Expected '=' after variable id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		int statusCode = expressionParser(false);
		if (statusCode != 0)
			return statusCode;
	}

	return 0;
};

bool isLiteralOrId(Token t) {
	if (t.kw == text || t.kw == decim || t.kw == num || t.kw == id)
		return true;
	return false;
}

int function_call(bool expectNext) {
	int statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}
	if (token.kw != lbracket) {
		fprintf(stderr, "Error: Expected '(' after function call\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	bool isPastFirstTerm = false;
	do {
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
		if (!isPastFirstTerm) {
			if (token.kw == comma) {
				fprintf(stderr, "Error: Unexpected ',' at the beginning of function call\n");
				return SYNTACTIC_ANALYSIS_ERROR;
			}
		}
		if (token.kw == rbracket) {
			break;
		}
		if (!isLiteralOrId(token)) {
			fprintf(stderr, "Error: Expected literal or id in function call\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		isPastFirstTerm = true;
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
	} while (token.kw == comma);

	if (token.kw != rbracket) {
		fprintf(stderr, "Error: Expected ')' after function call\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	if (expectNext) {
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
		if (token.kw != next) {
			fprintf(stderr, "Error: Expected ';' after function call\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
	}

	return 0;
}

int while_syntax() {
	int statusCode;
	enterScope();

	statusCode = expressionParser(false);

	if (statusCode != 0)
		return statusCode;
	if (token.kw == vertical_bar) {
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
		if (token.kw != id) {
			fprintf(stderr, "Error: Expected ID after unwrapped value\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}

		defineSymbol(token.s, INT, false, false);
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
		if (token.kw != vertical_bar) {
			fprintf(stderr, "Error: Expected '|' after unwrapped value id\n");
			return SYNTACTIC_ANALYSIS_ERROR;
		}
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
	}
	if (token.kw != lblock) {
		fprintf(stderr, "Error: Expected '{' after while condition\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	statusCode = read_token();
	if (statusCode != 0) {
		return statusCode;
	}

	while (token.kw != rblock) {
		statusCode = code();
		if (statusCode != 0)
			return statusCode;
		statusCode = read_token();
		if (statusCode != 0) {
			return statusCode;
		}
	}
	exitScope();
	return 0;
}