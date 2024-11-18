/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#include "errors_enum.h"
#include "syntax_an.h"
#include "tokens.h"

int main() {
	/*List *pL;
	if (!(pL = (List *)malloc(sizeof(List)))) {
		return INTERNAL_COMPILER_ERROR;
	}
	ListInit(pL);

	//Lexikální analýza
	int err = Lexem_analyzer(pL);
	if (err) {
		LDestroy(pL);
		free(pL);
		return err;
	}*/

	//TODO další kontroly a generace kodu
	int err = syntax_analyzer(/*pL*/);

	/*LDestroy(pL);
	free(pL);*/
	if (err) {
		return err;
	}

	return 0;
}
