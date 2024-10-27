#include "precedent_an.h"

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
	int n = 100;
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