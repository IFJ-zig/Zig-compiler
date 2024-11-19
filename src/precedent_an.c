#include "precedent_an.h"


static char precedentTable[TABLE_SIZE * TABLE_SIZE + 1] = {
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

Token *token_prec;
List *tokenList_prec;

int get_token_prec() {
	token_prec = LGetAct(tokenList_prec);
	LActNext(tokenList_prec);
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
		fprintf(stderr, "Error: Stack is empty\n");
		return -1;
	}
	t_StackItem *helper = s->first;
	while (helper != NULL && (helper->data == nonterminal_token || helper->data == precedent_less_token)) {
		helper = helper->next;
	}
	if (helper != NULL) {
		return helper->data;
	} else {
		fprintf(stderr, "Error: topterm\n");
		return -1;
	}
}

void freeStackItem(t_StackItem *stack) // Free attributes from stack element. Important for no memory leaks
{
	if (stack->attributes != NULL) {
		if (stack->attributes->s != NULL) {
			free(stack->attributes->s);
			stack->attributes->s = NULL;
		}
		free(stack->attributes);
		stack->attributes = NULL;
	}
}
t_StackItem popStack(t_Stack *s) // Pops element from stack. WARNING - call freeStackItem on popped item for no memleaks
{
	if (s->first == NULL) {
		t_StackItem empty;
		empty.data = error_token;
		empty.attributes = NULL;
		empty.dataType = void_token;
		empty.next = NULL;
		return empty;
	}
	t_StackItem *helper;
	t_StackItem returnItem;
	helper = s->first;
	s->first = s->first->next;
	returnItem.dataType = helper->dataType;
	returnItem.data = helper->data;
	returnItem.attributes = helper->attributes;
	returnItem.next = NULL;
	returnItem.ranking = helper->ranking;
	returnItem.stillCons = helper->stillCons;
	free(helper);
	helper = NULL;
	return returnItem;
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
		fprintf(stderr, "Error: malloc\n");
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
	newItem->attributes = createAttributes(attributes); // Create attributes for stack element - IMPORTANT - call freeStackItem after pop, or memory leaks occur
	if (newItem->attributes == NULL) {
		fprintf(stderr, "Error: malloc\n");
		return -1;
	}

	return 0;
}

int pushAfterTopTerminal(t_Stack *s) // Pushes < after first terminal in stack
{
	if (s->first == NULL) {
		fprintf(stderr, "Error: Stack is empty\n");
		return -1;
	}
	t_StackItem *helper = s->first;
	if (helper->data != nonterminal_token && helper->data != precedent_less_token) {
		return pushStack(s, precedent_less_token, void_token, NULL, true, 0);
	}

	while (helper->next != NULL && (helper->next->data == nonterminal_token || helper->next->data == precedent_less_token)) {
		helper = helper->next;
	}
	if (helper->next != NULL) // Terminal found, allocate new element.
	{
		t_StackItem *newItem = malloc(sizeof(struct StackItem));
		if (newItem == NULL) {
			fprintf(stderr, "Error: malloc\n");
			return -1;
		}
		newItem->next = helper->next;
		newItem->data = precedent_less_token;
		newItem->dataType = void_token;
		newItem->attributes = NULL;
		helper->next = newItem;
		return 0;
	} else {
		fprintf(stderr, "Error: pushafter\n");
		return -1;
	}
}

bool isInStack(t_Stack *s, int data) // Finds wheter type of element is in stack or not.
{
	if (s->first == NULL) {
		return false;
	}
	t_StackItem *helper = s->first;
	while (helper != NULL && helper->data != data) {
		helper = helper->next;
	}
	return helper != NULL;
}

void deleteStack(t_Stack *s) // Deletes stack - only option where you dont need to call freeStackItem separatedly. Still, dont forget call freeStackItem on your popStacked elements.
{
	t_StackItem helper;
	while ((helper = popStack(s)).data != error_token) {
		freeStackItem(&helper);
	}
	s->first = NULL;
}

void printStack(t_Stack *s) // Prints stack
{
	fprintf(stderr, "---- stack print ----\n");
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
		fprintf(stderr, "Error: Invalid token value\n");
		return -1;
	}
	int j = precedentTableTranslator(tokenInput);
	if (j == -1) {
		fprintf(stderr, "Error: Invalid token value\n");
		return -1;
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
	empty.dataType = void_token;
	empty.next = NULL;
	return empty;
}


int precedentAnalysis(varType expected, varType *returned, Token *_token, List *_tokenList_prec) {
	token_prec = _token;
	tokenList_prec = _tokenList_prec;
	int comp = 0;
	t_Stack s;
	bool success = true; // Input is gramatically correct
	t_StackItem topFromStack = emptyItem();
	bool correctOperation;
	stackInit(&s);
	t_StackItem top = emptyItem();
	t_StackItem helper = emptyItem();
	t_StackItem deletor = emptyItem();

	pushStack(&s, next, void_token, NULL, true, 0);

	do {
		printStack(&s);
		freeStackItem(&top);
		freeStackItem(&topFromStack);

		top.data = topTerminal(&s);
		int tableValue = getValuePrecedentTable(top.data, token_prec->kw);
		switch (tableValue) {
			case '=':
				if (pushStack(&s, token_prec->kw, void_token, token_prec, true, 0) == -1) // Push token to
				{
					printf("Error: Memory allocation failed\n");
					return INTERNAL_COMPILER_ERROR;
				}

				get_token_prec();
				if (token_prec->kw == error_token) {
					freeStackItem(&top);
					freeStackItem(&topFromStack);
					freeStackItem(&helper);
					return 1;
				}
				break;
			case '<':
				if (pushAfterTopTerminal(&s) == -1) // pushes precedent_less_token after first terminal, for example E+E -> E+<E (E are nonterminal_tokens)
				{
					return INTERNAL_COMPILER_ERROR;
				}
				if (pushStack(&s, token_prec->kw, void_token, token_prec, true, 0) == -1) {
					return INTERNAL_COMPILER_ERROR;
				}

				get_token_prec();
				if (token_prec->kw == error_token) {
					freeStackItem(&top);
					freeStackItem(&topFromStack);
					freeStackItem(&helper);
					return 1;
				}
				break;
			case '>':
				if (!isInStack(&s, precedent_less_token)) // If there is precedent_less_token (<) in stack, you can do all the stuff, if not, it's error
				{
					success = false;
					break;
				}
				freeStackItem(&top);
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
							if (expected != VOID && expected != INT && expected != BOOL) {
								freeStackItem(&deletor); // Cleaning
								freeStackItem(&top);
								freeStackItem(&topFromStack);
								return 5;
							}
						} else {
							success = false;
						}
						freeStackItem(&deletor);
						break;
					case decim: // Top terminal is float and has higher priority than next token, apply E->float
						if ((deletor = popStack(&s)).data == precedent_less_token) {
							int alert = pushStack(&s, nonterminal_token, FLOAT, top.attributes, true, 0);
							if (alert == -1) {
								return INTERNAL_COMPILER_ERROR;
							}
							if (returned != NULL) {
								*returned = FLOAT; // configure return type
							}
							if (expected != VOID && expected != FLOAT && expected != BOOL) {
								freeStackItem(&deletor);
								freeStackItem(&top);
								freeStackItem(&topFromStack);
								return 5;
							}
						} else {
							success = false;
						}
						freeStackItem(&deletor);
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
							if (expected != VOID && expected != STRING && expected != BOOL) {
								freeStackItem(&deletor);
								freeStackItem(&top);
								freeStackItem(&topFromStack);
								return 5;
							}
							fprintf(stderr, "7 ");
						} else {
							success = false;
						}
						freeStackItem(&deletor);
						break;
					case id: // Apply E->id
						if ((deletor = popStack(&s)).data == precedent_less_token) {
							if (false) {
								deleteStack(&s); // Some cleaning
								freeStackItem(&deletor);
								freeStackItem(&topFromStack);
								freeStackItem(&top);
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
							freeStackItem(&deletor);
							freeStackItem(&helper);
							freeStackItem(&topFromStack);
							freeStackItem(&top);
							return 2;
						}
						freeStackItem(&deletor);
						break;
					case rbracket: // Apply E->(E)
						freeStackItem(&helper);
						helper = popStack(&s);
						if (helper.data == nonterminal_token) {
							if ((deletor = popStack(&s)).data == lbracket) {
								freeStackItem(&deletor);
								if ((deletor = popStack(&s)).data == precedent_less_token) {

									int alert = pushStack(&s, nonterminal_token, helper.dataType, helper.attributes, helper.stillCons, 0);
									if (alert == -1) {
										return INTERNAL_COMPILER_ERROR;
									}
									freeStackItem(&helper);
									fprintf(stderr, "3 ");
								} else {
									success = false;
								}
								freeStackItem(&deletor);
							} else {
								success = false;
							}
							freeStackItem(&deletor);

						} else {
							success = false;
						}
						freeStackItem(&helper);
						freeStackItem(&top);
						freeStackItem(&topFromStack);
						break;
					case nonterminal_token: // Apply some E->E op E rule
						freeStackItem(&topFromStack);
						topFromStack = popStack(&s);
						correctOperation = false;
						int operation = topFromStack.data;
						if (comp > 0) // If program have already done some comparing, return error
						{
							freeStackItem(&deletor);
							freeStackItem(&topFromStack);
							freeStackItem(&top);
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
									freeStackItem(&top);
									freeStackItem(&topFromStack);
									deleteStack(&s);
									return 5;
								}
								correctOperation = true;
								break;
							case multiply:
								fprintf(stderr, "2 ");
								if (top.dataType == STRING) {
									freeStackItem(&top);
									freeStackItem(&topFromStack);
									deleteStack(&s);
									return 5;
								}
								correctOperation = true;
								break;
							case division:
								fprintf(stderr, "9 ");
								if (top.stillCons == true && top.attributes != NULL && ((top.dataType == INT && top.attributes->i == 0) || (top.dataType == FLOAT && top.attributes == 0))) //
								{                                                                                                                                                           // Division by zero
									freeStackItem(&top);
									freeStackItem(&topFromStack);
									deleteStack(&s);
									return 9;
								}
								if (top.dataType == STRING) {
									freeStackItem(&top);
									freeStackItem(&topFromStack);
									deleteStack(&s);
									return 5;
								}
								correctOperation = true;
								break;
							case more:
								fprintf(stderr, "10 ");
								if (expected != BOOL && expected != VOID) // You shouldn't compare anything, total bullshit
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
								if (expected != BOOL && expected != VOID) {
									comp--;

								} else {
									comp++;
								}
								correctOperation = true;
								break;
							case nequal:
								fprintf(stderr, "12 ");
								if (expected != BOOL && expected != VOID) {
									comp--;

								} else {
									comp++;
								}
								correctOperation = true;
								break;
							case mequal:
								fprintf(stderr, "13 ");
								if (expected != BOOL && expected != VOID) {
									comp--;

								} else {
									comp++;
								}
								correctOperation = true;
								break;
							case lequal:
								fprintf(stderr, "14 ");
								if (expected != BOOL && expected != VOID) {
									comp--;

								} else {
									comp++;
								}
								correctOperation = true;
								break;
							case compare_equal:
								fprintf(stderr, "15 ");
								if (expected != BOOL && expected != VOID) {
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

							freeStackItem(&topFromStack);
							topFromStack = popStack(&s);
							if (topFromStack.data == nonterminal_token) // Rules are E-> E op E, no E -> bullshit op E
							{
								if ((deletor = popStack(&s)).data == precedent_less_token) {
									if (topFromStack.dataType == void_token || top.dataType == VOID) // Expressions like _ + _ are VOID, sorry
									{
										freeStackItem(&deletor);
										freeStackItem(&topFromStack);
										freeStackItem(&top);
										return 7;
									}
									if (topFromStack.dataType == top.dataType) {
										if (topFromStack.dataType == FLOAT && topFromStack.stillCons && top.stillCons) // Totally useless, because variables exist and will break all the stuff
										{
											*topFromStack.attributes->f = decimalOperation(*(topFromStack.attributes->f), *(top.attributes->f), operation);
										} else if (topFromStack.dataType == INT && topFromStack.stillCons && top.stillCons) {
											*topFromStack.attributes->i = decimalOperation(*(topFromStack.attributes->i), *(top.attributes->i), operation);
										}
										switch (top.dataType) {
											case FLOAT:
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
										freeStackItem(&topFromStack);
										freeStackItem(&top);
									} else {
										freeStackItem(&deletor);
										freeStackItem(&topFromStack);
										freeStackItem(&top);
										return 5;
									}

								} else {
									success = false;
								}
								freeStackItem(&deletor);
							} else {
								success = false;
							}
							freeStackItem(&topFromStack);
							freeStackItem(&top);
						} else {
							success = false;
						}
						freeStackItem(&topFromStack);
						freeStackItem(&top);
						freeStackItem(&deletor);
						break;
					default:
						success = false;
						break;
				}
				break;
			case 0:
				freeStackItem(&deletor);
				freeStackItem(&topFromStack);
				freeStackItem(&top);
				deleteStack(&s);
				return 2;
				break;
			default:
				success = false;
				break;
		}
		if (!success)
			break;

		if (token_prec->kw == next)
			break;

		top.data = topTerminal(&s);
	} while ((!isEndingTerminal(top.data)) || (!isEndingTerminal(token_prec->kw)));


	deleteStack(&s);
	freeStackItem(&topFromStack);
	freeStackItem(&top);

	if ((expected == BOOL && comp != 1) || comp > 1) {
		return 5;
	}
	if (!success) {
		return SYNTACTIC_ANALYSIS_ERROR;
	}

	if (token_prec->kw == end) {
		fprintf(stderr, "Error: Expected token, but found eof\n");
		return -1;
	}

	if (comp < 0) // Comparing were VOID and that's the result
	{
		return 7;
	}


	return 0;
}