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
typedef int htab_value_t;               // typ hodnoty

typedef enum {
    INT, FLOAT, STRING, BOOL
} varType;

// Data v tabulce:
typedef struct htab_data {
    htab_key_t    key;          // klíč
    htab_value_t  value;        // asociovaná hodnota
    int depth;
    varType type;
    bool isDefined;
} htab_data_t;                  // typedef podle zadání

// Tabulka:
typedef struct htab {
    int depth;
    struct htab *previous;
    struct htab *next;
    size_t size;
    size_t arr_size;
    struct htab_itm **arr_ptr;
} htab_t;

typedef struct htab_itm {
    htab_data_t data;
    struct htab_itm *next;
} htab_itm_t;

// DLL list s hash tabulkama podle hloubky
typedef struct htabs {
    struct htab *first;
    struct htab *last;
    struct htab *active;
    int tablesCount;
} htabs_l;


// Rozptylovací (hash) funkce (stejná pro všechny tabulky v programu)
// Pokud si v programu definujete stejnou funkci, použije se ta vaše.
size_t htab_hash_function(htab_key_t str);

// Initializace symtable:
htabs_l *symtable_init(htabs_l *list);

// Funkce pro práci s tabulkou:
htab_t *htab_init(const size_t n, int depth);              // konstruktor tabulky
size_t htab_size(const htab_t * t);             // počet záznamů v tabulce
size_t htab_bucket_count(const htab_t * t);     // velikost pole

htab_data_t * htab_find(const htab_t * t, htab_key_t key);  // hledání
htab_data_t * htab_create(htab_t * t, htab_key_t key);

bool htab_erase(htab_t * t, htab_key_t key);    // ruší zadaný záznam

// for_each: projde všechny záznamy a zavolá na ně funkci f
// Pozor: f nesmí měnit klíč .key ani přidávat/rušit položky
void htab_for_each(const htab_t * t, void (*f)(htab_data_t *data));

void htab_clear(htab_t * t);    // ruší všechny záznamy
void htab_free(htab_t * t);     // destruktor tabulky

// výpočet a tisk statistik délky seznamů (min,max,avg) do stderr:
void htab_statistics(const htab_t * t);

#endif // HTAB_H__