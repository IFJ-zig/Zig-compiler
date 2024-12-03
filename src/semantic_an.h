/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák - xhorakt00
*********************************************/

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdbool.h>

#include "errors_enum.h"
#include "symtable.h"
#include "tokens.h"
#include "ast.h"


/**
 * @brief Initializes the symtable
 * 
 */
void semanticInit();

/**
 * @brief Destroys the symtable and frees everything
 * 
 */
void semanticDestroy();

/**
 * @brief Defines a new symbol in the symtable
 * 
 * @param name Name of the symbol
 * @param type Type of the symbol
 * @param isConst Boolean flag indicating whether the symbol is constant
 * @param isNullable Boolean flag indicating whether the symbol is nullable
 * 
 * @return returns 0 on success or REDEFINITION_ERROR is symbol of same name in scope already exists
 * 
 */
int defineSymbol(char *name, varType type, bool isConst, bool isNullable);

/**
 * @brief Undefines a symbol in the symtable
 * 
 * @param name Name of the symbol
 * 
 * @warning This function is only meant to be used to discard of payload symbols from IfElse and while nodes
 */
void undefineSymbol(char *name);

/**
 * @brief Checks if assigning a value of type `type` to a symbol is valid
 * 
 * @param symbol Pointer to the symbol
 * @param type Type of the value we want to assign
 * 
 * @note If the symbol is of type `UNDEFINED`, the type will be assigned to the symbol, used in definiting symbols without explicit type (definition by inference)
 * 
 * @return Returns 0 on success, or TYPE_COMPATIBILITY_ERROR if the types are incompatible
 * 
 */
int assignSymbol(symbol_t *symbol, varType type);

/**
 * @brief Gets a symbol from the symtable
 * 
 * @param name Name of the symbol
 * 
 * @note This function is used pretty much any time we need to get a symbol so it's also used as a check to see if the symbol ever gets used. Only in the case of variable definition we need to use getSymbol and no mark the symbol as used so that is done in the variable definition function.
 * 
 * @return Returns a pointer to the symbol, or NULL if the symbol does not exist
 * 
 */
symbol_t *getSymbol(char *name);

/**
 * @brief Checks if a parameter type is valid
 * 
 * @param kw Keyword of the parameter type
 * 
 * @return Returns `true` if the parameter type is valid, or `false` if it is not
 * 
 */
bool isValidParamType(KeyWord kw);

/**
 * @brief Assigns a parameter to a function
 * 
 * 
 * @param function Pointer to the function symbol
 * @param paramName Token containing the name of the parameter, must be id and have token.s initialized.
 * @param paramType Token specifying the type of the parameter
 * @param isNullable Boolean flag indicating whether the parameter is nullable.
 * 
 * @return Returns `0` on success, or `PARAM_ERROR` on trying to assign a parameter to the `main` function.
 * 
 * @warning symbols are allocated dynamically and must be freed after use.
 */
int assignFunctionParameter(symbol_t *function, Token paramName, Token paramType, bool isNullable);

/**
 * @brief Validates type compatibility from the expression node and figures out the type of the expression
 * 
 * 
 * @param expression Pointer to the root node of the expression tree
 * @param expType Pointer to the variable of the data type of the expression, at first set to UNDEFINED, the function will figure out the type
 * 
 * @return Returns `0` if the types are compatible throughout the expression.
 *         - `UNDEFINED_FUNCTION_OR_VARIABLE_ERROR`: If a variable or function in the expression is not defined.
 *         - `TYPE_COMPATIBILITY_ERROR`: If there is a type mismatch in the expression.
 * 
 */
int checkExprTypesCompatibility(ast_node_exp_t *expression, varType *expType);

/**
 * @brief Checks if the expression contains a symbol that could be nullable
 * 
 * @param expression Pointer to the root node of the expression tree
 * 
 * @return Returns `true` if the expression contains a nullable symbol, or `false` otherwise
 * 
 */
bool nullableInExpr(ast_node_exp_t *expression);

/**
 * @brief Checks if the expression contains a null keyword (meaning trying to assign NULL)
 * 
 * @param expression Pointer to the root node of the expression tree
 * 
 * @return Returns `true` if the expression contains a null keyword, or `false` otherwise
 * 
 */
bool nullKWInExpr(ast_node_exp_t *expression);

/**
 * @brief Checks if the expression contains a non-constant variable
 * 
 * @param expression Pointer to the root node of the expression tree
 * 
 * @return Returns `true` if the expression contains a non-const variable, or `false` otherwise
 * 
 */
bool containsNonConst(ast_node_exp_t *expression);

/**
 * @brief Checks if the expression results in a boolean and thus can be parsed by if/while conditions
 * 
 * @param expression Pointer to the root node of the expression tree
 * 
 * @return Returns `0` if the expression is valid for if/while conditions, or `TYPE_COMPATIBILITY_ERROR` if the expression is not valid
 */
int isValidIfExpression(ast_node_exp_t *expression);

/**
 * @brief Processes a function parameter by validating its type and defining it within the current scope.
 * 
 * 
 * @param paramName Token (kw == id) containing the name of the parameter.
 * @param paramType Token specifying the type of the parameter (dtstr, dtint, dtflt).
 * @param isNullable Boolean flag indicating whether the parameter is nullable.
 * 
 * @return Returns `0` on successful parameter definition.
 *         `PARAM_ERROR`: If the parameter type is invalid.
 * 
 * @warning Ensure we are in a function scope before calling this function.
 */
int processParam(Token paramName, Token paramType, bool isNullable);

/**
 * @brief Convers the keyword to the variable type
 * 
 * @param kw Keyword to convert
 * 
 * @return Returns the variable type 
 */
varType kwToVarType(KeyWord kw);

/**
 * @brief A function for going into bigger depth so syntax analyzer doesn't need to keep track of depth itself
 */
void enterScope();

/**
 * @brief Exits the current scope, and checks if all symbols got used and constants got changed
 * 
 * @return Returns 0 on success, or UNUSED_VARIABLE_ERROR if there are unused/unchanged variables in the scope
 */
int exitScope();


#endif // SEMANTIC_H