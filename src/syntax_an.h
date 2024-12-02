/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains declarations of functions used for recursive descendance syntax analysis
 *  @file codegen.h
 *  @author Daniel Bohata, xbohatd00
 *  @brief Header file for recursive descendance syntax analysis
 */

#ifndef SYNTAX_H
#define SYNTAX_H

#include <stdbool.h>

#include "ast.h"
#include "codegen.h"
#include "errors_enum.h"
#include "expression_parser.h"
#include "semantic_an.h"
#include "symtable.h"
#include "tokens.h"

/**
 * @brief Entry point for the syntax analysis, 
 * first reads headers of functions and then the whole source file.
 * 
 * @return int Status code of the syntax analysis.
 */
int syntax_analyzer();


/**
 * @brief Seeks function headers in the source code.
 * 
 * @return int Status code.
 */
int headers();
/**
 * @brief Analyses functions until end.
 * 
 * @return int Status code.
 */
int program();

/**
 * @brief Analyses body of fucntion, if and swhile.
 * 
 * @return int Status code.
 */
int code();

/**
 * @brief Used to skip function body when looking only for function headers.
 * 
 * @return int Status code.
 */
int skip_function_body();

/**
 * @brief Expects import at the begginig of source file.
 * 
 * @return int Status code.
 */
int checkImport();

/**
 * @brief Analyzes function header and calls code to analyze body.
 * 
 * @return int Status code.
 */
int function_analysis();

/**
 * @brief Analyzes a function call in the source code.
 * 
 * @param expectReturn Indicates if a return value is expected.
 * @param fnCallNode Pointer to the function call node in the AST.
 * @return int Status code.
 */
int function_call(bool expectReturn, ast_default_node_t *fnCallNode);

/**
 * @brief Parses the parameters of a function call.
 * 
 * @param fnCallNode Pointer to the function call node in the AST.
 * @return int Status code.
 */
int parse_params(ast_default_node_t *fnCallNode);

/**
 * @brief Expects syntactically correct return type.
 * 
 * @return int Status code.
 */
int return_type();

/**
 * @brief Expects syntactically correct data type.
 * 
 * @param isNullable Pointer to a boolean indicating if the data type is nullable.
 * @param canBeVoid Indicates if the data type can be void.
 * @return int Status code.
 */
int data_type(bool *isNullable, bool canBeVoid);

/**
 * @brief Checks parameter list in header function.
 * 
 * @return int Status code.
 */
int param_list();

/**
 * @brief Parses a while loop syntax.
 * 
 * @return int Status code.
 */
int while_syntax();

/**
 * @brief Parses an if-else statement syntax.
 * 
 * @return int Status code.
 */
int if_else();

/**
 * @brief Parses library function call.
 * 
 * @param expectNext Indicates if the ; token is expected.
 * @return int Status code.
 */
int library_function(bool expectNext);

/**
 * @brief Parses a return statement syntax.
 * 
 * @return int Status code.
 */
int return_syntax();

/**
 * @brief Checks if id is function or a variable.
 * 
 * @return int Status code.
 */
int call_or_assignment();

/**
 * @brief Parses a variable definition.
 * 
 * @param isConst Indicates if the variable is constant.
 * @return int Status code.
 */
int variable_definition(bool isConst);

/**
 * @brief Parses an empty variable definition.
 * 
 * @return int Status code.
 */
int empty_variable();

/**
 * @brief Reads the next token from lexical analysis.
 * 
 * @return int Status code.
 */
int read_token();

/**
 * @brief Loads the IFJ24 library functions.
 */
void loadIFJ24();
#endif // SYNTAX_H