#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "symbol.h"

/** @file symbol.c
 *  @brief You will modify this file and implement nine functions
 *  @details Your implementation of the functions defined in symbol.h.
 *  You may add other functions if you find it helpful. Added functions
 *  should be declared <b>static</b> to indicate they are only used
 *  within this file. The reference implementation added approximately
 *  90 lines of code to this file. This count includes lines containing
 *  only a single closing bracket (}).
 * <p>
 * @author <b>Sadie Henry</b> goes here
 */

/** size of LC3 memory */
#define LC3_MEMORY_SIZE  65536

/** Provide prototype for strdup() */
char *strdup(const char *s);

/** defines data structure used to store nodes in hash table */
typedef struct node {
  symbol_t     symbol;   /**< the data the user is interested in   */
  int          hash;     /**< hash value - makes searching faster  */
  struct node* next;     /**< linked list of symbols at same index */
} node_t;

/** defines the data structure for the hash table */
struct sym_table {
  node_t*  hash_table[SYMBOL_SIZE]; /**< head of linked list for this index */
  char**   addr_table;              /**< look up symbols by addr            */
};

/** djb hash - found at http://www.cse.yorku.ca/~oz/hash.html
 * tolower() call to make case insensitive.
 */

static int symbol_hash (const char* name) {
  unsigned char* str  = (unsigned char*) name;
  unsigned long  hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + tolower(c); /* hash * 33 + c */

  c = hash & 0x7FFFFFFF; /* keep 31 bits - avoid negative values */

  return c;
}

/** initializes the symbol table */
sym_table_t* symbol_init (int lookup_by_addr) {

  sym_table_t* pointer = (sym_table_t*)malloc(sizeof(sym_table_t));
  return pointer;
}

/** free all allocated memory */
void symbol_term (sym_table_t* symTab) {

  symbol_reset(symTab);
  free(symTab);
  
}

/** free any allocated memory */
void symbol_reset(sym_table_t* symTab) {
  
  for(int i = 0; i < SYMBOL_SIZE; i++){
    node_t* temp1 = symTab -> hash_table[i];
    while(temp1 != NULL){
	node_t* temp2 = temp1 -> next;
	free(temp1);
	temp1 = temp2;
    }
    symTab -> hash_table[i] = NULL;
  }
}

/** add a symbol to the table */
int symbol_add (sym_table_t* symTab, const char* name, int addr) {

  symbol_t* tempSym = symbol_find_by_name(symTab, name);

  if(tempSym != NULL)
    return 0;

  else{
    node_t* node = calloc(1, sizeof(node_t));
    node -> symbol.name = strdup(name);
    node -> symbol.addr = addr;
    int hash = symbol_hash(name);
    int index = hash % SYMBOL_SIZE;
    node_t* tempNode = symTab -> hash_table[index];
    node -> next = tempNode;
    symTab -> hash_table[index] = node;
    return 1;
  } 
}

/** search for a symbol */
struct node* symbol_search (sym_table_t* symTab, const char* name, int* hash, int* index) {
  *hash = symbol_hash(name);
  *index = *hash % SYMBOL_SIZE;
  node_t* temp = symTab -> hash_table[*index];
  symbol_t* tempSym = &(temp -> symbol);

  while(temp != NULL){
    *tempSym = temp -> symbol;
    if(strcasecmp(name, tempSym -> name) == 0)
	return temp;
    temp = temp -> next;
  }

  return NULL;
}

/** find a symbol by name */
symbol_t* symbol_find_by_name (sym_table_t* symTab, const char* name) {
  int* hash = malloc(sizeof(int));
  int* index = malloc(sizeof(int));
  node_t* retNode = symbol_search(symTab, name, hash, index);
  if(retNode == NULL)
    return NULL;
  else{
    symbol_t* tempSym = &(retNode -> symbol);
    return tempSym;
  }
}

/** @todo implement this function */
char* symbol_find_by_addr (sym_table_t* symTab, int addr) {
  return NULL;
}

/** iterate through the symbol table */
void symbol_iterate (sym_table_t* symTab, iterate_fnc_t fnc, void* data) {
  node_t* node = malloc(sizeof(node_t));
  symbol_t* symbol = malloc(sizeof(symbol_t));

  for(int i = 0; i < SYMBOL_SIZE; i++){
    node = symTab -> hash_table[i];
    while(node != NULL){
	*symbol = node -> symbol;
	(*fnc)(symbol, data);
	node = node -> next;
    }
  }

  free(node);
  free(symbol);
	
}



