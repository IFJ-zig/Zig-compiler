#ifndef CODEGEN
#define CODEGEN 1

void printHeader();

typedef struct bst_items {
  ast_node_exp_t **nodes;     // pole uzlu
  int capacity;           // kapacita alokované paměti v počtu položek
  int size;               // aktuální velikost pole v počtu položek
} bst_items_t;

#endif