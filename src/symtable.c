/********************************************
* Projekt: Implementace překladače imperativního jazyka IFJ24
* Tvůrci: Tadeáš Horák, xhorakt00
*********************************************/

//This file is for keeping track of variable names, function names etc
//It does so by creating a DLL list with multiple hash tables sorted by their depth
//I believe this would be a good enough system to use since we don't really expect
//anyone to go too much into depth as that is not a recommended programming practice

//This system could by simplyfied by only keeping track of the previous (less immersed) level
//since when we go to a previous level we can immediatelly delete the more immersed table
//because we won't be going back to it, however, keeping it like this makes it simpler for me

#include "symtable.h"
#include "errors_enum.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HASH_TABLE_SIZE 251  //Should be 1.3x times if expected entries and it should be a prime, 251 is just a guess

//remove me, im just for testing!
int main(){
    htabs_l *tables = malloc(sizeof(struct htabs));
    htab_t *t = htab_init(HASH_TABLE_SIZE, 0);
    char *symbol = "counter";
    htab_data_t *pair = htab_find(t, symbol);
    if(pair == NULL)
        printf("it was not found, this is expected behaviour\n");
    htab_create(t, symbol);
    pair = htab_find(t, symbol);
    if(pair != NULL)
        printf("it was found, this is expected behaviour\n");
}


size_t htab_hash_function(const char *str) {
    uint32_t h=0;     // musí mít 32 bitů
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
    h = 65599*h + *p;
    return h;
}

htabs_l *symtable_init(htabs_l *list){
    list->active = NULL;
    list->first = NULL;
    list->last = NULL;
    list->tablesCount = 0;
}

//Initializes the hash table
htab_t *htab_init(const size_t n, int depth){
    htab_t *tab = malloc(sizeof(htab_t));
    if(tab == NULL){
        fprintf(stderr, "Failed to allocate memory for hash table!\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    tab->size = 0;
    tab->arr_size = n;
    tab->arr_ptr = calloc(n, sizeof(htab_itm_t*));
    tab->depth = depth;
    if(tab->arr_ptr == NULL){
        fprintf(stderr, "Failed to allocate memory while initializing hash table!\n");
        exit(INTERNAL_COMPILER_ERROR);
    }
    return tab;
}

//This function inserts the newly created hash table into a list of hash tables sorted by their depth
void htab_insert(htabs_l *list, htab_t *t){
    if(list->first == NULL){
        list->first = t;
        list->last = t;
        list->tablesCount++;
    }
    htab_t *lt = list->first;
    int depth = lt->depth;
    while(depth < t->depth && lt->next != NULL){
        lt = lt->next;
        depth = lt->depth;
    }
    if(depth == t->depth)
        fprintf(stderr, "Error trying to insert a table that already exists!\n");

    if(depth > t->depth){   //Insert before
        t->next = lt;
        t->previous = lt->previous;
        if(lt->previous == NULL)
            list->first = t;
        else
            lt->previous->next = t;
        lt->previous = t;
    }
    if(depth < t->depth){    //Insert after
        t->next = lt->next;
        t->previous = lt;
        if(lt->previous == NULL)
            list->last = t;
        else
            lt->next->previous = t;
        lt->previous = t;
    }
    list->tablesCount++;
}

void htab_free(htab_t * t){
    htab_clear(t);
    free(t->arr_ptr);
    free(t);
}

//Deletes all entries in hash table
void htab_clear(htab_t * t){
    htab_itm_t *previtm = NULL;
    for(size_t i=0; i<t->arr_size; i++){
        htab_itm_t *itm = t->arr_ptr[i];
        while(itm!=NULL){
            previtm=itm;
            itm=itm->next;
            free((char *)previtm->data.key);    //is set to const char*, need to cast away constness
            free(previtm);
        }
        t->arr_ptr[i]=NULL;
    }
}

htab_data_t *htab_find(const htab_t * t, htab_key_t key){
    size_t hash = htab_hash_function(key);
    int index = hash % t->arr_size;
    htab_itm_t *itm = t->arr_ptr[index];
    while(itm!=NULL){
        if(!strcmp(key, itm->data.key))
            return &itm->data;
        itm=itm->next;
    }
    return NULL;
}

//This function will add an entry in the hash table, if it already exists, it will return the existing entry
htab_data_t *htab_create(htab_t *t, htab_key_t key){
    htab_data_t *data = htab_find(t, key);
    if(data!=NULL){
        return data;
    }
    else{   //Creating new entry
        size_t hash = htab_hash_function(key);
        size_t index = hash % t->arr_size;
        htab_itm_t *itm = t->arr_ptr[index];
        if(itm!=NULL){
            while(itm->next!=NULL){ //Advance until we get a free spot
                itm=itm->next;
            }
        }
        htab_itm_t *newitm = malloc(sizeof(htab_itm_t));
        if(newitm==NULL){
            fprintf(stderr, "Failed to allocate memory while creating a new item in hash table\n");
            exit(INTERNAL_COMPILER_ERROR);
        }
        if(itm==NULL)
            t->arr_ptr[index] = newitm;
        else
            itm->next = newitm;
        newitm->next=NULL;

        char *keyString = malloc(sizeof(char) * (strlen(key)+1));
        if(keyString==NULL){
            fprintf(stderr, "Failed to allocate memory while inserting a new item into the hash table!\n");
            free(newitm);
            exit(INTERNAL_COMPILER_ERROR);
        }
        strcpy(keyString, key);
        newitm->data.key=keyString;
        t->size++;
        return &newitm->data;
    }
}