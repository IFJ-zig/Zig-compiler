/**
 *  Project: IFJ24 Language compiler
 *	
 *	This file contains declarations of functions used for recursive descendance syntax analysis
 *  @file syntax_an.h
 *  @author Adam Vožda, xvozdaa00
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
 * function_header -> pub fn id ( param_def ) return_type
 * @brief Seeks function headers in the source code.
 * 
 * @return int Status code.
 */
int headers();

/**
 * @brief Rewinds the source file on stdin back to start.
 * 
 * @return int Status code.
 */
int rewind_stdin();
/**
 * @brief Analyses functions until end.
 * 
 * @return int Status code.
 */
int program();

/**
 * code -> variable_definition
 * code -> call_or_assignment
 * code -> if_else
 * code -> while_syntax
 * code -> return_syntax
 * code -> empty_variable
 * code -> library_function
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
 * import -> const ifj = @ ( text ) ;
 * @brief Expects import at the begginig of source file.
 *  
 * @return int Status code.
 */
int import();

/**
 * pub fn id ( param_def ) return_type { code }
 * @brief Analyzes function header and calls code to analyze body.
 * 
 * @return int Status code.
 */
int function_analysis();

/**
 * function_call -> id ( parse_params ) ;
 * @brief Analyzes a function call in the source code.
 * 
 * @param expectReturn Indicates if a return value is expected.
 * @param fnCallNode Pointer to the function call node in the AST.
 * @return int Status code.
 */
int function_call(bool expectReturn, ast_default_node_t *fnCallNode);

/**
 * parse_params -> ε
 * parse_params -> term_list parse_params
 * @brief Parses the parameters of a function call.
 * 
 * @param fnCallNode Pointer to the function call node in the AST.
 * @return int Status code.
 */
int parse_params(ast_default_node_t *fnCallNode);

/**
 * return_type -> data_type
 * return_type -> void
 * @brief Expects syntactically correct return type.
 * 
 * @return int Status code.
 */
int return_type();

/**
 * var_type -> data_type_optional data_type
 * data_type -> i32
 * data_type -> f64
 * data_type -> []u8
 * data_type_optional -> ?
 * data_type_optional -> ε
 * @brief Expects syntactically correct data type.
 * 
 * @param isNullable Pointer to a boolean indicating if the data type is nullable.
 * @param canBeVoid Indicates if the data type can be void.
 * @return int Status code.
 */
int data_type(bool *isNullable, bool canBeVoid);

/**
 * param_list -> , param_def
 * param_list -> ε
 * param -> id : var_type
 * @brief Checks parameter list in header function.
 * 
 * @return int Status code.
 */
int param_list();

/**
 * while_syntax -> while ( exp ) unwrapped { code }
 * @brief Parses a while loop syntax.
 * 
 * @return int Status code.
 */
int while_syntax();

/**
 * if_else -> if ( exp ) unwrapped { code } else { code }
 * @brief Parses an if-else statement syntax.
 * 
 * @return int Status code.
 */
int if_else();

/**
 * library_function -> ifj . function_call
 * @brief Parses library function call.
 * 
 * @param expectNext Indicates if the ; token is expected.
 * @return int Status code.
 */
int library_function(bool expectNext);

/**
 * return_syntax -> return ;
 * return_syntax -> return exp
 * @brief Parses a return statement syntax.
 * 
 * @return int Status code.
 */
int return_syntax();

/**
 * call_or_assignment -> function_call
 * call_or_assignment -> assignment
 * @brief Checks if id is function or a variable.
 * 
 * @return int Status code.
 */
int call_or_assignment();

/**
 * variable_definition -> rewrite_type const id variable_definition_type = exp
 * rewrite_type -> var
 * rewrite_type -> const
 * @brief Parses a variable definition.
 * 
 * @param isConst Indicates if the variable is constant.
 * @return int Status code.
 */
int variable_definition(bool isConst);

/**
 * empty_variable -> _ = exp
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