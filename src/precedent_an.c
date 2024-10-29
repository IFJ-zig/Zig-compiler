#include "precedent_an.h"

typedef enum
{
	FLOAT64,
	INT,
	STRING,
	FUNC,
	ILLEGAL,
	BOOL
} var_type_t;


static char GV_precedentTable[TABLE_SIZE * TABLE_SIZE + 1] = {
		//  +     -     *     /     (     )     i     <     >     ==    <=    >=    !=    ;/, num
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
		/*;*/ '<', '<', '<', '<', '<', 0, '<', '<', '<', '<', '<', '<', '<', 0, '<',
		/*num*/ '>', '>', '>', '>', 0, '>', 0, '>', '>', '>', '>', '>', '>', '>', 0};

Token *token;
List *tokenList;

int get_token_prec() {
	token = LGetAct(tokenList);
	LActNext(tokenList);
	return 0;
}
//_________________________________________________________________________
//________________________Stack functions__________________________________
//_________________________________________________________________________
void stackInit(t_Stack *s) // Init stack
{
	s->first = NULL;
}

t_StackItem topStack(t_Stack *s) // Returns top element from stack. If stack is empty, creates empty element
{
	if (s->first == NULL) {
		t_StackItem emptyElement;
		emptyElement.data = EMPTY;
		emptyElement.attributes = NULL;
		emptyElement.dataType = EMPTY;
		emptyElement.next = NULL;
		return emptyElement;
	}
	return *s->first;
}

t_StackItem *pointerToTop(t_Stack *s) {
	return s->first;
}

int topTerminal(t_Stack *s) // Returns type of first terminal in stack. If there isn't any, returns -1
{
	if (s->first == NULL) {
		return -1;
	}
	t_StackItem *pom = s->first;
	while (pom != NULL && (pom->data == nonterminal_token || pom->data == precedent_less_token)) {
		pom = pom->next;
	}
	if (pom != NULL) {
		return pom->data;
	} else {
		return -1;
	}
}

void freeItAll(t_StackItem *freeman) // Free attributes from stack element. Important for no memory leaks
{
	if (freeman->attributes != NULL) {
		if (freeman->attributes->s != NULL) {
			free(freeman->attributes->s);
			freeman->attributes->s = NULL;
		}
		free(freeman->attributes);
		freeman->attributes = NULL;
	}
}
t_StackItem popStack(t_Stack *s) // Pops element from stack. WARNING - call freeItAll on popped item for no memleaks
{
	if (s->first == NULL) {
		t_StackItem pom;
		pom.data = error_token;
		pom.attributes = NULL;
		pom.dataType = illegal_token;
		pom.next = NULL;
		return pom;
	}
	t_StackItem *pom;
	t_StackItem returnor;
	pom = s->first;
	s->first = s->first->next;
	returnor.dataType = pom->dataType;
	returnor.data = pom->data;
	returnor.attributes = pom->attributes;
	returnor.next = NULL;
	returnor.ranking = pom->ranking;
	returnor.stillCons = pom->stillCons;
	free(pom);
	pom = NULL;
	return returnor;
}

Token *createAttributes(Token *attributes) // Creates stack element attributes. Dont call function if attributes is NULL
{
	Token *newItem = malloc(sizeof(Token));
	if (newItem == NULL) {
		return NULL;
	}

	newItem->f = attributes->f;
	newItem->sCurrentIndex = attributes->sCurrentIndex;
	newItem->sSize = attributes->sSize;
	newItem->i = attributes->i;
	//int n = 100;
	/* 	if (attributes->s != NULL) {
		newItem->s = malloc(n * sizeof(htab_key_t));
		if (newItem->s == NULL) {
			free(newItem);
			return NULL;
		}
		strcpy(newItem->s, attributes->s);
	} else {
		newItem->s = NULL;
	} */
	return newItem;
}

int pushStack(t_Stack *s, int data, int dataType, Token *attributes, bool isConst, int ranking) // Pushes element to stack, allocates new attribute for it. Don't forget to avoid memleaks.
{
	t_StackItem *newItem = malloc(sizeof(t_StackItem));
	if (newItem == NULL) {
		return -1;
	}
	newItem->ranking = ranking;
	newItem->next = s->first;
	newItem->data = data;
	newItem->dataType = dataType;
	newItem->stillCons = isConst;
	s->first = newItem;
	if (attributes == NULL) {
		newItem->attributes = NULL;
		return 0;
	}
	newItem->attributes = createAttributes(attributes); // Create attributes for stack element - IMPORTANT - call freeItAll after pop, or memory leaks occur
	if (newItem->attributes == NULL) {
		return -1;
	}

	return 0;
}

int pushAfterTopTerminal(t_Stack *s) // Pushes < after first terminal in stack
{
	if (s->first == NULL) {
		return -1;
	}
	t_StackItem *pom = s->first;
	if (pom->data != nonterminal_token && pom->data != precedent_less_token) {
		return pushStack(s, precedent_less_token, illegal_token, NULL, true, 0);
	}

	while (pom->next != NULL && (pom->next->data == nonterminal_token || pom->next->data == precedent_less_token)) {
		pom = pom->next;
	}
	if (pom->next != NULL) // Terminal found, allocate new element.
	{
		t_StackItem *newItem = malloc(sizeof(struct StackItem));
		if (newItem == NULL) {
			return -1;
		}
		newItem->next = pom->next;
		newItem->data = precedent_less_token;
		newItem->dataType = illegal_token;
		newItem->attributes = NULL;
		pom->next = newItem;
		return 0;
	} else {
		return -1;
	}
}

bool isInStack(t_Stack *s, int data) // Finds wheter type of element is in stack or not.
{
	if (s->first == NULL) {
		return false;
	}
	t_StackItem *pom = s->first;
	while (pom != NULL && pom->data != data) {
		pom = pom->next;
	}
	return pom != NULL;
}

void deleteStack(t_Stack *s) // Deletes stack - only option where you dont need to call freeItAll separatedly. Still, dont forget call freeItAll on your popStacked elements.
{
	t_StackItem pom;
	while ((pom = popStack(s)).data != error_token) {
		freeItAll(&pom);
	}
	s->first = NULL;
}

void printStack(t_Stack *s) // Prints stack
{
	t_StackItem *stackElement = s->first;
	while (stackElement != NULL) {
		fprintf(stderr, "%d\n", stackElement->data);
		stackElement = stackElement->next;
	}
	fprintf(stderr, "DNO\n");
}


//_________________________________________________________________________
//________________________Precedent table functions________________________
//_________________________________________________________________________

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
		case colon:
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
	return 0;
}

int getValuePrecedentTable(int tokenStack, int tokenInput) // Translates your token values to >, <, = or 0.
{
	int i = precedentTableTranslator(tokenStack);
	if (i == -1) // Token is not in stack
	{
		return -1;
	}
	int j = precedentTableTranslator(tokenInput);
	if (j == -1) {
		return -1;
	}
	return (int)GV_precedentTable[(TABLE_SIZE * i) + j];
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

float decimalOperation(double data1, float data2, int operation) // TODO - generate code
{
	switch (operation) {
		case plus:
			return data1 + data2;
		case minus:
			return data1 - data2;
		case multiply:
			return data1 * data2;
		case division:
			return data1 / data2;
		default:
			fprintf(stderr, "I cannot count this!");
			return 0;
	}
}

bool isEndingTerminal(int token) {
	return token == next || token == comma || token == lblock || token == colon;
}

t_StackItem emptyItem() {
	t_StackItem empty;
	empty.attributes = NULL;
	empty.data = 1000;
	empty.dataType = illegal_token;
	empty.next = NULL;
	return empty;
}


int precedentAnalysis(var_type_t expected, var_type_t *returned, Token *_token, List *_tokenList) {
	token = _token;
	tokenList = _tokenList;
	int comp = 0;
	t_Stack s;
	bool success = true; // Input is gramatically correct
	t_StackItem topFromStack = emptyItem();
	bool correctOperation;
	stackInit(&s);
	t_StackItem top = emptyItem();
	t_StackItem pom = emptyItem();
	t_StackItem deletor = emptyItem();

	pushStack(&s, next, illegal_token, NULL, true, 0);

	do {
		freeItAll(&top);
		freeItAll(&topFromStack);

		top.data = topTerminal(&s);
		int tableValue = getValuePrecedentTable(top.data, token->kw);
		switch (tableValue) {
			case '=':
				if (pushStack(&s, token->kw, illegal_token, token, true, 0) == -1) // Push token to
				{
					printf("Error: Memory allocation failed\n");
					return INTERNAL_COMPILER_ERROR;
				}

				get_token_prec();
				if (token->kw == error_token) {
					freeItAll(&top);
					freeItAll(&topFromStack);
					freeItAll(&pom);
					return 1;
				}
				break;
			case '<':
				if (pushAfterTopTerminal(&s) == -1) // pushes precedent_less_token after first terminal, for example E+E -> E+<E (E are nonterminal_tokens)
				{
					return INTERNAL_COMPILER_ERROR;
				}
				if (pushStack(&s, token->kw, illegal_token, token, true, 0) == -1) {
					return INTERNAL_COMPILER_ERROR;
				}

				get_token_prec();
				if (token->kw == error_token) {
					freeItAll(&top);
					freeItAll(&topFromStack);
					freeItAll(&pom);
					return 1;
				}
				break;
			case '>':
				if (!isInStack(&s, precedent_less_token)) // If there is precedent_less_token (<) in stack, you can do all the stuff, if not, it's error
				{
					success = false;
					break;
				}
				freeItAll(&top);
				top = popStack(&s);
				switch (top.data) {
					case num: // Top terminal is number and has higher priority than next token, apply E->int

						if ((deletor = popStack(&s)).data == precedent_less_token) // pops < from stack
						{
							int alert = pushStack(&s, nonterminal_token, INT, top.attributes, true, 0); // Creates new nonterminal_token in stack
							if (alert == -1) {
								return INTERNAL_COMPILER_ERROR;
							}
							if (returned != NULL) {
								*returned = INT;
							}
							if (expected != ILLEGAL && expected != INT && expected != BOOL) {
								freeItAll(&deletor); // Cleaning
								freeItAll(&top);
								freeItAll(&topFromStack);
								return 5;
							}
						} else {
							success = false;
						}
						freeItAll(&deletor);
						break;
					case decim: // Top terminal is float and has higher priority than next token, apply E->float
						if ((deletor = popStack(&s)).data == precedent_less_token) {
							int alert = pushStack(&s, nonterminal_token, FLOAT64, top.attributes, true, 0);
							if (alert == -1) {
								return INTERNAL_COMPILER_ERROR;
							}
							if (returned != NULL) {
								*returned = FLOAT64; // configure return type
							}
							if (expected != ILLEGAL && expected != FLOAT64 && expected != BOOL) {
								freeItAll(&deletor);
								freeItAll(&top);
								freeItAll(&topFromStack);
								return 5;
							}
						} else {
							success = false;
						}
						freeItAll(&deletor);
						break;
					case text: // Top terminal is string and... E->string
						if ((deletor = popStack(&s)).data == precedent_less_token) {
							int alert = pushStack(&s, nonterminal_token, STRING, top.attributes, true, 0);
							if (alert == -1) {
								return INTERNAL_COMPILER_ERROR;
							}
							if (returned != NULL) {
								*returned = STRING; // configure return type
							}
							if (expected != ILLEGAL && expected != STRING && expected != BOOL) {
								freeItAll(&deletor);
								freeItAll(&top);
								freeItAll(&topFromStack);
								return 5;
							}
							fprintf(stderr, "7 ");
						} else {
							success = false;
						}
						freeItAll(&deletor);
						break;
					case id: // Apply E->id
						if ((deletor = popStack(&s)).data == precedent_less_token) {
							if (false) {
								deleteStack(&s); // Some cleaning
								freeItAll(&deletor);
								freeItAll(&topFromStack);
								freeItAll(&top);
								return 3;
							} else {
								int alert = pushStack(&s, nonterminal_token, INT, top.attributes, false, 0);
								if (alert == -1) {
									return INTERNAL_COMPILER_ERROR;
								}
							}
							if (returned != NULL) {
								*returned = INT; // Configure return type
							}

							fprintf(stderr, "4 ");
						} else {
							deleteStack(&s); // Some cleaning
							freeItAll(&deletor);
							freeItAll(&pom);
							freeItAll(&topFromStack);
							freeItAll(&top);
							return 2;
						}
						freeItAll(&deletor);
						break;
					case rbracket: // Apply E->(E)
						freeItAll(&pom);
						pom = popStack(&s);
						if (pom.data == nonterminal_token) {
							if ((deletor = popStack(&s)).data == lbracket) {
								freeItAll(&deletor);
								if ((deletor = popStack(&s)).data == precedent_less_token) {

									int alert = pushStack(&s, nonterminal_token, pom.dataType, pom.attributes, pom.stillCons, 0);
									if (alert == -1) {
										return INTERNAL_COMPILER_ERROR;
									}
									freeItAll(&pom);
									fprintf(stderr, "3 ");
								} else {
									success = false;
								}
								freeItAll(&deletor);
							} else {
								success = false;
							}
							freeItAll(&deletor);

						} else {
							success = false;
						}
						freeItAll(&pom);
						freeItAll(&top);
						freeItAll(&topFromStack);
						break;
					case nonterminal_token: // Apply some E->E op E rule
						freeItAll(&topFromStack);
						topFromStack = popStack(&s);
						correctOperation = false;
						int operation = topFromStack.data;
						if (comp > 0) // If program have already done some comparing, return error
						{
							freeItAll(&deletor);
							freeItAll(&topFromStack);
							freeItAll(&top);
							deleteStack(&s);
							return 5;
						}
						switch (topFromStack.data) // Possible rules for E->E op E
						{
							case plus:
								fprintf(stderr, "1 ");
								correctOperation = true;
								break;
							case minus:
								fprintf(stderr, "8 ");
								if (top.dataType == STRING) // String can use only + operation, nothing else (and of course they can be compared, you curious)
								{
									freeItAll(&top);
									freeItAll(&topFromStack);
									deleteStack(&s);
									return 5;
								}
								correctOperation = true;
								break;
							case multiply:
								fprintf(stderr, "2 ");
								if (top.dataType == STRING) {
									freeItAll(&top);
									freeItAll(&topFromStack);
									deleteStack(&s);
									return 5;
								}
								correctOperation = true;
								break;
							case division:
								fprintf(stderr, "9 ");
								if (top.stillCons == true && top.attributes != NULL && ((top.dataType == INT && top.attributes->i == 0) || (top.dataType == FLOAT64 && top.attributes == 0))) //
								{                                                                                                                                                             // Division by zero
									freeItAll(&top);
									freeItAll(&topFromStack);
									deleteStack(&s);
									return 9;
								}
								if (top.dataType == STRING) {
									freeItAll(&top);
									freeItAll(&topFromStack);
									deleteStack(&s);
									return 5;
								}
								correctOperation = true;
								break;
							case more:
								fprintf(stderr, "10 ");
								if (expected != BOOL && expected != ILLEGAL) // You shouldn't compare anything, total bullshit
								{
									comp--;

								} else {
									comp++;
								}
								correctOperation = true;


								correctOperation = true;
								break;
							case less:
								fprintf(stderr, "11 ");
								if (expected != BOOL && expected != ILLEGAL) {
									comp--;

								} else {
									comp++;
								}
								correctOperation = true;
								break;
							case nequal:
								fprintf(stderr, "12 ");
								if (expected != BOOL && expected != ILLEGAL) {
									comp--;

								} else {
									comp++;
								}
								correctOperation = true;
								break;
							case mequal:
								fprintf(stderr, "13 ");
								if (expected != BOOL && expected != ILLEGAL) {
									comp--;

								} else {
									comp++;
								}
								correctOperation = true;
								break;
							case lequal:
								fprintf(stderr, "14 ");
								if (expected != BOOL && expected != ILLEGAL) {
									comp--;

								} else {
									comp++;
								}
								correctOperation = true;
								break;
							case compare_equal:
								fprintf(stderr, "15 ");
								if (expected != BOOL && expected != ILLEGAL) {
									comp--;

								} else {
									comp++;
								}
								correctOperation = true;
								break;
							default:
								fprintf(stderr, "\nIncorrect operator\n");
								correctOperation = false;
								break;
						}
						if (correctOperation) {

							freeItAll(&topFromStack);
							topFromStack = popStack(&s);
							if (topFromStack.data == nonterminal_token) // Rules are E-> E op E, no E -> bullshit op E
							{
								if ((deletor = popStack(&s)).data == precedent_less_token) {
									if (topFromStack.dataType == illegal_token || top.dataType == ILLEGAL) // Expressions like _ + _ are illegal, sorry
									{
										freeItAll(&deletor);
										freeItAll(&topFromStack);
										freeItAll(&top);
										return 7;
									}
									if (topFromStack.dataType == top.dataType) {
										if (topFromStack.dataType == FLOAT64 && topFromStack.stillCons && top.stillCons) // Totally useless, because variables exist and will break all the stuff
										{
											*topFromStack.attributes->f = decimalOperation(*(topFromStack.attributes->f), *(top.attributes->f), operation);
										} else if (topFromStack.dataType == INT && topFromStack.stillCons && top.stillCons) {
											*topFromStack.attributes->i = decimalOperation(*(topFromStack.attributes->i), *(top.attributes->i), operation);
										}
										switch (top.dataType) {
											case FLOAT64:
												break;
											case INT:
												break;
											case STRING:
												break;
											default:
												break;
										}
										int alert = pushStack(&s, nonterminal_token, topFromStack.dataType, topFromStack.attributes, (topFromStack.stillCons && top.stillCons), 0);
										if (alert == -1) {
											return INTERNAL_COMPILER_ERROR;
										}
										// Creates new nonterminal_token with some E -> E op E rule
										freeItAll(&topFromStack);
										freeItAll(&top);
									} else {
										freeItAll(&deletor);
										freeItAll(&topFromStack);
										freeItAll(&top);
										return 5;
									}

								} else {
									success = false;
								}
								freeItAll(&deletor);
							} else {
								success = false;
							}
							freeItAll(&topFromStack);
							freeItAll(&top);
						} else {
							success = false;
						}
						freeItAll(&topFromStack);
						freeItAll(&top);
						freeItAll(&deletor);
						break;
					default:
						success = false;
						break;
				}
				break;
			case 0:
				freeItAll(&deletor);
				freeItAll(&topFromStack);
				freeItAll(&top);
				deleteStack(&s);
				return 2;
				break;
			default:
				success = false;
				break;
		}
		if (!success)
			break;

		if (token->kw == next)
			break;

		top.data = topTerminal(&s);
	} while ((!isEndingTerminal(top.data)) || (!isEndingTerminal(token->kw)));

	deleteStack(&s);
	freeItAll(&topFromStack);
	freeItAll(&top);

	if ((expected == BOOL && comp != 1) || comp > 1) {
		return 5;
	}

	if (token->kw == next) {
		return -1;
	}

	if (comp < 0) // Comparing were illegal and that's the result
	{
		return 7;
	}

	return 0;
}