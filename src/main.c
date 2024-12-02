/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains main function for the compiler
 *  @file main.c
 *  @author Adam Vožda, xvozdaa00
 *  @brief Main function for the compiler
*/

#include "errors_enum.h"
#include "semantic_an.h"
#include "syntax_an.h"
#include "tokens.h"

int main() {
	int err = syntax_analyzer();

	if (err) {
		return err;
	}
	semanticDestroy();

	return 0;
}
