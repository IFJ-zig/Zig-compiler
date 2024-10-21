#ifndef ERROR_CODES_H
#define ERROR_CODES_H

typedef enum
{
	//Lexical errors
	LEXEM_ERROR = 1,

	//Syntactic errors
	SYNTACTIC_ANALYSIS_ERROR = 2,

	//Semantic errors
	UNDEFINED_FUNCTION_OR_VARIABLE_ERROR = 3,
	PARAM_ERROR = 4,              // Incorrect number/type of parameters in function call;
								  // wrong type or disallowed discard of function return value
	REDEFINITION_ERROR = 5,       // Redefinition of variable or function or assignment to an unmodifiable variable
	RETURN_EXPRESSION_ERROR = 6,  // Missing/extraneous expression in return statement
	TYPE_COMPATIBILITY_ERROR = 7, // Type compatibility issues in arithmetic, string, and relational expressions;
	TYPE_INFERENCE_ERROR = 8,     // Variable type not specified and cannot be inferred from expression used
	UNUSED_VARIABLE_ERROR = 9,    // Unused variable in its scope; modifiable variable without change after initialization
	OTHER_SEMANTIC_ERROR = 10,    // Other semantic errors

	//Other errors
	INTERNAL_COMPILER_ERROR = 99
} ErrorCode;

#endif // ERROR_CODES_H