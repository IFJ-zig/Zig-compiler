/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák - xhorakt00
*********************************************/

//Upravený 2. příklad z IJC
//Tadeáš Horák xhorakt00
//VUT FIT
//Řešení pro IJC dokončeno 20.04.2024, s výraznějšími úpravy pro IFJ

#ifndef HTAB_H__
#define HTAB_H__

#include <stdbool.h> // bool
#include <string.h>  // size_t

#define HASH_TABLE_SIZE 251 //Should be 1.3x the amount of expected entries and it should be a prime, 251 is just a guess


typedef const char *htab_key_t; // typ klíče

/**
 * @brief Data types for symbols
 */
typedef enum
{
	INT,
	FLOAT,
	STRING,
	BOOL,
	FUNCTION,
	ANYTYPE,
	VOID,
	UNDEFINED
} varType;

/**
 * @brief Symbol in the hash table
 */
typedef struct symbol
{
	htab_key_t key; // klíč
	int depth;
	varType type;
	varType returnType;
	int paramCount;
	struct symbol **params;
	bool isDefined;
	bool isNullable;
	bool isConst;
	bool isUsed;
	bool isChanged;
	bool hasReturn;
	bool isMutable;
} symbol_t; // typedef podle zadání

/**
 * @brief Item in the hash table, all useful data is located in the symbol_t structure
 */
typedef struct htab_itm
{
	symbol_t symbol;
	struct htab_itm *next;
} htab_itm_t;

/**
 * @brief The hash (symbol) table
 */
typedef struct htab
{
	int depth;
	struct htab *previous;
	struct htab *next;
	size_t size;
	size_t arr_size;
	struct htab_itm **arr_ptr;
} htab_t;

/**
 * @brief List of symbol tables
 * 
 * @note This didn't have to be a DLL, and a stack would be more appropriate now, but at first we had a different approach where we needed it to be a DLL, and since the code is now fully working with a DLL I'd rather not change it
 */
typedef struct htabs
{
	struct htab *first;
	struct htab *last;
	int tablesCount;
} htabs_l;



/**
 * @brief Hashing function
 * 
 * @param str string to hash
 * 
 * @return hash
 */
size_t htab_hash_function(htab_key_t str);

/**
 * @brief Initializes the DLL for symtables
 * 
 * @return initialized DLL
 */
htabs_l *symtable_init();

/**
 * @brief Removes the last table inserted into the list
 * 
 * @param list list to remove from
 * 
 * @note This function would very easily allow us to enable variable shadowing if we wanted to
 */
void htab_removeLast(htabs_l *list);

/**
 * @brief Inserts a new symtable to the list
 * 
 * @param list list to insert into
 */
void htab_insertLast(htabs_l *list, htab_t *t);

/**
 * @brief Initializes a new hash table
 * 
 * @param depth depth of the scope (possible variable shadowing with very minimal changes)
 */
htab_t *htab_init(int depth); // konstruktor tabulky

/**
 * @brief Try to find an item in the hash table
 * 
 * @param t table to search in
 * @param key key to search for
 * 
 * @return symbol if found, NULL if not
 */
symbol_t *htab_find(const htab_t *t, htab_key_t key);

/**
 * @brief Define a new symbol in the hash table
 * 
 * @param t table to define in
 * @param key key to define
 * 
 * @note If the symbol already exists, it will not create a new one and will return the existing one
 * 
 * @warning In case of malloc failing this function will exit the program with INTERNAL_COMPILER_ERROR error code
 * 
 * @return symbol if defined
 */
symbol_t *htab_define(htab_t *t, htab_key_t key);

/**
 * @brief Undefine a symbol in the hash table
 * 
 * @param t table to undefine in
 * @param key key to undefine
 * 
 * @note This function is only meant to be used to discard of payload symbols from IfElse and while nodes
 */
void htab_undefine(htab_t *t, htab_key_t key);

/**
 * @brief Clear the hash table
 */
void htab_clear(htab_t *t);

/**
 * @brief Free the hash table (calls htab_clear)
 */
void htab_free(htab_t *t);

/**
 * @brief Get the current depth of the list
 * 
 * @param list list to get the depth from
 * 
 * @return depth
 */
int getCurrentDepth(htabs_l *list);

#endif // HTAB_H__