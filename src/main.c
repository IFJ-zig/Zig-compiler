/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#include "tokens.h"
#include "errors_enum.h"

int main() {
	List *pL;
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
	}

	//TODO další kontroly a generace kodu

	LDestroy(pL);
	free(pL);

	return 0;
}
