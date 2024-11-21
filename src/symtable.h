/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
*********************************************/

//Upravený 2. příklad z IJC
//Tadeáš Horák xhorakt00
//VUT FIT
//Řešení pro IJC dokončeno 20.04.2024, úpravy pro IFJ 25.10.2024

#ifndef HTAB_H__
#define HTAB_H__

#include <string.h>     // size_t
#include <stdbool.h>    // bool

// Typy:
typedef const char * htab_key_t;        // typ klíče

typedef enum {
    INT, FLOAT, STRING, BOOL, FUNCTION, VOID
} varType;

// Data v tabulce:
typedef struct symbol {
    htab_key_t    key;          // klíč
    int depth;
    varType type;
    varType returnType;
    int paramCount;
    struct symbol **params;
    bool isDefined;
    bool isNullable;
    bool isConst;
} symbol_t;                  // typedef podle zadání

// Prvek tabulky:
typedef struct htab_itm {
    symbol_t symbol;
    struct htab_itm *next;
} htab_itm_t;

// Tabulka:
typedef struct htab {
    int depth;
    struct htab *previous;
    struct htab *next;
    size_t size;
    size_t arr_size;
    struct htab_itm **arr_ptr;
} htab_t;

// DLL list s hash tabulkama podle hloubky
typedef struct htabs {
    struct htab *first;
    struct htab *last;
    int tablesCount;
} htabs_l;


// Rozptylovací (hash) funkce (stejná pro všechny tabulky v programu)
// Pokud si v programu definujete stejnou funkci, použije se ta vaše.
size_t htab_hash_function(htab_key_t str);

// Initializace symtable:
htabs_l *symtable_init();

//  Vložení tabulky do listu
void htab_removeLast(htabs_l *list);
void htab_insertLast(htabs_l *list, htab_t *t);

// Funkce pro práci s tabulkou:
htab_t *htab_init(int depth);              // konstruktor tabulky

symbol_t * htab_find(const htab_t * t, htab_key_t key);  // hledání
symbol_t * htab_define(htab_t * t, htab_key_t key);

void htab_clear(htab_t * t);    // ruší všechny záznamy
void htab_free(htab_t * t);     // destruktor tabulky

int getCurrentDepth(htabs_l *list);

#endif // HTAB_H__