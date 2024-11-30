/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Ivo Puchnar, xpuchn02
*********************************************/
#include "errors_enum.h"
#include "semantic_an.h"
#include "syntax_an.h"
#include "tokens.h"

int main() {
	//TODO další kontroly a generace kodu

	int err = syntax_analyzer();printf("\n\n%d\n\n",err);

	if (err) {
		return err;
	}
	semanticDestroy();

	return 0;
}
