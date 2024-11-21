/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Adam Vožda, xvozdaa00
*********************************************/

#include "expression_parser.h"


char precedentTable[TABLE_SIZE * TABLE_SIZE + 1] = {
		//  +     -     *     /     (     )     i     <     >     ==    <=    >=    !=    ;, x
		/* + */ '>', '>', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<',
		/* - */ '<', '>', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<',
		/* * */ '>', '>', '>', '>', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<',
		/* / */ '>', '>', '<', '>', '<', '>', '<', '>', '>', '>', '>', '>', '>', '>', '<',
		/* ( */ '<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', 0, '<',
		/* ) */ '>', '>', '>', '>', 0, '>', 0, '>', '>', '>', '>', '>', '>', '>', 0,
		/* i */ '>', '>', '>', '>', 0, '>', 0, '>', '>', '>', '>', '>', '>', '>', 0,
		/* < */ '<', '<', '<', '<', '<', '>', '<', 0, 0, 0, 0, 0, 0, '>', '<',
		/* > */ '<', '<', '<', '<', '<', '>', '<', 0, 0, 0, 0, 0, 0, '>', '<',
		/*== */ '<', '<', '<', '<', '<', '>', '<', 0, 0, 0, 0, 0, 0, '>', '<',
		/*!= */ '<', '<', '<', '<', '<', '>', '<', 0, 0, 0, 0, 0, 0, '>', '<',
		/*<= */ '<', '<', '<', '<', '<', '>', '<', 0, 0, 0, 0, 0, 0, '>', '<',
		/*>= */ '<', '<', '<', '<', '<', '>', '<', 0, 0, 0, 0, 0, 0, '>', '<',
		/*;*/ '<', '<', '<', '<', '<', 0, '<', '<', '<', '<', '<', '<', '<', 1, '<',
		/*x*/ '>', '>', '>', '>', 0, '>', 0, '>', '>', '>', '>', '>', '>', '>', 0};

int precedentTableTranslator(int token) // Returns position of element in precedent table
{
	switch (token) //  +     -     *     /     (     )     i     <     >     ==    <=    >=    !=    ;/,
	{
		case plus:
			return 0;
		case minus:
			return 1;
		case multiply:
			return 2;
		case division:
			return 3;
		case lbracket:
			return 4;
		case rbracket:
			return 5;
		case id:
			return 6;
		case less:
			return 7;
		case more:
			return 8;
		case equal:
			return 9;
		case lequal:
			return 10;
		case mequal:
			return 11;
		case nequal:
			return 12;
		case next:
			return 13;
		case comma:
			return 13;
		case lblock:
			return 13;
		case num:
			return 14;
		case decim:
			return 14;
		case text:
			return 14;
		default:
			fprintf(stderr, "\nInvalid input token value\n");
			return -1;
	}
}

int getOperation(int tokenStack, int tokenInput) {
	int i = precedentTableTranslator(tokenStack);
	if (i == -1) {
		fprintf(stderr, "Error: Invalid token value\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	int j = precedentTableTranslator(tokenInput);
	if (j == -1) {
		fprintf(stderr, "Error: Invalid token value\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	return (int)precedentTable[(TABLE_SIZE * i) + j];
}

Token *tokenPrec;
List *tokenListPrec;

int getTokenPrec() {
	tokenPrec = LGetAct(tokenListPrec);
	LActNext(tokenListPrec);
	return 0;
}


int expressionParser(List *tokenList) {
	int statusCode;
	tokenListPrec = tokenList;

	t_Stack stack;
	stackInit(&stack);
	statusCode = stackPush(&stack, TERMINAL, next);
	if (statusCode != 0) {
		return statusCode;
	}
	getTokenPrec();
	while (1) {
		printStack(&stack);
		int operation = getOperation(topTerminal(&stack)->keyword, tokenPrec->kw);
		switch (operation) {
			case '<':
				statusCode = stackPushPrecedentLess(&stack);

				if (statusCode != 0) {
					stackClear(&stack);
					return statusCode;
				}
				statusCode = stackPush(&stack, TERMINAL, tokenPrec->kw);
				if (statusCode != 0) {
					stackClear(&stack);
					return statusCode;
				}
				getTokenPrec();
				break;
			case '=':
				statusCode = stackPush(&stack, TERMINAL, tokenPrec->kw);
				if (statusCode != 0) {
					stackClear(&stack);
					return statusCode;
				}
				getTokenPrec();
				/* code */
				break;
			case '>':
				/* code */
				statusCode = tryToMatchRule(&stack);
				if (statusCode != 0) {
					stackClear(&stack);
					return statusCode;
				}
				break;
			case 1:
				stackClear(&stack);
				return 0;
				break;
			case 0:
				stackClear(&stack);
				return SYNTACTIC_ANALYSIS_ERROR;
				break;
			default:
				stackClear(&stack);
				return SYNTACTIC_ANALYSIS_ERROR;
		}
	}
}


//Rules
/*
* 1: E->E+E
* 2: E->E*E
* 3: E->(E)
* 4: E->id
* 5: E->num
* 6: E->float
* 7: E->string
* 8: E->E-E
* 9: E->E/E
* 10: E->E>E
* 11: E->E<E
* 12: E->E!=E
* 13: E->E>=E
* 14: E->E<=E
* 15: E->E==E
* 15: E->-E
*/
int tryToMatchRule(t_Stack *stack) {
	t_StackItem *temp = stackTop(stack);

	if (temp == NULL) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	//Match rules 3-7
	if (temp->type == TERMINAL) {
		if (temp->keyword == id || temp->keyword == num || temp->keyword == decim || temp->keyword == text) {
			stackPop(stack);
			temp = stackTop(stack);
			if (temp == NULL) {
				return SYNTACTIC_ANALYSIS_ERROR;
			}

			if (temp->type == PRECEDENT_LESS) {
				stackPop(stack);
				stackPush(stack, NON_TERMINAL, EMPTY);
				return 0;
			}
		} else if (temp->keyword == rbracket) {
			stackPop(stack);
			temp = stackTop(stack);
			if (temp == NULL) {
				return SYNTACTIC_ANALYSIS_ERROR;
			}
			if (temp->type == NON_TERMINAL) {
				stackPop(stack);
				temp = stackTop(stack);
				if (temp == NULL) {
					return SYNTACTIC_ANALYSIS_ERROR;
				}
				if (temp->keyword == lbracket) {
					stackPop(stack);
					temp = stackTop(stack);
					if (temp == NULL) {
						return SYNTACTIC_ANALYSIS_ERROR;
					}
					if (temp->type == PRECEDENT_LESS) {
						stackPop(stack);
						stackPush(stack, NON_TERMINAL, EMPTY);
						return 0;
					}
				}
			}
		}
	} // Math rest of rules
	else if (temp->type == NON_TERMINAL) {
		stackPop(stack);
		temp = stackTop(stack);

		switch (temp->keyword) {
			case plus:
			case multiply:
			case division:
			case less:
			case more:
			case compare_equal:
			case nequal:
			case lequal:
			case mequal:
				stackPop(stack);
				temp = stackTop(stack);
				if (temp == NULL) {
					return SYNTACTIC_ANALYSIS_ERROR;
				}
				if (temp->type == NON_TERMINAL) {
					stackPop(stack);
					temp = stackTop(stack);
					if (temp == NULL) {
						return SYNTACTIC_ANALYSIS_ERROR;
					}
					if (temp->type == PRECEDENT_LESS) {
						stackPop(stack);
						stackPush(stack, NON_TERMINAL, EMPTY);
						return 0;
					}
				}
				break;
			case minus:
				temp = stackTop(stack);
				if (temp == NULL) {
					return SYNTACTIC_ANALYSIS_ERROR;
				}
				if (temp->type == PRECEDENT_LESS) {
					stackPop(stack);
					stackPush(stack, NON_TERMINAL, EMPTY);
					return 0;
				} else if (temp->type == NON_TERMINAL) {
					temp = stackTop(stack);
					if (temp == NULL) {
						return SYNTACTIC_ANALYSIS_ERROR;
					}
					if (temp->type == PRECEDENT_LESS) {
						stackPop(stack);
						stackPush(stack, NON_TERMINAL, EMPTY);
						return 0;
					}
				}
				break;
			default:
				return SYNTACTIC_ANALYSIS_ERROR;
				break;
		}
	}
	return SYNTACTIC_ANALYSIS_ERROR;
}