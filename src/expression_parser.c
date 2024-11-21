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

int precedent_table_translator(int token) // Returns position of element in precedent table
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

int get_value_precedent_table(int tokenStack, int tokenInput) {
	int i = precedent_table_translator(tokenStack);
	if (i == -1) {
		fprintf(stderr, "Error: Invalid token value\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	int j = precedent_table_translator(tokenInput);
	if (j == -1) {
		fprintf(stderr, "Error: Invalid token value\n");
		return SYNTACTIC_ANALYSIS_ERROR;
	}
	return (int)precedentTable[(TABLE_SIZE * i) + j];
}

//_________________________________________________________________________
//________________________Grammar definition_______________________________
//_________________________________________________________________________

//Our grammar
/*
* 1: E->E+E
* 2: E->E*E
* 3: E->(E) yes
* 4: E->id yes
* 5: E->int yes
* 6: E->float yes
* 7: E->string yes
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


Token *tokenPrec;
List *tokenListPrec;

int get_token_prec() {
	tokenPrec = LGetAct(tokenListPrec);
	LActNext(tokenListPrec);
	return 0;
}


int expression_parser(List *tokenList) {
	int statusCode;
	tokenListPrec = tokenList;

	t_Stack stack;
	stackInit(&stack);
	statusCode = statstackPush(&stack, TERMINAL, next);
	if (statusCode != 0) {
		return statusCode;
	}
	while (1) {
		get_token_prec();
		int operation = get_value_precedent_table(topTerminal(&stack)->keyword, tokenPrec->kw);

		switch (operation) {
			case '<':
				statusCode = stackPush(&stack, TERMINAL, tokenPrec->kw);
				if (statusCode != 0) {
					return statusCode;
				}
				statusCode = stakcPushPrecedentLess(&stack);
				if (statusCode != 0) {
					return statusCode;
				}
				break;
			case '=':
				statusCode = stackPush(&stack, TERMINAL, tokenPrec->kw);
				if (statusCode != 0) {
					return statusCode;
				}
				/* code */
				break;
			case '>':
				/* code */

				break;
			case 1:
				return 0;
				break;
			case 0:
				return SYNTACTIC_ANALYSIS_ERROR;
				break;
			default:
				return SYNTACTIC_ANALYSIS_ERROR;
		}
	}
}